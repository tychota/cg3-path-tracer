# include "cg3/manager/render_manager.hpp"

# include "cg3/common/timing.hpp"

# include "cg3/common/utils.hpp"

using namespace std;

RenderManager::RenderManager( tiny_vec< size_t, 2 > resolution
                      , shared_ptr< Renderer >      r
                      , shared_ptr< Strategy >      s
                      , size_t num_threads
                      )
: _cancel_request( false )
, _renderer( r )
, _resolution( resolution )
, _image( s->create_target_tiles( resolution ), resolution )
, _strategy( s )
, _tile_pool()
, _num_threads( num_threads )
{ }

void RenderManager::render_image()
{
  _cancel_request = false;
  _tile_pool = _strategy->create_render_tiles( _resolution );

  // process tiles
  {
    auto worker = [this]()
    {
      Tile current_tile;
      while ( ! this->_cancel_request && this->next_tile( current_tile ) )
      {
        this-> render_tile( current_tile );
        this->compose_tile( current_tile );
      }
    };

    vector< thread > workers;

    for( size_t wi = 0; wi < _num_threads; wi++ )
      workers.push_back( thread( worker ) );

    for( auto& w : workers )
      w.join();
  }

  # if defined USE_RENDERMANAGER_SIGNALS
    lock_guard< mutex > image_lock( _image_mutex );
    signal_image_completed( _image.full_image() );
  # endif
}

void RenderManager::send_cancel_request()
{
  _cancel_request = true;
}

Tile::image_type RenderManager::get_image() const
{
  lock_guard< mutex > image_lock( _image_mutex );
  return _image.full_image();
}

bool RenderManager::next_tile( Tile& t )
{
  lock_guard< mutex > tile_pool_lock( _tile_pool_mutex );
  if( _tile_pool.empty() ) return false;
  t = move( _tile_pool.back() );
  _tile_pool.pop_back();
  return true;
}

void RenderManager::render_tile( Tile& t )
{
  cout << "thread #" << this_thread::get_id() 
       << " render tile (" << t.origin[0] << ", " << t.origin[1] << ")(" << t.image.w() << 'x' << t.image.h() << 'x' << t.samples_per_pixel << ')' 
       << endl;

  shared_ptr< Camera const > cam = _renderer->camera();
  position< 3 > orig = cam->origin();

  for( size_t y = 0; y < t.image.h(); ++y)
    for( size_t x = 0; x < t.image.w(); ++x)
    {
      radiance_rgb flux( 0,0,0 );

      // accumulate subpixel samples
      for( size_t ss = 0; ss < t.samples_per_pixel; ss++ )
      {
        position<2> const sample_pos = t.get_subpixel_sample( ss );
        direction<3> const dir = cam->ray_direction( x + t.origin[0] + sample_pos[0]
                                                   , y + t.origin[1] + sample_pos[1]
                                                   , _resolution
                                                   );
        // gather radiance along a ray through the pixel
        intersection_info hit = intersection_info( ray( orig, dir ) );
        hit.set_potential( tiny_vec< real_type, 3>( 1,1,1 ) );
        flux += _renderer->trace( hit, 0 );
      }
      
      flux /= real_type( t.samples_per_pixel );
      t.image( x, y ) = flux;
    }
}

void RenderManager::compose_tile( Tile const& source )
{
  lock_guard< mutex > image_lock( _image_mutex );
  auto const& updated_tile = _image.compose( source );

  # if defined USE_RENDERMANAGER_SIGNALS
    signal_tile_completed( updated_tile );
  # endif
}
