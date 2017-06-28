#pragma once

# include "cg3/manager/tile.hpp"

# include <map>

class TiledImage
{
  public:
    tiny_vec< std::size_t, 2 > const resolution;

    TiledImage( std::vector< Tile > t, tiny_vec< std::size_t, 2 > r );

    Tile::image_type full_image() const;

    Tile const& compose( Tile const& source );
  private:
    std::map< std::pair< std::size_t, std::size_t >, Tile > tiles;
};