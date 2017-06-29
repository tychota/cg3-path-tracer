#include "cg3/common/light_source.hpp"
#include "cg3/common/utils.hpp"

light_source::light_source()
: _total_power( 0.,0.,0. )
{}

light_source::light_source( tiny_vec< si::power, 3 > const& total_emissive_power )
: _total_power( total_emissive_power ) 
{}

void light_source::set_total_power( tiny_vec< si::power, 3 > const& col ) 
{ 
  _total_power = col; 
}

void light_source::set_total_power( si::power r, si::power g, si::power b) 
{
  _total_power[0] = r;
  _total_power[1] = g;
  _total_power[2] = b;
}

tiny_vec< si::power, 3 >& light_source::total_power() 
{
  return _total_power; 
}

const tiny_vec< si::power, 3 >& light_source::total_power() const
{ 
  return _total_power; 
}