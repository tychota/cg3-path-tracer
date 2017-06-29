#include "cg3/common/spot_light.hpp"

using namespace std;

spot_light::spot_light()
{
  set_position( 10., 10., 10. );
  set_spot_direction( -1., -1., -1. );
  set_spot_exponent( 80. );
  set_total_power( 0.8, 0.8, 0.8 );
  set_spot_cutoff( 30.0 );
}

void spot_light::set_spot_direction( real_type dx, real_type dy, real_type dz )
{
  _spot_direction[0] = dx;
  _spot_direction[1] = dy;
  _spot_direction[2] = dz;
  _spot_direction.normalize();
}
void spot_light::set_direction( direction< 3 > const& dir )
{
  _spot_direction = dir;
  _spot_direction.normalize();
}
void spot_light::set_position( si::length px, si::length py, si::length pz )
{
  _position[0] = px;
  _position[1] = py;
  _position[2] = pz;
}
void spot_light::set_position( position<3 > const& pos)
{
  _position = pos;
}

void spot_light::set_spot_exponent( real_type exponent )
{
  _spot_exponent = exponent;
}

float spot_light::get_spot_exponent() const
{
  return _spot_exponent;
}

void spot_light::set_spot_cutoff( degree_type co)
{
  _spot_cos_cutoff = cos( co * M_PI / 180 );
}

// void spot_light::fill_shader_info( shader_info& i ) const
// {
//   i.light = this;
//   auto diff_vec = _position - i.intersection->location();
//   i.light_distance  = diff_vec.length();
//   i.light_direction = diff_vec / i.light_distance;
// 
//   float att = 1 /  i.light_distance * i.light_distance;
// 
//   float spotdot = dot( - i.light_direction, _spot_direction );
//   if( spotdot >= _spot_cos_cutoff )
//     att *= pow( spotdot, _spot_exponent );
//   else
//     att = 0;
// 
//   i.light_attentuation = { att, att, att };
// }

void spot_light::sample( intersection_info const& scene_sample
                       , position<3>&  light_sample
                       , direction<3>& light_direction
                       , si::length&   light_distance
                       , probability&  light_sample_probability  
                       ) const
{
  
  light_sample = _position;
  light_direction = light_sample - scene_sample.location();
  light_distance = light_direction.length();
  light_direction /= light_distance;

  light_sample_probability = 1.;
}

radiance_rgb spot_light::emitted_radiance( position< 3 > light_pos, direction< 3 > dir, si::length dist ) const
{
  real_type attentuation = 1 / ( dist * dist );

  real_type spotdot = dot( - dir, _spot_direction );
  if( spotdot >= _spot_cos_cutoff )
    attentuation *= pow( spotdot, _spot_exponent );
  else
    attentuation = 0;

  // TODO: calculation of emitted_radiance wrt _total_power is INCORRECT
  return _total_power / ( 4 * M_PI * dist * dist );
}
