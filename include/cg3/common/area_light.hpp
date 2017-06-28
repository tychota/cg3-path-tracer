# pragma once

# include "cg3/common/light_source.hpp"

class area_light : public light_source
{
  public:
    area_light( position<3> const& pos
              , direction<3> const& span_x
              , direction<3> const& span_y
              , tiny_vec< si::power, 3 > const& total_power 
              );

    virtual ~area_light() {};

    virtual void sample( intersection_info const& scene_sample
                       , position<3>&  light_sample
                       , direction<3>& light_direction
                       , si::length&   light_distance
                       , probability& p  
                       ) const override;

    radiance_rgb emitted_radiance( position< 3 > light_pos
                                 , direction<3> dir
                                 , si::length dist 
                                 ) const override;

  protected:
    si::area area() const { return cross( _span_x, _span_y ).length(); }

    position< 3 >  _pos;
    direction< 3 > _span_x;
    direction< 3 > _span_y;
    direction< 3 > _normal;
};

