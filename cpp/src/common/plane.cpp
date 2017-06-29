#include "cg3/common/plane.hpp"





plane::plane():normal(0.0f,1.0f,0.0f),distance(0){}

plane::plane(const tiny_vec<float,3>& nml, float d)
{
	normal=nml;
	normal.normalize();
	distance=d; 
}


bool plane::closest_intersection(intersection_info* hit, float min_lambda ) const
{
  real_type const nd = dot( normal, hit->incoming_ray().dir() );

  if( nd == 0 )
    return false;

  si::length const lambda = ( distance - dot( normal, hit->incoming_ray().origin() ) ) / nd;
  if(lambda > min_lambda && lambda < hit->lambda() )
  {
    hit->set_lambda(lambda);
    hit->set_object(this);

    if( nd < 0)
      hit->set_normal( normal );
    else
      hit->set_normal( -normal );

    return true;
  }
  return false;
}

bool plane::any_intersection( ray& r, si::length min_lambda, si::length max_lambda  ) const
{
  real_type nd = dot( normal, r.dir() );
  if( nd == 0 )
    return false;

  si::length const lambda =( distance - dot( normal, r.origin() ) ) / nd;
  return lambda > min_lambda && lambda < max_lambda;
}
