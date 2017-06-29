# include "cg3/common/primitive_group.hpp"


primitive_group::~primitive_group()
{
  clear();
}

bool primitive_group::closest_intersection( intersection_info* hit, si::length min_lambda ) const
{
  bool intersect=false;
  for( auto p : primitives ) //find closest intersection 
  {
    if( p->closest_intersection( hit, min_lambda) )
      intersect = true;
  }
  return intersect;
}

bool primitive_group::any_intersection( ray & r, si::length min_lambda, si::length max_lambda ) const
{
  for( auto p : primitives ) 
  {
    if( p->any_intersection( r, min_lambda, max_lambda ) )
      return true;
  }
  return false;
}

void primitive_group::add_primitive( primitive* p )
{
  primitives.push_back(p);
}

void primitive_group::clear()
{
  for( auto* p : primitives )
    delete p;

  primitives.clear();
}