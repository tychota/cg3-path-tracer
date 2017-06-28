#include "cg3/common/point_light.hpp"

using namespace std;

point_light::point_light() : _position(0,10,0)
{
}

point_light::point_light( si::length px, si::length py, si::length pz ) : _position( px, py, pz )
{
}

void point_light::set_position( si::length px, si::length py, si::length pz )
{
  _position[0] = px;
  _position[1] = py;
  _position[2] = pz;
}
void point_light::set_position( position<3> const& pos )
{
  _position = pos;
}

void point_light::sample( intersection_info const& scene_sample
                        , position<3>&  light_sample
                        , direction<3>& light_direction
                        , si::length&   light_distance
                        , probability&  light_sample_probability  
                        ) const
{
  light_sample = _position;
  light_direction = _position - scene_sample.location();
  light_distance = light_direction.length();
  light_direction /= light_distance;

  light_sample_probability = 1.;
}

radiance_rgb point_light::emitted_radiance( position< 3 > light_pos, direction< 3 > dir, si::length dist ) const
{
  return _total_power / ( 4 * M_PI * dist * dist );
}
