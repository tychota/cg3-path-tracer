# pragma once

# include "cg3/common/material.hpp"

class phong : public Material
{
    virtual tiny_vec< real_type, 3 > brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& ) const override;

  public:
    virtual tiny_vec< real_type, 3 > diffuse_brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& ) const override;
    virtual tiny_vec< real_type, 3 > specular_brdf( direction< 3 > const& in, direction< 3 > const& out, intersection_info const& ) const override;
};
