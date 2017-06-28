# pragma once

# include "cg3/common/intersection_info.hpp"
# include "cg3/common/material.hpp"

class primitive
{
  public:
    void set_name(const std::string& s);

    std::string get_name() const;

    primitive();

    primitive( Material* m );

    void set_material( Material* m );

    Material* material() const;

    virtual std::pair< position<3>, position<3> > calc_bounds() const;

    virtual bool closest_intersection( intersection_info* hit, si::length min_lambda ) const = 0;

    virtual bool any_intersection( ray& r, si::length min_lambda, si::length max_lambda ) const = 0;

    virtual ~primitive();

  protected:
    std::string name;
    Material* _material;

};




