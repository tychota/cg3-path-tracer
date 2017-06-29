# pragma once

# include "cg3/common/renderer.hpp"
# include "cg3/common/scene.hpp"

class Pathtracer : public Renderer
{
  public:
    //using Renderer::Renderer;
	Pathtracer(std::shared_ptr< Scene const > s) : Renderer(s) {}

    virtual radiance_rgb trace( intersection_info& hit, std::size_t recursion_depth) final override;

  protected:
    radiance_rgb estimate_indirect_radiance( intersection_info const& hit, std::size_t recursion_depth );
    radiance_rgb direct_reflected_radiance( intersection_info const& hit );
};

