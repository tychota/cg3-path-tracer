# include "cg3/manager/progressive_strategy.hpp"

using namespace std;

vector< Tile > ProgressiveStrategy::create_render_tiles( tiny_vec< size_t, 2 > resolution )
{
  tiny_vec< size_t, 2 > const width = resolution / _subdivisions;
  tiny_vec< size_t, 2 > const last_width( resolution[0] % _subdivisions[0]
                                        , resolution[1] % _subdivisions[1] );

  vector< Tile > pool;
  pool.reserve( _subdivisions[0] * _subdivisions[1] );

  for( size_t s = 0; s < _samples_per_pixel; s++ )
  {
    auto tile_layer = create_tile_field( resolution, 1, 1 / real_type( _samples_per_pixel ) );
    pool.insert( pool.begin(), tile_layer.begin(), tile_layer.end() );
  }
  return pool;
}