
#include "cg3/common/intersection_info.hpp"
#include "cg3/common/primitive.hpp"

#include <limits>

using namespace std;

si::length intersection_info::lambda() const 
{
  return _lambda;
}

void intersection_info::set_lambda( si::length l)
{
  _lambda = l;
  _location = incoming_ray().eval_at( l );
}

void intersection_info::set_incoming_ray( ray const& r )
{
  _incoming_ray = r;
  _location = incoming_ray().eval_at( lambda() );
}

position< 3 > const& intersection_info::location() const
{
  return _location;
}

direction<3> const& intersection_info::normal() const
{
  return _normal;
}

void intersection_info::set_normal( direction< 3 > const& n )
{
  _normal = n;
  _geometry_factor = dot( _normal, _incoming_ray.dir() );
}

real_type intersection_info::geometry_factor() const
{
  return _geometry_factor;
}

ray const& intersection_info::incoming_ray() const
{
  return _incoming_ray;
}

direction< 3 > intersection_info::direction_to_camera() const 
{
  return -1 * incoming_ray().dir();
}

direction< 3 > intersection_info::reflected_incoming_direction() const
{
  real_type cosI = - geometry_factor();
  return incoming_ray().dir() + 2.f * cosI * normal();
}


primitive const* intersection_info::object() const 
{
  return _object;
}

void intersection_info::set_object( primitive const* obj ) 
{
  _object = obj;
}

tiny_vec< real_type, 3 > const& intersection_info::potential() const
{
  return _potential;
}

void intersection_info::set_potential( tiny_vec< real_type,3 > const& p )
{
  _potential = p;
}


intersection_info::intersection_info()
: _lambda( numeric_limits< si::length >::infinity() )
, _object( nullptr )
{}

intersection_info::intersection_info( ray r, si::length l) 
: _incoming_ray( r )
, _lambda( l )
, _object( nullptr )
{}