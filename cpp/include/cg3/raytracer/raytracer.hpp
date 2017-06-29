# pragma once

# include "cg3/common/renderer.hpp"
# include "cg3/common/scene.hpp"

class Raytracer : public Renderer
{
    std::size_t _max_recursion_depth;

  public:

    Raytracer( std::shared_ptr< Scene const > s, int max_recursion_depth ) 
    : Renderer( s ), _max_recursion_depth( max_recursion_depth ) 
    {}

    tiny_vec<float,3> trace( intersection_info& hit, std::size_t recursion_depth ) final override;

  protected:
    radiance_rgb direct_reflected_radiance( intersection_info const& hit );

    tiny_vec<float,3> sample_fresnel_surface( float n_env
                                            , bool from_inside
                                            , intersection_info const& hit
                                            , std::size_t recursion_depth
                                            );

};
