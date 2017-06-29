# pragma once

# include "cg3/common/primitive.hpp"
# include <vector>

class primitive_group: public primitive
{
  public:

    virtual ~primitive_group();
    virtual bool closest_intersection( intersection_info* hit, si::length min_lambda ) const override;
    virtual bool any_intersection( ray& r, si::length min_lambda, si::length max_lambda ) const override;
    virtual void add_primitive(primitive* p);
    virtual void clear();

  protected:
    std::vector<primitive*> primitives;
};
