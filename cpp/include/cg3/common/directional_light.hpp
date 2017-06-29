# pragma once

# include "cg3/common/light_source.hpp"
# include "cg3/common/si_units.hpp"

class directional_light: public light_source
{
  public:
    directional_light();

    directional_light( real_type dx, real_type dy, real_type dz);

    directional_light( direction<3> const& dir );

    void set_direction( real_type dx, real_type dy, real_type dz);

    void set_direction( direction<3> const& dir );

    virtual void sample( intersection_info const& scene_sample
                       ,  position<3>& light_sample
                       , direction<3>& light_direction
                       ,   si::length& light_distance
                       ,  probability& light_sample_probability
                       ) const override;

    // TODO: calculation of emitted_radiance wrt _total_power is INCORRECT
    virtual radiance_rgb emitted_radiance(  position< 3 > light_pos
                                         , direction< 3 > dir
                                         ,     si::length dist
                                         ) const override;

  private:
    direction< 3 > _direction;

};
