# pragma once

# include "cg3/common/light_source.hpp"
# include "cg3/common/area_light.hpp"
# include "cg3/common/primitive.hpp"
# include "cg3/common/camera.hpp"

# include <vector>
# include <memory>

class Scene
{
  public:
    Scene();
    virtual ~Scene();

    void add_light( light_source * );

    std::size_t num_lights() const;
    std::vector< light_source *      > lights();
    std::vector< light_source const* > lights() const;

    radiance_rgb background_radiance() const;
    void     set_background_radiance( radiance_rgb const& color );

    //get viewing camera
    std::shared_ptr< Camera >       camera();
    std::shared_ptr< Camera const > camera() const;

    void set_camera( std::shared_ptr< Camera > c);

    // Get primitives
    primitive*       root();
    primitive const* root() const;
    void         set_root(primitive* r);

    // Return index of refraction of ambient medium (usually air).
    real_type get_index_of_refraction() const;

  private:
    primitive*                   _root;
    std::vector< light_source* > _lights;
    std::vector< area_light* >   _area_lights;
    std::shared_ptr< Camera >    _camera;
    radiance_rgb                 _background;
};
