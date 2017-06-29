# pragma once

# include "cg3/common/primitive.hpp"

class sphere: public primitive
{
  public:
    sphere();
    sphere( si::length cx, si::length cy, si::length cz, si::length r);
    sphere( position<3> const& c, si::length r);

    void set_radius( si::length r);

    std::pair< position<3>, position<3> > calc_bounds() const final override;

    bool closest_intersection( intersection_info* hit, si::length min_lambda ) const final override;
    bool any_intersection( ray& r, si::length min_lambda, si::length max_lambda ) const final override;

  protected:
    void calc_normal( intersection_info* hit ) const;

  private:
    position<3> center;
    si::length radius;
    si::length sqr_radius; //TODO: unit is length^2
};