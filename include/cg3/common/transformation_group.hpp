# pragma once

#include "cg3/common/primitive_group.hpp"

class transformation_group: public primitive_group
{
  public:
    transformation_group();

    transformation_group( mat< real_type >& tr );

    void set_transformation( const mat< real_type >& T );

    virtual bool closest_intersection( intersection_info* hit, si::length min_lambda ) const override;

    virtual bool any_intersection( ray& r,si::length min_lambda, si::length max_lambda ) const override;

  private:
    mat< real_type > trafo;
    mat< real_type > trafo_inv;
    mat< real_type > trafo_inv_transpose;
};
