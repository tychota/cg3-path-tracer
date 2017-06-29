# define _USE_MATH_DEFINES
# include <cmath>


# include "cg3/pathtracer/pathtracer.hpp"
# include "cg3/common/utils.hpp"

# include <iomanip>


using namespace std;

struct IndirectTerminator
{
  virtual bool operator() () const = 0;
  virtual real_type survivor_weight() const = 0;
  virtual size_t num_rays() const = 0;
};

struct RussianRoullete : public IndirectTerminator
{
  real_type const continuation_probability;
  real_type const weight;

  RussianRoullete( real_type const& cp )
  : continuation_probability( cp )
  , weight( 1 / continuation_probability )
  {
    if( continuation_probability > 1. || continuation_probability < 0. )
      throw invalid_argument( "Invalid continuation probability : " + to_string( cp ) );
  }

  virtual bool operator() () const override
  {
    const real_type rand01 = uniform_random< real_type >( 0, 1 );
    return rand01 <= continuation_probability;
  }

  virtual real_type survivor_weight() const override { return weight; }

  virtual size_t num_rays() const override { return 1; }
};


pair< direction< 3 >, direction< 3 > > tangent_vectors( direction< 3 > normal )
{
  // student begin
  real_type const n0abs = fabs( normal[0] );
  real_type const n1abs = fabs( normal[1] );
  real_type const n2abs = fabs( normal[2] );

  short const less12 = n1abs < n2abs ? 1 : 2;
  short const smallest_n_idx = n0abs < n1abs ? ( n0abs < n2abs ? 0 : less12 ) : less12;

  position< 3 > ext_pt( 0.,0.,0. );
  ext_pt[ smallest_n_idx ] = 1.;

  direction< 3 > span_s( ext_pt - dot( ext_pt, normal ) * normal );
  span_s.normalize();
  auto span_t = cross( normal, span_s );

  if( dot( normal, cross( span_s, span_t ) ) < 0. )
    span_t = -span_t;
  return { span_s, span_t };
  // student end
};

direction< 3 > cosine_importance_hemisphere_sample( direction< 3 > const& normal )
{
  // student begin

  // student end
}

direction< 3 > uniform_hemisphere_sample( direction< 3 > const& normal )
{
  real_type const r1 = uniform_random< real_type >( 0., 1. );
  real_type const r2 = uniform_random< real_type >( 0., 1. );

  auto const phi = 2 * real_type(M_PI) * r1;
  auto const rad = sqrt( 1.f - r2 * r2 );

  si::length  x  = cos( phi ) * rad;
  si::length  y  = sin( phi ) * rad;
  si::length  z  = r2;

  auto span = tangent_vectors( normal );

  return x * span.first + y * span.second + z * normal;
}

struct HemisphereSampler
{
  struct sample
  {
    direction<3> dir;
    probability  factor;
  };
  // Return random direction and normalization factor for the hemisphere integration.
  virtual sample operator() ( direction<3> const & normal ) = 0;
};

struct UniformHemisphereSampler : public HemisphereSampler
{
  virtual sample operator() ( direction<3> const & normal ) final override
  {
    auto random_dir = uniform_hemisphere_sample( normal );
    return { random_dir, fabs( 2 /** M_PI*/ * dot( normal, random_dir ) ) };
  }
};

radiance_rgb Pathtracer::estimate_indirect_radiance( intersection_info const& hit, size_t recursion_depth )
{
  RussianRoullete terminator( hit.object()->material()->get_diffuse().max_elem() );
  
  if( ! terminator() ) return radiance_rgb( 0., 0., 0. );

  UniformHemisphereSampler hemisphere_sampler;
  
  radiance_rgb result( 0., 0. ,0. );
  size_t const num_rays = terminator.num_rays();
  for( size_t r = 0; r < num_rays; ++r )
  {
    auto direction_sample = hemisphere_sampler( hit.normal() );

    tiny_vec< real_type, 3 > const brdf = hit.object()->material()->brdf( - direction_sample.dir, hit.direction_to_camera(), hit );

    intersection_info indirect_hit( ray( hit.location(), direction_sample.dir ) );

    // student begin
    indirect_hit.set_potential( hit.potential() /*TODO*/ );
    // student end

    radiance_rgb const  incoming_radiance =  direction_sample.factor * trace( indirect_hit, recursion_depth + 1 );
    radiance_rgb const reflected_radiance =  terminator.survivor_weight() * brdf * incoming_radiance;

    if( reflected_radiance.min_elem() < 0 ) 
      throw logic_error( "Negative radiance" );

    result += reflected_radiance;
  }

  return result / real_type( num_rays );
}

radiance_rgb Pathtracer::direct_reflected_radiance( intersection_info const& hit )
{
  radiance_rgb result( 0.,0.,0. );

  for( auto const& al : _scene->lights() )
  { 
    position<3>  l_pos; // position sample on light source
    direction<3> l_dir; // direction to sample on light source
    si::length   l_dist;// distance to light source
    probability  l_prob;// probability of choosing position sample
    al->sample( hit, l_pos, l_dir, l_dist, l_prob );

    ray shadow_ray( hit.location(), l_dir );
    if( ! _scene->root()->any_intersection( shadow_ray, si::length(1e-3), l_dist ) )
    {
      auto const brdf = hit.object()->material()->brdf( -l_dir, hit.direction_to_camera(), hit );
      auto const geometry_factor = clamp< real_type >( dot( hit.normal(), l_dir ), 0., 1. );
      auto const incoming_radiance = al->emitted_radiance( l_pos, - l_dir, l_dist );

      result += brdf * geometry_factor * incoming_radiance / l_prob;
    }
  }

  return result;
}

radiance_rgb Pathtracer::trace( intersection_info& hit, size_t recursion_depth )
{
  if( _scene->root()->closest_intersection( &hit, si::length(1e-3) ) ) //if intersection is found... 
  {
    radiance_rgb flux( 0,0,0 );

    if( hit.potential().max_elem() < 1e-3 )
      return flux;

    flux += direct_reflected_radiance( hit );

    flux += estimate_indirect_radiance( hit, recursion_depth );

    return flux;
  }

  return _scene->background_radiance();
}
