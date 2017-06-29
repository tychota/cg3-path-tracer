#ifndef BOX_H
#define BOX_H

#include "cg3/common/primitive.hpp"

class box : public primitive
{

	
public: 
	
    box();
    box(const tiny_vec<float,3>& minimum, const tiny_vec<float,3>& maximum);
	virtual ~box();


    std::pair< position<3>, position<3> > calc_bounds() const final override;

    void extend(const tiny_vec<float,3>& a);

	void extend(const std::pair<tiny_vec<float,3>,tiny_vec<float,3> >& minmax);
 
    void extend(const box& b);
 
    void clear();

    position<3> &      get_min();
    position<3> const& get_min() const;

    position<3> &      get_max();
    position<3> const& get_max() const;
 
    tiny_vec< si::length, 3 > get_extends() const;

    position<3> get_center() const;

     std::pair< si::length, si::length > intersect( const ray &r ) const;

	void calc_normal( intersection_info* hit ) const;

    bool closest_intersection( intersection_info* hit, si::length min_lambda ) const final override;

    bool any_intersection( ray& r, si::length min_lambda, si::length max_lambda ) const final override;
private:
	position<3> minimum; 
    position<3> maximum; 


 };

#endif