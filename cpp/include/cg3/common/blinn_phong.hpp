# pragma once

#include "cg3/common/material.hpp"
#include "cg3/common/phong.hpp"

class blinn_phong : public phong
{
    virtual tiny_vec< real_type, 3 > brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& hit ) const override;

  public:
    virtual tiny_vec< real_type, 3 > specular_brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& hit ) const override;
};
