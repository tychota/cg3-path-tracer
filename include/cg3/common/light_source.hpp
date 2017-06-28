# pragma once

# include "cg3/common/intersection_info.hpp"
# include "cg3/common/si_units.hpp"

struct shader_info;

class light_source
{
  public:
    light_source();
    light_source( tiny_vec< si::power, 3 > const& );
    virtual ~light_source() {};
 
    void set_total_power( tiny_vec< si::power, 3 > const& col ); 
    void set_total_power( si::power r, si::power g, si::power b);

    tiny_vec< si::power, 3 >      & total_power(); 
    tiny_vec< si::power, 3 > const& total_power() const;

    virtual void sample( intersection_info const& scene_sample
                       , position<3>&  light_sample
                       , direction<3>& light_direction
                       , si::length&   light_distance
                       , probability&  light_sample_probability
                       ) const = 0;

    virtual radiance_rgb emitted_radiance( position< 3 > light_pos
                                         , direction<3> dir
                                         , si::length dist 
                                         ) const = 0;

  protected:
    tiny_vec< si::power, 3 > _total_power;
};
