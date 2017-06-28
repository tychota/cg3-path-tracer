#ifdef WIN32
# pragma warning (disable: 4996)
#endif

 #include "cg3/common/point_light.hpp"
#include "cg3/common/directional_light.hpp"
#include "cg3/common/spot_light.hpp"
#include "cg3/common/box.hpp"
#include "cg3/common/plane.hpp"
#include "cg3/common/sphere.hpp"
#include "cg3/common/cylinder.hpp"
#include "cg3/common/primitive_group.hpp"
#include "cg3/common/transformation_group.hpp" 
#include "cg3/common/kd_tree.hpp"
#include "cg3/common/triangle.hpp"
#include "cg3/common/blinn_phong.hpp"
#include "cg3/common/phong.hpp"
#include "cg3/common/image_io.hpp"

float rad2deg( float radiants )
{
  return ( radiants * 360. * .5 / M_PI );
}

tiny_vec<float, 3> refracted_direction(float n1, float n2, bool from_inside, bool &total_reflection, tiny_vec<float, 3> N, tiny_vec<float, 3> i)
{

  if (from_inside)
  {
    std::swap(n1, n2);
    N = -N;
  }
  float n = n1 / n2;
  std::cout << n << " :" << N << std::endl;

  float cosI = -dot(i, N);
  float cosT2 = 1. - n * n * (1.f - cosI * cosI);

  std::clog << "I[deg] " << rad2deg( std::acos( cosI ) ) << " T[deg] " << rad2deg( std::acos( sqrt( cosT2 ) ) ) << std::endl;

  total_reflection = cosT2 < 0.;

  if (total_reflection)
  {
    auto reflected = i + 2.f * cosI * N;
    return reflected;
  }

  //return refracted ray
  auto normal_factor = n * cosI - std::sqrt(cosT2);
  auto refracted = n * i + normal_factor * N;
  return refracted;
}

tiny_vec<float, 3> refracted_direction_my(float n1, float n2, bool from_inside, bool &total_reflection, tiny_vec<float, 3> normal, tiny_vec<float, 3> i )
{
  if (from_inside)
  {
    std::swap(n1, n2);
    normal = -normal;
  }
  tiny_vec<float, 3> V = i;
  float n = n1 / n2;
  std::cout << n << " :" << normal << std::endl;
  float cosI = dot(normal, V);
  float sinT2 = n * n * (1.0f - cosI * cosI);
  
  std::clog << "I[deg] " << rad2deg( std::acos( -cosI ) ) << " T[deg] " << rad2deg( std::asin( sqrt( sinT2 ) ) ) << std::endl;
  
  if (sinT2 > 1.0f)//total reflection
  {
    total_reflection = true;
    return V - 2.0f * cosI*normal;//return reflected ray
  }
  else
  {
    total_reflection = false;
    return n * V - (n /*!!! * cosI !!!*/ + sqrt(1.0f - sinT2)) * normal;
  }
}


int main(int argc, char** argv)
{
  tiny_vec<float, 3> const nml(0, 1, 0);
  tiny_vec<float, 3> inc(-1, -1, 0);
  inc.normalize();
  bool const from_inside = false;
  float const n1 = 1.;
  float const n2 = 1.333;

  bool total_reflection;
  tiny_vec<float, 3>  v = refracted_direction( n1, n2, from_inside, total_reflection, nml, inc);

  std::cout << v << " :" << v.length();
  v.normalize();
  std::cout << " [deg] " << rad2deg( std::acos( -dot(v,nml) ) ) << std::endl;
  std::cout << std::boolalpha << total_reflection << std::endl;

  tiny_vec<float, 3>  v2 = refracted_direction_my( n1, n2, from_inside, total_reflection, nml, inc);

  std::cout << v2 << ": " << v2.length();
  v2.normalize();
  std::cout << " [deg] " << rad2deg( std::acos( -dot(v2,nml) ) ) << std::endl;
  std::cout << std::boolalpha << total_reflection << std::endl;
}
