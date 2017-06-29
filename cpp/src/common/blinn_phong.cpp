# include "cg3/common/blinn_phong.hpp"
# include "cg3/common/utils.hpp"

using namespace std;

tiny_vec< real_type, 3 > blinn_phong::brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& hit ) const
{
  if( dot( hit.normal(), in ) > 0 ) return {0.,0.,0.};

  auto result = diffuse /*/ M_PI*/;

  direction< 3 > H = out - in;
  H.normalize();
  real_type dot_NH = clamp< real_type >( dot( hit.normal(), H ), 0., 1. );

  result += specular * pow( dot_NH, shininess );

  return result;
}

tiny_vec< real_type, 3 > blinn_phong::specular_brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& hit ) const
{
  direction< 3 > H = out - in;
  H.normalize();
  real_type cosNH = clamp< real_type >( dot( hit.normal(), H ), 0., 1. );

  return specular * pow( cosNH, shininess );
}



