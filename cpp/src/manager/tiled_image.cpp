# include "cg3/manager/tiled_image.hpp"

using namespace std;

TiledImage::TiledImage( vector< Tile > t, tiny_vec< size_t, 2 > r ) 
: resolution( r )
{
  for( Tile & tile : t )
  { 
    tiles.insert( { { tile.origin[0], tile.origin[1] }, tile } );
  }
}

Tile::image_type TiledImage::full_image() const
{
  Tile::image_type full( resolution[0], resolution[1] );
  //full.fill( Tile::pixel_format_type(0.,0.,0.) );

  for( auto const& tp : tiles )
  {
    Tile const& source = tp.second;
    for( size_t j = 0; j < source.image.h(); ++j )
      for( size_t i = 0; i < source.image.w(); ++i )
      {
        full( source.origin[0] + i, source.origin[1] + j ) = source.image( i, j );
      }
  }

  return full;
}

Tile const& TiledImage::compose( Tile const& source )
{
  auto target_it = tiles.find( { source.origin[0], source.origin[1] } );
  if( target_it == tiles.end() ) throw out_of_range( "Target tile not found." );
  Tile& target_tile = target_it->second;

  for( size_t j = 0; j < source.image.h(); ++j )
    for( size_t i = 0; i < source.image.w(); ++i )
    {
      target_tile.image( i, j ) += source.weight * source.image( i, j );
    }

  target_tile.weight += source.weight;
  target_tile.samples_per_pixel += source.samples_per_pixel;
  return target_tile;
}
