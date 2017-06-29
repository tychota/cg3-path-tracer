# pragma once

# include "cg3/common/primitive.hpp"

class cylinder: public primitive
{
  public:
    cylinder();

    cylinder( position<3> const& p1, position<3> const& p2, const si::length r);

    void calc_normal( intersection_info* hit) const;

    bool closest_intersection( intersection_info* hit, si::length min_lambda ) const final override;

    bool any_intersection( ray& r, si::length min_lambda, si::length max_lambda ) const final override; 

    void set_finite( bool b );

    bool is_finite() const;

    void set_caps( bool b );

    bool has_caps() const;

  private:
    position<3> bottom;
    position<3> top;

    si::length radius;

    bool finite;
    bool caps;
};
