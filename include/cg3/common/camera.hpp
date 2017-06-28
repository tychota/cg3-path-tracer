# pragma once

# include "cg3/common/si_units.hpp"

class Camera
{
  public:

    Camera();

    void set_look_at( position<3> const& eye, position<3> const& center, direction<3> const& up );

    void set_perspective( degree_type fov, real_type aspect
                        , si::length znear, si::length zfar );

    void set_frustrum( si::length left, si::length right, si::length top, si::length bottom
                    , si::length znear, si::length zfar );

    void set_resolution( int resx, int resy );

    position<3>& origin();
    position<3> const& origin() const;

    //get ray direction through center of pixel(i,j)
    direction<3> ray_direction( si::length u, si::length v
                              , tiny_vec< std::size_t, 2 > const& res ) const;

    si::length get_znear() const;

    si::length get_zfar() const;

  private:
    position<3>  _eye; 
    direction<3> _u_dir; //x direction of camera coordinate frame
    direction<3> _v_dir; //y direction of camera coordinate frame
    direction<3> _w_dir; //z direction of camera coordinate frame

    // view frustrum distances in camera coordinate frame
    si::length _left, _right, _top, _bottom, _znear, _zfar;
};

