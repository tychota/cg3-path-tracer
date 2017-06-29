# pragma once

# include "cg3/common/light_source.hpp"

class point_light : public light_source
{
  public:
    point_light();
    point_light( si::length px, si::length py, si::length pz);

    void set_position( si::length px, si::length py, si::length pz );
    void set_position( position<3> const& pos );

    virtual void sample( intersection_info const& //scene_sample
                       ,  position<3>& //light_sample
                       , direction<3>& //light_direction
                       ,   si::length& //light_distance
                       ,  probability& //light_sample_probability
                       ) const override;

    virtual radiance_rgb emitted_radiance(  position< 3 > //light_pos
                                         , direction< 3 > //dir
                                         ,     si::length //dist
                                         ) const override;

  private:
    tiny_vec<float,3> _position;
};
