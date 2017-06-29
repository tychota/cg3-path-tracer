# pragma once

# include "cg3/manager/tile.hpp"

class Strategy
{
  public:
    Strategy( tiny_vec< std::size_t, 2 > subdivisions
            , std::size_t                samples_per_pixel
            , Tile::sampling_type        subsampling = Tile::SAMPLE_RANDOM
            )
    : _subdivisions( subdivisions )
    , _samples_per_pixel( samples_per_pixel )
    , _subpixel_sampling( subsampling )
    {}

    virtual std::vector< Tile > create_render_tiles( tiny_vec< std::size_t, 2 > resolution );

    virtual std::vector< Tile > create_target_tiles( tiny_vec< std::size_t, 2 > resolution );

    virtual std::vector< Tile > create_tile_field( tiny_vec< std::size_t, 2 > resolution, size_t const samples_per_pixel, real_type const weight );

  protected:
    tiny_vec< std::size_t, 2 > _subdivisions;
    std::size_t _samples_per_pixel;
    Tile::sampling_type _subpixel_sampling;
};
