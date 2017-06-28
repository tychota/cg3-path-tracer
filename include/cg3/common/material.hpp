#pragma once
#include "cg3/common/tiny_vec.hpp"
#include "cg3/common/intersection_info.hpp"
#include "cg3/common/light_source.hpp"

class Material
{
  public:
    Material();

    void set_diffuse( tiny_vec<float,3>& col ) ;
    void set_diffuse( float r, float g, float b);
    tiny_vec<float,3>& get_diffuse();
    const tiny_vec<float,3>& get_diffuse() const;

    void set_specular( tiny_vec<float,3>& col ) ;
    void set_specular( float r, float g, float b );

    tiny_vec<float,3> &      get_specular() ;
    tiny_vec<float,3> const& get_specular() const;

    void  set_shininess( float );
    float get_shininess() const;

    void  set_reflectivity( float );
    float get_reflectivity() const ;

    void  set_refractivity( float ) ;
    float get_refractivity() const ;

    void  set_index_of_refraction( float );
    float get_index_of_refraction() const;

    virtual tiny_vec< real_type, 3 > brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& ) const;

  protected:

    virtual tiny_vec< real_type, 3 >  diffuse_brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& ) const = 0;
    virtual tiny_vec< real_type, 3 > specular_brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& ) const = 0;

    tiny_vec<float,3> diffuse;
    tiny_vec<float,3> specular;
    float shininess;

    float reflection;
    float refraction;

    float refrac_index;
};







