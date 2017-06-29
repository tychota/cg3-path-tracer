# pragma once

# include "cg3/common/tiny_vec.hpp"
# include "cg3/common/ray.hpp"
# include "cg3/common/si_units.hpp"

class primitive;

class intersection_info
{
  public:
    intersection_info();
    intersection_info( ray r, si::length l = std::numeric_limits< si::length >::infinity() ) ;

    void set_lambda( si::length l);
    si::length lambda() const;
    position< 3 > const& location() const;

    void set_incoming_ray( ray const& );
    ray const& incoming_ray() const;

    void set_normal( direction< 3 > const& );
    direction< 3 > const& normal() const;

    primitive const* object() const;
    void set_object( primitive const* obj);

    tiny_vec< real_type, 3 > const& potential() const;
    void  set_potential( tiny_vec< real_type, 3 > const& );

    real_type      geometry_factor() const;
    direction< 3 > direction_to_camera() const;
    direction< 3 > reflected_incoming_direction() const;

  private:
    ray              _incoming_ray;
    primitive const* _object; // intersected object
    si::length       _lambda;
    position< 3 >    _location; // computed in set_lambda()
    direction< 3 >   _normal; // surface normal at intersection point
    real_type        _geometry_factor; // computed in set_normal()
    tiny_vec< real_type, 3 > _potential;

};