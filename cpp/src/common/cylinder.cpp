#include "cg3/common/cylinder.hpp"
#include "cg3/common/utils.hpp"

using namespace std;

cylinder::cylinder():bottom(0,0,0),top(0,1,0),finite(true),caps(true),radius(0.25)
{
}
	
cylinder::cylinder(const tiny_vec<float,3>& p1, const tiny_vec<float,3>& p2,const float r)
{
	bottom = p1;
	top = p2;
	radius = r;
	finite=true;
	caps=true;
}

void cylinder::calc_normal( intersection_info* hit ) const
{
  position< 3 > l = hit->location();
  position< 3 > p = l-top;
  position< 3 > q = l-bottom;
  tiny_vec<float,3> Va = (top-bottom);
  Va.normalize();

  if(caps)
  {
    if( p.sqr_length() < radius*radius && fabs( dot( p, Va ) ) < 0.001)
    { 
      hit->set_normal( Va );
      return ;
    }
    else if( q.sqr_length() < radius*radius && fabs( dot( q, Va ) ) < 0.001 )
    { 
      hit->set_normal( - Va );
      return ;
    }
  }

  direction< 3 > nml;
  nml = l - dot( q, Va ) * Va;
  nml.normalize();

//# warning not capable of handling rays from "inside".
//   if( dot( nml, hit->incoming_ray().direction() ) < 0 )
  hit->set_normal(nml);
//   else
//       hit->set_normal (-nml);

}

bool cylinder::closest_intersection(intersection_info* hit, si::length min_lambda ) const
{
  direction< 3 > const V = hit->incoming_ray().dir();
  position< 3 > const P = hit->incoming_ray().origin();
  tiny_vec<float,3> Va = top-bottom;
  Va.normalize();
  tiny_vec<float,3> const dp = P - bottom;
  tiny_vec<float,3> const Avec =V-dot(V,Va)*Va;
  tiny_vec<float,3> const Cvec = dp - dot(dp,Va)*Va;
  float const A = Avec.sqr_length();
  float const B = 2*dot(Avec,Cvec);
  float const C = Cvec.sqr_length() - radius*radius;
  float ts[4];
  int n=0;

  if(caps)
  {
    ts[n]=dot(Va,bottom-P)/dot(Va,V);
    tiny_vec<float,3> Q = P + ts[n] * V;
    if( (Q-bottom).sqr_length() < radius*radius)
        n++;

    ts[n]=dot(Va,top-P)/dot(Va,V);
    Q = P + ts[n] * V;
    if( (Q-top).sqr_length() < radius*radius)
        n++;
  }

  int const nroots = solve_real_quadratic(A, B, C, &(ts[n]));

  if(finite)
  {
    if(nroots > 0)
    {
      tiny_vec<float,3> Q = P + ts[n] * V;
      if ( dot(Q-bottom,Va) > 0 && dot(Q-top,Va) < 0)
          n++;
      else
          ts[n] =ts[n+1];
      if(nroots > 1)
      {
          tiny_vec<float,3> Q = P + ts[n] * V;
          if ( dot(Q-bottom,Va) > 0 && dot(Q-top,Va) < 0)
              n++;
      }
    }
  }

  bool ret = false;
  for(int i = 0; i < n;i++)
  {
    if(ts[i] > min_lambda && ts[i] < hit->lambda())
    {
        hit->set_lambda(  ts[i]);
        hit->set_object ( this);
        calc_normal(hit);
        ret = true;
    }
  }

  return ret;
}

bool cylinder::any_intersection( ray & r, si::length min_lambda, si::length max_lambda ) const
{
  direction<3> V = r.dir();
  position<3> P = r.origin();
  direction<3> Va = top-bottom;
  Va.normalize();
  auto dp = P - bottom;
  auto Avec =V-dot(V,Va)*Va;
  auto Cvec = dp - dot(dp,Va)*Va;
  auto A = Avec.sqr_length();
  auto B = 2*dot(Avec,Cvec);
  auto C = Cvec.sqr_length() - radius*radius;
  si::length ts[4];
  int n=0;

  if(caps)
  {
    ts[n]=dot(Va,bottom-P)/dot(Va,V);
    tiny_vec<float,3> Q = P + ts[n] * V;
    if( (Q-bottom).sqr_length() < radius*radius)
      n++;

    ts[n]=dot(Va,top-P)/dot(Va,V);
    Q = P + ts[n] * V;
    if( (Q-top).sqr_length() < radius*radius)
      n++;
  }

  int nroots= solve_real_quadratic(A, B, C, &(ts[n]));

  if(finite)
  {
    if(nroots > 0)
    {
      tiny_vec<float,3> Q = P + ts[n] * V;
      if ( dot(Q-bottom,Va) > 0 && dot(Q-top,Va) < 0)
          n++;
      else
          ts[n] =ts[n+1];
      if(nroots > 1)
      {
          tiny_vec<float,3> Q = P + ts[n] * V;
          if ( dot(Q-bottom,Va) > 0 && dot(Q-top,Va) < 0)
              n++;
      }
    }
  }

  for(int i = 0; i < n;i++)
  {
    if(ts[i] > min_lambda && ts[i] < max_lambda)
      return true;
  }

  return false;
}

void cylinder::set_finite(bool b)
{
	finite = b;
}

bool cylinder::is_finite() const
{
	return finite;
}

void cylinder::set_caps(bool b)
{
	caps = b;
	if(caps)
		finite=true;
}

bool cylinder::has_caps() const
{
	return caps;
}

