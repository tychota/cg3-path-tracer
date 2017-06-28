# pragma once
# include "cg3/common/si_units.hpp"
# include "cg3/common/mat.hpp"

class ray
{
  public:

    ray(){}

    ray( const ray& r )
    : _origin( r._origin ), _direction( r._direction ), _inv_direction( r._inv_direction )
    {}

    ray( position<3> const& origin, direction<3> const& dir)
    { 
      set_origin( origin );
      set_direction( dir ); 
    }

    ray( si::length ox, si::length oy, si::length oz
       , si::length dx, si::length dy, si::length dz )
    {
      set_origin( ox,oy,oz );
      set_direction( dx,dy,dz );
    }

    void set_origin( position<3> const& orig)
    {
      _origin = orig;
    }

    void set_origin( si::length ox, si::length oy, si::length oz )
    {
      _origin[0] = ox;
      _origin[1] = oy;
      _origin[2] = oz;
    }

    position<3> &      origin()       { return _origin; }
    position<3> const& origin() const { return _origin; }

    void set_direction( real_type dx, real_type dy, real_type dz )
    {
        _direction[0] = dx;
        _direction[1] = dy;
        _direction[2] = dz;
        _direction.normalize();
        _inv_direction[0] = 1.0f / _direction[0];
        _inv_direction[1] = 1.0f / _direction[1];
        _inv_direction[2] = 1.0f / _direction[2];
    }
    void set_direction( direction<3> const& dir )
    {
        _direction = dir;
        _direction.normalize();
        _inv_direction[0] = 1.0f/_direction[0];
        _inv_direction[1] = 1.0f/_direction[1];
        _inv_direction[2] = 1.0f/_direction[2];

    }
    direction<3> &      dir()      { return _direction; }
    direction<3> const& dir()const { return _direction; }

    direction<3> &      inv_dir()       { return _inv_direction; }
    direction<3> const& inv_dir() const { return _inv_direction; }

    position<3> eval_at( si::length lambda) const { return _origin + lambda * _direction;}

    void transform( mat<real_type> const& m )
    {
      vec<real_type> p = m * homog( _origin );
      set_origin( unhomog( direction<4>( p.begin(), p.end() ) ) );
      direction<4> dirh = homog_dir( _direction );
      vec<real_type> d = m * dirh;
      direction<3> dir( d.begin(), d.end()-1 );
      set_direction(dir);
    }

  private:
    position<3>  _origin;
    direction<3> _direction;
    direction<3> _inv_direction;

};