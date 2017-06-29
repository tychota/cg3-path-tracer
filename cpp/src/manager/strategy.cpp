#include "cg3/manager/strategy.hpp"

using namespace std;

vector< Tile > Strategy::create_render_tiles( tiny_vec< size_t, 2 > resolution )
{
  return create_tile_field( resolution, _samples_per_pixel, 1. );
}

vector< Tile > Strategy::create_target_tiles( tiny_vec< size_t, 2 > resolution )
{
  return create_tile_field( resolution, 0, 0. );
}


vector< Tile > Strategy::create_tile_field( tiny_vec< size_t, 2 > resolution, size_t const samples_per_pixel, real_type const weight )
{
  tiny_vec< size_t, 2 > const width = resolution / _subdivisions;
  tiny_vec< size_t, 2 > const last_width( resolution[0] % _subdivisions[0]
                                        , resolution[1] % _subdivisions[1] );

  vector< Tile > pool;
  pool.reserve( _subdivisions[0] * _subdivisions[1] );

  for( size_t i = 0; i < _subdivisions[0]; i++ )
  {
    for( size_t j = 0; j < _subdivisions[1]; j++ )
    {
      tiny_vec< size_t, 2 > origin( width[0] * i, width[1] * j );
      tiny_vec< size_t, 2 > tile_extent = width;

      if( i == _subdivisions[0]-1 ) tile_extent[0] += last_width[0];
      if( j == _subdivisions[1]-1 ) tile_extent[1] += last_width[1];

      
      Tile new_tile = { origin, { tile_extent[0], tile_extent[1] }, _subpixel_sampling, samples_per_pixel, weight };
      
      for( size_t j = 0; j < new_tile.image.h(); ++j )
        for( size_t i = 0; i < new_tile.image.w(); ++i )
          new_tile.image(i,j) = Tile::pixel_format_type( 0.,0.,0. );
        //tile.image.fill( Tile::pixel_format_type( 0.,0.,0. ) );
        
      
      pool.emplace_back( new_tile );
    }
  }

  return pool;
}
