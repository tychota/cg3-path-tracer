# pragma once

# include "tiny_vec.hpp"

typedef float real_type;

namespace si
{
  // base quantities
  typedef real_type length;
  typedef real_type time;
  typedef real_type mass;

  // derived units
  typedef real_type area;
  typedef real_type radian;
  typedef real_type sterradian;
  typedef real_type power; // W
  typedef real_type radiance; // W
}

typedef real_type degree_type;
typedef real_type probability;

// multivariate quantities
template< std::size_t Dim, typename Unit = si::length >
using position = tiny_vec<  Unit, Dim >;

template< std::size_t Dim, typename Unit = real_type >
using direction = tiny_vec< Unit, Dim >;

using radiance_rgb = tiny_vec< si::radiance, 3 >;
