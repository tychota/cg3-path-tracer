#include "cg3/common/sphere.hpp"

using namespace std;

sphere::sphere():radius(1.0f),sqr_radius(1.0f)
{
  center.zeros();
}

sphere::sphere( si::length cx, si::length cy, si::length cz, si::length r )
: center( cx, cy, cz ), radius( r ), sqr_radius( r * r )
{}

sphere::sphere( position< 3 > const& c, si::length r)
: center( c ), radius( r ), sqr_radius( r * r )
{}

void sphere::set_radius( si::length r )
{
  radius = r;
  sqr_radius = r * r;
}

void sphere::calc_normal( intersection_info* hit )const 
{
  direction< 3 > nml = hit->location() - center;
  nml.normalize();

  hit->set_normal( nml );
}

pair<tiny_vec<float,3>, tiny_vec<float,3> > sphere::calc_bounds() const
{
  return make_pair( center - radius, center + radius );
}

bool sphere::closest_intersection( intersection_info* hit, si::length min_lambda ) const
{
  tiny_vec<si::length,3> dst =  hit->incoming_ray().origin() - center;

  auto const B = dot( dst, hit->incoming_ray().dir());
  auto const C = dot( dst, dst) - sqr_radius;
  auto const D = B*B - C;
  if( D > 0)
  {
    auto const sqrtD = sqrt(D);
    auto const lambda1 = -B - sqrtD;
    if( lambda1 > min_lambda && lambda1 < hit->lambda() )
    {
      hit->set_lambda( lambda1 );
      hit->set_object( this );
      calc_normal( hit );
      return true;
    }
    auto const lambda2 = -B + sqrtD;
    if( lambda2 > min_lambda && lambda2 < hit->lambda() )
    {
      hit->set_lambda( lambda2 );
      hit->set_object( this );
      calc_normal(hit);
      return true;
    }
  }
  return false;
}

bool sphere::any_intersection( ray& r, si::length min_lambda, si::length max_lambda ) const
{
  tiny_vec< si::length, 3 > dst =  r.origin() - center;

  auto const B = dot( dst, r.dir());
  auto const C = dot( dst, dst) - sqr_radius;

  auto const D = B*B - C;
  if( D > 0 )
  {
    auto const sqrtD = sqrt(D);
    auto const lambda1 = -B - sqrtD;
    if( lambda1 > min_lambda && lambda1 < max_lambda ) return true;

    auto const lambda2 = -B + sqrtD;
    if( lambda2 > min_lambda && lambda2 < max_lambda ) return true;
  }
  return false;
}
