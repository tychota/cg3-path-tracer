# include "cg3/common/area_light.hpp"
# include "cg3/common/utils.hpp"

using namespace std;

area_light::area_light( position<3> const& pos
                      , direction<3> const& span_x
                      , direction<3> const& span_y
                      , tiny_vec< si::power, 3 > const& total_power 
                      )
: light_source( total_power )
, _pos( pos ), _span_x( span_x ), _span_y( span_y )
, _normal( cross( _span_x, _span_y ) / _span_x.length() / _span_y.length() )
{
}

void area_light::sample( intersection_info const& scene_sample
                       , position<3>&  light_sample
                       , direction<3>& light_direction
                       , si::length&   light_distance
                       , probability&  light_sample_probability  
                       ) const
{
  // student begin

  // student end
}

radiance_rgb area_light::emitted_radiance( position< 3 > light_pos, direction< 3 > dir, si::length dist ) const
{
  // student begin

  // student end
}

