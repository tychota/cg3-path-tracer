#include "cg3/common/box.hpp"
#include <limits>

using namespace std;

box::box() 
{
  clear(); 
}

box::box( tiny_vec<float,3> const& minimum, const tiny_vec<float,3>& maximum) 
: minimum( minimum )
, maximum( maximum ) 
{}

box::~box()
{}


pair< tiny_vec< float, 3 >, tiny_vec< float, 3 > > box::calc_bounds() const
{
  return make_pair( minimum, maximum );
}

void box::extend( tiny_vec<float,3> const& a )
{
  for( int i = 0; i < 3; i++ )
  {
    if( minimum( i ) > a( i ) ) minimum( i ) = a( i ); 
    else minimum( i ) = minimum( i );

    if( maximum( i ) < a( i ) ) maximum( i ) = a( i ); 
    else maximum( i ) = maximum( i );
  }
}

void box::extend( pair<tiny_vec<float,3>,tiny_vec<float,3> > const& minmax )
{
  extend( minmax.first );
  extend( minmax.second );
}

void box::extend( box const& b )
{
  extend( b.minimum );
  extend( b.maximum );
}

void box::clear()
{ 
  minimum = tiny_vec<float,3>(  numeric_limits<float>::infinity(),  numeric_limits<float>::infinity(),  numeric_limits<float>::infinity() );
  maximum = tiny_vec<float,3>( -numeric_limits<float>::infinity(), -numeric_limits<float>::infinity(), -numeric_limits<float>::infinity() );
}


position< 3 >& box::get_min()
{
  return minimum;
}

position< 3 > const& box::get_min() const
{
    return minimum;
}

position< 3 >& box::get_max()
{
  return maximum;
}

position< 3 > const& box::get_max() const
{
  return maximum;
}

tiny_vec<float,3> box::get_extends() const
{
  return 0.5f * ( maximum - minimum );
}

position< 3 > box::get_center() const
{
  return get_extends() + get_min();
}

pair< si::length, si::length > box::intersect( ray const& r ) const
{
  si::length entry, exit;
  position< 3 > const orig = r.origin();
  direction< 3 > const dir = r.dir();
  direction< 3 > const inv_dir = r.inv_dir();

  // x axis
  si::length xmin = ( minimum[0] - orig[0] ) * inv_dir[0];
  si::length xmax = ( maximum[0] - orig[0] ) * inv_dir[0];
  if( xmin > xmax ) swap(xmin,xmax);

  // y axis
  si::length ymin = ( minimum[1] - orig[1] ) * inv_dir[1];
  si::length ymax = ( maximum[1] - orig[1] ) * inv_dir[1];
  if( ymin > ymax ) swap(ymin,ymax);

  if( xmin > ymax || ymin > xmax) 
    return make_pair( numeric_limits< si::length >::infinity(), - numeric_limits< si::length >::infinity() );

  entry = max(xmin,ymin);
  exit  = min(xmax,ymax);

  // z axis
  si::length zmin = ( minimum[2] - orig[2] ) * inv_dir[2];
  si::length zmax = ( maximum[2] - orig[2] ) * inv_dir[2];
  if( zmin > zmax ) swap( zmin, zmax );

  if( entry > zmax || zmin > exit ) 
    return make_pair( numeric_limits< si::length >::infinity(), - numeric_limits< si::length >::infinity() );

  entry = max( entry,zmin );
  exit  = min( exit, zmax );

  return make_pair( entry, exit );
}

void box::calc_normal( intersection_info* hit ) const
{
  position< 3 > ex1 = hit->location() - get_min();
  position< 3 > ex2 = hit->location() - get_max();

  int idx = -1;
  si::length d = numeric_limits< si::length >::infinity();
  float s = 0;
  for( int i = 0; i < 3; i++ )
  {
    if( fabs(ex1[i]) < d )
    {
      idx = i;
      d = fabs(ex1[i]);
      s = -1.0f;
    }

    if( fabs(ex2[i]) < d )
    {
      idx = i;
      d = fabs(ex2[i]);
      s = 1.0f;
    }
  }
  tiny_vec<float,3> nml( 0,0,0 );
  nml( idx ) = s;

  hit->set_normal( nml );
}

bool box::closest_intersection( intersection_info* hit, si::length min_lambda ) const
{
  pair< si::length, si::length > intersections = intersect( hit->incoming_ray() );

  if( intersections.first > min_lambda && intersections.first < hit->lambda() )
  {
    hit->set_lambda( intersections.first );
    hit->set_object( this );	

    calc_normal( hit );
    return true;
  }
  if( intersections.second > min_lambda && intersections.second < hit->lambda() )
  {
    hit->set_lambda( intersections.second );
    hit->set_object( this );	
    calc_normal( hit );
    return true;
  }
  return false;
}

bool box::any_intersection( ray& r, si::length min_lambda, si::length max_lambda ) const
{
  pair< si::length, si::length > intersections = intersect( r );

  if( intersections.first > min_lambda && intersections.first < max_lambda )
    return true;

  if( intersections.second > min_lambda && intersections.second < max_lambda )
    return true;

  return false;
}
