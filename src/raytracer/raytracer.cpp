# include "cg3/raytracer/raytracer.hpp"

# include "cg3/common/utils.hpp"

# include <iomanip>
# include <limits>

using namespace std;

radiance_rgb Raytracer::trace( intersection_info& hit, size_t recursion_depth )
{

  if( _scene->root()->closest_intersection( &hit, si::length(1e-3) ) ) //if intersection is found...
  {
    bool const from_inside = false; //hit.geometry_factor() > 0;
    radiance_rgb result(0.,0.,0.);

    if( ! from_inside )
    {
      result += direct_reflected_radiance( hit );
    }

    if( recursion_depth < _max_recursion_depth )
    {
      real_type refrac = hit.object()->material()->get_refractivity();
      if(refrac > 0)
      {
        real_type n1 = _scene->get_index_of_refraction();
        result += sample_fresnel_surface( _scene->get_index_of_refraction(), from_inside, hit, recursion_depth );
      }
    }
    return result;
  } 
  else
    return _scene->background_radiance();
}

radiance_rgb Raytracer::direct_reflected_radiance( intersection_info const& hit )
{
  radiance_rgb result( 0.,0.,0. );

  for( auto const& al : _scene->lights() )
  {
    position<3>  l_pos; // position sample on light source
    direction<3> l_dir; // direction to sample on light source
    si::length   l_dist;// distance to light source
    probability  l_prob;// probability of choosing position sample
    al->sample( hit, l_pos, l_dir, l_dist, l_prob );

    ray shadow_ray( hit.location(), l_dir );
    if( ! _scene->root()->any_intersection( shadow_ray, si::length(1e-3), l_dist ) )
    {
      auto const brdf = hit.object()->material()->brdf( - l_dir, hit.direction_to_camera(), hit );
      auto const geometry_factor = clamp< real_type >( dot( hit.normal(), l_dir ), 0., 1. );
      auto const incoming_radiance = al->emitted_radiance( l_pos, - l_dir, l_dist );

      result += brdf * geometry_factor * incoming_radiance / l_prob;
    }
  }
  return result;
}

radiance_rgb Raytracer::sample_fresnel_surface( real_type n_env, bool from_inside, intersection_info const& hit, size_t recursion_depth )
{
  // incident direction (normalized).
  direction< 3 > const& i =  hit.incoming_ray().dir();
  // unit normal at hit point.
  direction< 3 > N = hit.normal();

  si::length cosI = - hit.geometry_factor();
  real_type  n1 = n_env;
  real_type  n2 = hit.object()->material()->get_index_of_refraction();
  if( cosI < 0 )
  {
    N = - N;
    cosI = - cosI;
    swap( n1, n2 );
  }

  real_type const n = n1 / n2;
  real_type const cosT2 = 1.f - n * n * ( 1.f - cosI * cosI );

  real_type cosT;
  real_type reflectance = 1.f;
  bool const total_reflection = cosT2 < 0.;
  if( ! total_reflection )
  { 
    cosT = sqrt( cosT2 );
    // calculate reflectance
    real_type const r_ortho = ( n1 * cosI - n2 * cosT ) / ( n1 * cosI + n2 * cosT );
    real_type const r_par   = ( n2 * cosI - n1 * cosT ) / ( n2 * cosI + n1 * cosT );
    reflectance = .5 * ( r_ortho * r_ortho + r_par * r_par );
  }

  position< 3 > const& hit_location = hit.location();
  radiance_rgb flux( 0.,0.,0. );

  if( reflectance != 0.f )
  {
    auto reflected = hit.reflected_incoming_direction();

    intersection_info reflect_intersection( ray( hit_location, reflected ) );
    flux += reflectance * trace( reflect_intersection, recursion_depth+1 );
  }

  if( reflectance != 1.f  )
  {
    //return refracted ray
    real_type const normal_factor = ( n * cosI - cosT );
    direction< 3 > refracted =  n * i + normal_factor * N;

    intersection_info refract_intersection( ray( hit_location, refracted ) );
    radiance_rgb refracted_color = trace( refract_intersection, recursion_depth+1 );
    flux += ( 1 - reflectance ) * refracted_color;
  }

  return flux;
}
