#include "cg3/common/transformation_group.hpp"




transformation_group::transformation_group()
{
	trafo = eye< real_type >(4);	
}

transformation_group::transformation_group( mat< real_type >& tr )
{
	set_transformation( tr );
}

void transformation_group::set_transformation(const mat< real_type >& T)
{
	trafo = T;

	trafo_inv = inv_44< real_type >(T);
	
	trafo_inv_transpose = transpose(trafo_inv);	
	
}

bool transformation_group::closest_intersection( intersection_info* hit, si::length min_lambda ) const
{
  //transform incoming ray  and lambda into local frame

  ray temp = hit->incoming_ray();
  ray rt = temp;
  rt.transform( trafo_inv );
  hit->set_incoming_ray( rt );

  bool intersect =  primitive_group::closest_intersection( hit, min_lambda );

  if(intersect)
  {
    vec< real_type > n = trafo_inv_transpose * homog_nml<float,3>( hit->normal() );
    tiny_vec< real_type,4> nn( n.begin(), n.end() );
    tiny_vec< real_type,3> nml = unhomog_nml<float,4>( nn );

    hit->set_normal( nml );
  }
  
  // invalid -> // NOTE: setting incoming_ray before set_normal (which contains logic requiring ray)
  hit->set_incoming_ray( temp );
  
  return intersect;
}

bool transformation_group::any_intersection( ray& r,si::length min_lambda, si::length max_lambda ) const
{
  ray rt = r;
  rt.transform( trafo_inv );
  return primitive_group::any_intersection( rt, min_lambda, max_lambda );
}

