# pragma once

# include "cg3/common/utils.hpp"
# include "cg3/common/mat.hpp"

struct Tile
{
  typedef tiny_vec< real_type, 3 > pixel_format_type;
  typedef mat< pixel_format_type > image_type;

  enum sampling_type
  {
    SAMPLE_REGULAR,
    SAMPLE_RANDOM,
  };

  position<2> get_subpixel_sample( std::size_t sample_index )
  {
    if( sample_generator == SAMPLE_REGULAR )
    {
      // TODO
      if( samples_per_pixel > 1 ) throw;
      return { real_type(.5), real_type(.5) };
    }
    else if( sample_generator == SAMPLE_RANDOM )
    {
      auto u = uniform_random< si::length >( 0, 1 );
      auto v = uniform_random< si::length >( 0, 1 );
      return { u, v };
    }
    throw;
  }

  tiny_vec< std::size_t, 2 > origin;
  image_type                 image;
  sampling_type              sample_generator;
  std::size_t                samples_per_pixel;
  real_type                  weight;
};

