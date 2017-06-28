# pragma once
# include "cg3/common/primitive.hpp"

class triangle: public primitive
{
  public:
    triangle(const tiny_vec<float,3>& p0, const tiny_vec<float,3>& p1, const tiny_vec<float,3>& p2,
        const tiny_vec<float,3>& n1, const tiny_vec<float,3>& n2, const tiny_vec<float,3>& n3);

    triangle( position<3>  const& p0, position<3>  const& p1, position<3>  const& p2);

    std::pair< position<3>, position<3> > calc_bounds() const final override;

    bool closest_intersection( intersection_info* hit, si::length min_lambda ) const final override;

    bool any_intersection( ray& r, si::length min_lambda, si::length max_lambda ) const final override;

  protected:
    void calc_normal( intersection_info* hit, si::length u, si::length v ) const;

  private:
    position<3> v0;
    direction<3> edge1,edge2;
    direction<3> normal;
    direction<3> nml1,nml2,nml3;
};
