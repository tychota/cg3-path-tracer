#include "cg3/common/camera.hpp"
#include "cg3/common/utils.hpp"

void Camera::set_look_at( position<3> const& eye, position<3> const& center, direction<3> const& up )
{
  _eye = eye;
  _w_dir = _eye-center;
  _w_dir.normalize();

  _v_dir = up;
  _v_dir.normalize();
  
  _u_dir = cross( _v_dir, _w_dir );
  _u_dir.normalize();

  _v_dir = cross( _w_dir, _u_dir );
  _v_dir.normalize();
}

void Camera::set_perspective( degree_type fov, real_type aspect
                            , si::length znear, si::length zfar )
{
  _top    = znear * tan( fov * 3.14159f / 360.0f );
  _bottom = -_top;
  _right  = aspect * _top;
  _left   = - _right;
  _zfar   = zfar;
  _znear  = znear;
}

void Camera::set_frustrum( si::length left, si::length right, si::length top, si::length bottom
                         , si::length znear, si::length zfar )
{
  _left   = left;
  _right  = right;
  _top    = top;
  _bottom = bottom;
  _znear  = znear;
  _zfar   = zfar;
}

position<3>& Camera::origin()
{
  return _eye;
}

position<3> const& Camera::origin() const
{
  return _eye;
}

//get ray direction through center of pixel(i,j)
direction<3> Camera::ray_direction( si::length u, si::length v
                                  , tiny_vec< std::size_t, 2 > const& res ) const
{
  si::length const us = _left + (_right-_left) * u  / res[0];
  si::length const vs = _bottom + ( _top - _bottom ) * ( real_type( res[1] ) - v ) / res[1];
  si::length const ws = -_znear;

  direction<3> dir =  us * _u_dir + vs * _v_dir + ws * _w_dir;
  dir.normalize();
  return dir;
}

float Camera::get_znear() const
{
  return _znear;
}

float Camera::get_zfar() const
{
  return _zfar;
}

Camera::Camera() : _eye(0,0,0), _u_dir(1,0,0), _v_dir(0,1,0), _w_dir(0,0,1)
{
  set_perspective( 45.0f, 1., 0.01f, 100.0f );
}
