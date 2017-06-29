# include "cg3/common/phong.hpp"
# include "cg3/common/utils.hpp"

using namespace std;

tiny_vec< real_type, 3 > phong::brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& hit ) const
{
  if( dot( hit.normal(), in ) > 0 ) return {0.,0.,0.};

  tiny_vec< real_type, 3 > result = diffuse /*/ M_PI*/;

  auto dot_LR = - dot( hit.reflected_incoming_direction(), in );
  if( dot_LR < 0. ) result += specular * pow( dot_LR, shininess );

  return result;
}

tiny_vec< real_type, 3 > phong::diffuse_brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& hit ) const
{
  return diffuse;
}

tiny_vec< real_type, 3 > phong::specular_brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& hit ) const
{
  real_type cosLR = clamp< real_type >( - dot( in, hit.reflected_incoming_direction() ), 0., 1. );
  return specular * pow( cosLR, shininess );
}

