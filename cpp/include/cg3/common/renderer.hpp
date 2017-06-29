# pragma once

# include "cg3/common/scene.hpp"

# include <memory>

class Renderer
{
  public:

    Renderer() = delete;

    Renderer( std::shared_ptr< Scene const > s ) : _scene( s ) {}

    virtual ~Renderer() {}

    virtual radiance_rgb trace( intersection_info& hit, std::size_t recursion_depth ) = 0;

    std::shared_ptr< Camera const > camera() const { return _scene->camera(); } 

  protected:

    std::shared_ptr< Scene const > _scene;
};