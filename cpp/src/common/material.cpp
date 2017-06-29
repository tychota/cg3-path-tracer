#include "cg3/common/material.hpp"
#include "cg3/common/utils.hpp"

using namespace std;

Material::Material()
: diffuse( .8,.8,0.8 )
, specular( .4,0.4,0.4 )
, shininess( 40 )
, reflection( 0. )
, refraction( 0. )
, refrac_index( 1.1 )
{}

void Material::set_diffuse( tiny_vec< real_type,3>& col )
{ 
  diffuse = col;
}

void Material::set_diffuse( real_type r, real_type g, real_type b) 
{
  diffuse[0] = r;
  diffuse[1] = g;
  diffuse[2] = b;
}

tiny_vec< real_type,3 >& Material::get_diffuse()
{ 
  return diffuse; 
}

const tiny_vec< real_type,3 >& Material::get_diffuse() const
{
  return diffuse;
}

void Material::set_specular( tiny_vec< real_type, 3 >& col )
{
  specular = col; 
}

void Material::set_specular( real_type r, real_type g, real_type b)
{
  specular[0] = r;
  specular[1] = g;
  specular[2] = b;
}
tiny_vec< real_type, 3 >& Material::get_specular() 
{ 
  return specular;
}

const tiny_vec< real_type, 3 >& Material::get_specular() const
{ 
  return specular; 
}

void Material::set_shininess( real_type s )
{
  shininess = s;
}

float Material::get_shininess() const
{
  return shininess;
}

void Material::set_reflectivity( real_type refl ) 
{
  reflection = refl; 
}

float Material::get_reflectivity() const
{
  return reflection; 
}

tiny_vec< real_type ,3 > Material::brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& hit ) const
{
  if( dot( hit.normal(), in ) > 0) return { 0.,0.,0. };
    
  tiny_vec< real_type, 3 > const reflected_diffuse = diffuse_brdf( in, out, hit );

  tiny_vec< real_type, 3 > const reflected_specular = specular_brdf( in, out, hit );

  tiny_vec< real_type, 3 > const result = reflected_diffuse + reflected_specular;

  if( result.min_elem() < 0. ) 
    throw out_of_range( "Negative reflectivity" );
   if( result.max_elem() > ( 1. + 1e-3 ) ) 
     throw out_of_range( "Reflectivity too large." );

  return result;
}

void Material::set_refractivity(float refrac)
{
  refraction = refrac; 
}

float Material::get_refractivity()const 
{
  return refraction;
}

void Material::set_index_of_refraction( float r )
{
  refrac_index = r;
}

float Material::get_index_of_refraction() const 
{
  return refrac_index;
}
