# pragma once

# include "cg3/common/light_source.hpp"

class spot_light: public light_source
{
  public:
    spot_light();

    void set_spot_direction(float dx, float dy, float dz);

    void set_direction( direction< 3 > const& dir);

    void set_position( float px, float py, float pz );
    void set_position( position< 3 > const& pos );

    void  set_spot_exponent( float exponent );
    float get_spot_exponent() const;

    void set_spot_cutoff( float co );

    virtual void sample( intersection_info const& //scene_sample
                       ,  position<3>& //light_sample
                       , direction<3>& //light_direction
                       ,   si::length& //light_distance
                       ,  probability& //light_sample_probability
                       ) const override;

    // TODO: calculation of emitted_radiance wrt _total_power is INCORRECT
    virtual radiance_rgb emitted_radiance(  position< 3 > //light_pos
                                         , direction< 3 > //dir
                                         ,     si::length //dist
                                         ) const override;

  private:
    position< 3 > _position;
    direction< 3 > _spot_direction;
    real_type _spot_exponent;
    real_type _spot_cos_cutoff;
};