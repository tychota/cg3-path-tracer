#include "cg3/common/primitive.hpp"
#include <limits>






void primitive::set_name(const std::string& s)
{
	name = s;
}
std::string primitive::get_name() const 
{
	return name;
}

primitive::primitive()
{
	_material=NULL;
}
primitive::primitive(Material* m)
{
	_material = m;
}

void primitive::set_material(Material* m)
{
	_material=m;
}

Material* primitive::material() const
{
	return _material;
}

std::pair< position<3>, position<3> > primitive::calc_bounds() const
{	
	tiny_vec<float,3 > a(-(std::numeric_limits<float>::infinity)(),-(std::numeric_limits<float>::infinity)(),-(std::numeric_limits<float>::infinity)());
	tiny_vec<float,3 > b((std::numeric_limits<float>::infinity)(),(std::numeric_limits<float>::infinity)(),(std::numeric_limits<float>::infinity)());
	return std::make_pair(a,b);
};

primitive::~primitive()
{
}