# include "cg3/common/directional_light.hpp"
# include <limits>

using namespace std;

directional_light::directional_light()
{
  set_direction( -1., -1., -1. );
}

directional_light::directional_light( float dx, float dy, float dz )
{
  set_direction( dx, dy, dz );
}

directional_light::directional_light( tiny_vec<float,3> const& dir )
{
  set_direction( dir );
}

void directional_light::set_direction( float dx, float dy, float dz )
{
  _direction[0] = dx;
  _direction[1] = dy;
  _direction[2] = dz;
  _direction.normalize();
}

void directional_light::set_direction( tiny_vec<float,3> const& dir)
{
  _direction = dir;
  _direction.normalize();
}

void directional_light::sample( intersection_info const& scene_sample
, position<3>&  light_sample
, direction<3>& light_direction
, si::length&   light_distance
, probability&  light_sample_probability  
) const
{
  light_sample = { scene_sample.location() - _direction * numeric_limits< si::length >::max() };
  light_direction = - _direction;
  light_distance = numeric_limits<si::length>::infinity();

  light_sample_probability = 1.;
}

radiance_rgb directional_light::emitted_radiance( position< 3 > light_pos
                                                , direction< 3 > dir
                                                , si::length dist ) const
{
  auto geometry_factor = dot( dir, _direction );
  if( geometry_factor < 0 ) return {0.f,0.f,0.f};

  // TODO: calculation of emitted_radiance wrt _total_power is INCORRECT
  return _total_power; // / area();
}



