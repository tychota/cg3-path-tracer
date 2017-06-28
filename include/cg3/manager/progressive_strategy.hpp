# pragma once

# include "cg3/manager/strategy.hpp"

class ProgressiveStrategy : public Strategy
{

  public:
    ProgressiveStrategy( tiny_vec< std::size_t, 2 > subdivisions
                       , std::size_t                samples_per_pixel
                       , Tile::sampling_type        subsampling = Tile::SAMPLE_RANDOM
                       )
    : Strategy( subdivisions, samples_per_pixel, subsampling )
    {}

    virtual std::vector< Tile > create_render_tiles( tiny_vec< std::size_t, 2 > resolution ) override;
};
