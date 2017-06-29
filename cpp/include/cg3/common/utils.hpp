# pragma once

# include "cg3/common/tiny_vec.hpp"
# include "cg3/common/si_units.hpp"

# include <algorithm>

using namespace std;

template <typename T>
T clamp(T v,T lo, T hi)
{
	return max( min( hi, v ), lo );
}

//find the real roots of ax^2 +bx+c = 0
// the number of real roots is returned
template <typename T>
int solve_real_quadratic(const T& a, const T& b, const T& c, T x[2]) 
{
  if(a == 0)
  {
      if(b == 0)
          return 0;
      else
      {
          x[0] = -c/b;
          return 1;
      }
  }
  T d = b*b -  4*a*c;

  if(d >= 0)
  {
      T q = sqrt(d)/(2 *a);
      if(q == 0)
      {
          x[0] = (-b/(2 * a)) ;
          return 1;
      }
      else
      {
          x[0] = (-b/(2 * a)) + q;
          x[1] = (-b/(2 * a)) - q;
          return 2;
      }
  }
  return 0;
} 


template< typename T >
T uniform_random( T const& min = T(0), T const& max = T(1) )
{
  static std::default_random_engine generator;
  std::uniform_real_distribution< T > distribution( min, max );
  return distribution( generator );
}
