#ifndef PLANE_H
#define PLANE_H

#include "cg3/common/primitive.hpp"

class plane: public primitive
{
public:
    plane();

    plane(const tiny_vec<float,3>& nml, float d);

    virtual bool closest_intersection( intersection_info* hit, si::length min_lambda ) const final override;

    virtual bool any_intersection( ray& r, si::length min_lambda, si::length max_lambda ) const final override;

  private:
    direction<3> normal;

    si::length distance;
};

#endif