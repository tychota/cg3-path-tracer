#include "cg3/common/scene.hpp"

using namespace std;

Scene::Scene()
: _root( nullptr )
, _camera( nullptr )
, _background( .4, .4, .8 ) 
{}

Scene::~Scene()
{
  if(_root)
    delete _root;

  //delete lights
  for( auto l : _lights )
    delete l;
  
  for( auto al : _area_lights )
    delete al;
}

void Scene::add_light( light_source *l )
{
  _lights.push_back( l );
}

size_t Scene::num_lights() const
{
  return _lights.size();
}

vector< light_source* > Scene::lights()
{ 
  return _lights; 
}

vector< light_source const* > Scene::lights() const
{  
  return { _lights.begin(), _lights.end() }; 
}

radiance_rgb Scene::background_radiance() const
{
  return _background;
}

void Scene::set_background_radiance( radiance_rgb const& bg )
{
  _background = bg;
}


shared_ptr< Camera > Scene::camera()
{
  return _camera;
}

shared_ptr< Camera const > Scene::camera() const
{
  return _camera;
}

void Scene::set_camera( shared_ptr< Camera > c)
{
  _camera = c;
}


primitive* Scene::root()
{
  return _root;
}

primitive const* Scene::root() const
{
  return _root;
}

void Scene::set_root(primitive* r)
{
  _root = r;
}

float Scene::get_index_of_refraction() const
{
  return 1.;
}
