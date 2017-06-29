# include "cg3/common/test_scenes.hpp"

# include "cg3/common/point_light.hpp"
# include "cg3/common/directional_light.hpp"
# include "cg3/common/spot_light.hpp"
# include "cg3/common/box.hpp"
# include "cg3/common/plane.hpp"
# include "cg3/common/sphere.hpp"
# include "cg3/common/cylinder.hpp"
# include "cg3/common/primitive_group.hpp"
# include "cg3/common/transformation_group.hpp" 
# include "cg3/common/kd_tree.hpp"
# include "cg3/common/triangle.hpp"
# include "cg3/common/blinn_phong.hpp"
# include "cg3/common/phong.hpp"
# include "cg3/common/image_io.hpp"

using namespace std;


shared_ptr< Scene > create_rt_test_scene( tiny_vec< size_t, 2 > resolution )
{
  shared_ptr< Scene > myscene( new Scene );
  
  int resx = 640;
  int resy = 480;
  
  shared_ptr< Camera > view( new Camera );
  view->set_perspective(45,resolution[0]/float(resolution[1]),0.01f,1000);
  view->set_look_at( position<3>(6,6,20), position<3>(6,2,6), direction<3>(0,1,0));
  myscene->set_camera(view);
  
  myscene->set_background_radiance(tiny_vec<float,3>(0.1f,0.1f,0.2f));
  
  
  //root node of Scene
  primitive_group *group = new primitive_group();
  myscene->set_root(group);
  
  
  //several materials
  blinn_phong *redmat = new blinn_phong();
  redmat->set_diffuse(1.0f,0.0f,0.0f);
  redmat->set_specular(1.0f,1.0f,1.0f);
  
  blinn_phong *greenmat = new blinn_phong();
  greenmat->set_diffuse(0.0f,1.0f,0.0f);
  greenmat->set_specular(.0f,.0f,.0f);
  
  blinn_phong *bluemat = new blinn_phong();
  bluemat->set_diffuse(0.0f,0.0f,1.0f);
  bluemat->set_specular(1.0f,1.0f,1.0f);
  
  blinn_phong *graymat = new blinn_phong();
  graymat->set_diffuse(0.8f,0.8f,0.8f);
  graymat->set_specular(1.0f,1.0f,1.0f);
  
  // ground plane
  primitive* pl = new plane(  tiny_vec<float,3>(0, 1, 0) , -2.2f );
  pl->set_name( "plane" );
  pl->set_material(new blinn_phong());
  //pl->material()->set_reflectivity( 0.3f );
  pl->material()->set_diffuse( 0.8f,0.8f,0.8f );
  pl->material()->set_specular( 0.8f,0.8f,0.8f );
  group->add_primitive(pl);
  
  //coordinate system axis
  cylinder* cylx = new cylinder(tiny_vec<float,3>(0,0,0),tiny_vec<float,3>(5.0f,0,0),0.2f );
  cylx->set_material(redmat);
  cylx->set_name( "cylinder X" );
  cylx->set_finite(true);
  cylx->set_caps(true);
  group->add_primitive(cylx);
  
  cylinder* cyly = new cylinder(tiny_vec<float,3>(0,0,0),tiny_vec<float,3>(0,5.0f,0),0.2f );
  cyly->set_name( "cylinder Y" );
  cyly->set_material(greenmat);
  cyly->set_finite(true);
  cyly->set_caps(true);
  group->add_primitive(cyly);
  
  cylinder* cylz = new cylinder(tiny_vec<float,3>(0,0,0),tiny_vec<float,3>(0,0,5.0f),0.2f );
  cylz->set_material(bluemat);
  cylz->set_name( "cylinder Z" );
  cylz->set_finite(true);
  cylz->set_caps(true);
  group->add_primitive(cylz);
  
  
  primitive *s1 = new sphere(  5.0, 0.0, 0.0 , 0.5f );
  s1->set_name( "x sphere" );
  s1->set_material(redmat);
  
  group->add_primitive(s1);
  
  primitive *s2 = new sphere(  0.0, 5.0, 0.0 , 0.5f );
  s2->set_name( "y sphere" );
  s2->set_material(greenmat );
  
  group->add_primitive(s2);
  
  primitive*s3 = new sphere(  0.0, 0.0, 5.0 , 0.5f );
  s3->set_name( "z sphere" );
  s3->set_material(bluemat);
  group->add_primitive(s3);
  
  primitive* s4 = new sphere(  0.0f, 0.0f, 0 , 0.5f );
  s4->set_name( "center" );
  s4->set_material(graymat );
  group->add_primitive(s4);
  
  
  //another big mirroring sphere
  primitive* sp = new sphere(  0.0, 1.0, -5.0 , 3.0f );
  sp->set_name( "big sphere" );
  sp->set_material(new blinn_phong());
  sp->material()->set_diffuse(  1.0f, 0.0f, 1.0f  );
  sp->material()->set_reflectivity(0.5f);
  group->add_primitive(sp);
  
  //an axis aligned box   
  //   primitive* boxsmall = new box(  tiny_vec<float,3>(11.0f, -2.0, -4.0) ,tiny_vec<float,3>(14.0f,2.0f,0.0f ) );
  //   boxsmall->set_name( "box small" );
  //   boxsmall->set_material(new blinn_phong());
  //   boxsmall->material()->set_diffuse(  0.5f, 0.5f, 0.5f  );
  //   boxsmall->material()->set_reflectivity(0.1f);
  //   boxsmall->material()->set_refractivity(0.3f);
  //   group->add_primitive(boxsmall);
  
  //an axis aligned box   
  primitive* boxsmallfront = new sphere( 4.,1.,3.5, 3. );
  boxsmallfront->set_name( "box small front" );
  boxsmallfront->set_material(new blinn_phong());
  boxsmallfront->material()->set_diffuse(  .1, .1, .1  );
  boxsmallfront->material()->set_reflectivity( 0. );
  boxsmallfront->material()->set_refractivity( 1. );
  boxsmallfront->material()->set_index_of_refraction( 1.3 );
  group->add_primitive(boxsmallfront);
  
  //a triangle mesh in a kdtree
  //   kd_tree *mesh = new kd_tree();
  //   if (!mesh->add_obj_mesh("../data/buddha.obj") && !mesh->add_obj_mesh("data/buddha.obj")) 
  //   {
  //     cout<<"Could not load obj-File!"<<endl;
  //   }
  //   else 
  //   {
  //     group->add_primitive(mesh);
  //     mesh->build_tree_spatial_median();
  //   }
  
  
  triangle *tri = new triangle(tiny_vec<float,3>(3.0f,0.0f,-10.0f),
                               tiny_vec<float,3>(7.0f,0.0f,-10.0f),
                               tiny_vec<float,3>(0.0f,7.0f,-10.0f));
  tri->set_material( new phong() );
  tri->material()->set_diffuse( .4,.8,.4 );
  group->add_primitive(tri);
  
  //a transformed axis aligned box
  
  transformation_group* t1 = new transformation_group();
  t1->set_transformation( translate_44<float>( 12.,0.,-20. ) * rotatex_44<float>( 45. ) * rotatez_44<float>( 45. ) );
  group->add_primitive( t1 );

  primitive* box2 = new box( tiny_vec<float,3>(-1.0, -1.0, -1.0) ,tiny_vec<float,3>(1.0,1.0f,1.0f ) );
  box2->set_material(new blinn_phong());
  box2->material()->set_diffuse(  1.0f, 1.0f, 1.0f  );
  //box2->material()->set_reflectivity(0.1f);
  t1->add_primitive(box2);

  //several different light sources

  //Point Light
  point_light* light0 = new point_light(  -10, 10, 10  );
  light0->set_total_power( tiny_vec< si::power, 3 >( .6f, .6f, .3f ) * 3e3 );
  myscene->add_light( light0 );

  //Spot light
  spot_light* spot = new spot_light(  );
  spot->set_total_power( 0.5,0.5,0. );
  spot->set_position( 7.5, 10.0, 11.0 );
  spot->set_spot_direction( 0.25, -1, -1 );
  spot->set_spot_cutoff( 10 );
  spot->set_spot_exponent( 250 );
  myscene->add_light( spot);
  
  // Directional light
  directional_light *light1 = new directional_light( -1.,-1.,-.3 );
  light1->set_total_power( .3,.1,.1 );
  myscene->add_light(light1);
  
  
  //Projector light with checker board image
  /*  mat<tiny_vec<float,3> >  pattern(1024,768);
   *  for(int y = 0; y < (int)pattern.h(); y++)
   *  {
   *  for(int x = 0; x < (int)pattern.w(); x++)
   *  {
   *  if(((x/10)%2 == 0)^((y/10)%2 == 0))
   *  pattern(x,y).set(1.0f,1.0f,1.0f);
   *  else
   *  pattern(x,y).set(0.0f,0.0f,0.0f);
}
}
projector_light* proj = new projector_light(pattern,20.0f,1024.0f/768.0f);
proj->set_look_at(tiny_vec<float,3>(12,20,20),tiny_vec<float,3>(6,2,6),tiny_vec<float,3>(0,1,0));
//  proj->disable_shadows();
proj->set_total_power(0.8f,0.8f,0.8f);
myscene->add_light(proj);*/
  
  
  return myscene;
  
}

shared_ptr< Scene > create_pt_test_scene( tiny_vec< size_t, 2 > resolution )
{
  shared_ptr< Scene > myscene( new Scene );


  shared_ptr< Camera > view( new Camera );
  view->set_perspective( 45, resolution[0]/float(resolution[1]), 0.01f, 1000 );
  view->set_look_at( position<3>(0,6,18), position<3>(2,2,4), direction<3>(0,1,0) );
  myscene->set_camera( view );

  myscene->set_background_radiance( radiance_rgb( 0.,0.,0.) );


  //root node of Scene
  primitive_group *group = new primitive_group();
  myscene->set_root(group);


  //several materials
  blinn_phong *redmat = new blinn_phong();
  redmat->set_diffuse(1.0f,0.0f,0.0f);
  redmat->set_specular(.0,.0,.0);

  blinn_phong *greenmat = new blinn_phong();
  greenmat->set_diffuse(0.0f,1.0f,0.0f);
  greenmat->set_specular(.0,.0,.0);

  blinn_phong *bluemat = new blinn_phong();
  bluemat->set_diffuse(0.0f,0.0f,1.0f);
  bluemat->set_specular(.0,.0,.0);

  blinn_phong *graymat = new blinn_phong();
  graymat->set_diffuse(0.8f,0.8f,0.8f);
  graymat->set_specular(.0,.0,.0);

  
  // ground plane
  primitive* pl = new plane(  tiny_vec<float,3>(0, 1, 0) , -2.f );
  pl->set_name( "plane" );
  pl->set_material(new blinn_phong());
  //pl->material()->set_reflectivity( 0.3f );
  pl->material()->set_diffuse( 0.8f,0.8f,0.8f );
  pl->material()->set_specular( 1.f,1.f,1.f );
  group->add_primitive(pl);

  //coordinate system axis
  cylinder* cylx = new cylinder(tiny_vec<float,3>(0,0,0),tiny_vec<float,3>(5.0f,0,0),0.2f );
  cylx->set_material(redmat);
  cylx->set_name( "cylinder X" );
  cylx->set_finite(true);
  cylx->set_caps(true);
  group->add_primitive(cylx);

  cylinder* cyly = new cylinder(tiny_vec<float,3>(0,0,0),tiny_vec<float,3>(0,5.0f,0),0.2f );
  cyly->set_name( "cylinder Y" );
  cyly->set_material(greenmat);
  cyly->set_finite(true);
  cyly->set_caps(true);
  group->add_primitive(cyly);

  cylinder* cylz = new cylinder(tiny_vec<float,3>(0,0,0),tiny_vec<float,3>(0,0,5.0f),0.2f );
  cylz->set_material(bluemat);
  cylz->set_name( "cylinder Z" );
  cylz->set_finite(true);
  cylz->set_caps(true);
  group->add_primitive(cylz);


  primitive *s1 = new sphere(  5.0, 0.0, 0.0 , 0.5f );
  s1->set_name( "x sphere" );
  s1->set_material(redmat);

  group->add_primitive(s1);

  primitive *s2 = new sphere(  0.0, 5.0, 0.0 , 0.5f );
  s2->set_name( "y sphere" );
  s2->set_material(greenmat );

  group->add_primitive(s2);

  primitive*s3 = new sphere(  0.0, 0.0, 5.0 , 0.5f );
  s3->set_name( "z sphere" );
  s3->set_material(bluemat);
  group->add_primitive(s3);

  primitive* s4 = new sphere(  0.0f, 0.0f, 0 , 0.5f );
  s4->set_name( "center" );
  s4->set_material(graymat );
  group->add_primitive(s4);


  //another big mirroring sphere
  primitive* sp = new sphere(  0.0, 1.0, -5.0 , 3.0f );
  sp->set_name( "big sphere" );
  sp->set_material(new blinn_phong());
  sp->material()->set_diffuse(  1.0f, 0.0f, 1.0f  );
  sp->material()->set_specular(  0., 1., 0.  );

  //sp->material()->set_reflectivity(0.05f);
  group->add_primitive(sp);

  //an axis aligned box   
//   primitive* boxsmall = new box(  tiny_vec<float,3>(11.0f, -2.0, -4.0) ,tiny_vec<float,3>(14.0f,2.0f,0.0f ) );
//   boxsmall->set_name( "box small" );
//   boxsmall->set_material(new blinn_phong());
//   boxsmall->material()->set_diffuse(  0.5f, 0.5f, 0.5f  );
//   boxsmall->material()->set_reflectivity(0.1f);
//   boxsmall->material()->set_refractivity(0.3f);
//   group->add_primitive(boxsmall);

  //an axis aligned box   
  primitive* boxsmallfront = new box(  tiny_vec<float,3>(3.f, -3.0, 2.0) ,tiny_vec<float,3>(5.0f,2.0f,5.0f ) );// new sphere( 4.,1.,3.5, 3. );
  boxsmallfront->set_name( "box front" );
  boxsmallfront->set_material(new blinn_phong());
  boxsmallfront->material()->set_diffuse(  1., .0, .0  );
  boxsmallfront->material()->set_reflectivity( 0. );
  boxsmallfront->material()->set_refractivity( .9 );
  boxsmallfront->material()->set_index_of_refraction( 1.3 );
  group->add_primitive(boxsmallfront);

  //a triangle mesh in a kdtree
//   kd_tree *mesh = new kd_tree();
//   if (!mesh->add_obj_mesh("../data/buddha.obj") && !mesh->add_obj_mesh("data/buddha.obj")) 
//   {
//     cout<<"Could not load obj-File!"<<endl;
//   }
//   else 
//   {
//     group->add_primitive(mesh);
//     mesh->build_tree_spatial_median();
//   }


  triangle *tri = new triangle( tiny_vec<float,3>(3.0f,0.0f,-10.0f),
                                tiny_vec<float,3>(7.0f,0.0f,-10.0f),
                                tiny_vec<float,3>(0.0f,7.0f,-10.0f) );
  tri->set_material( new phong() );
  tri->material()->set_diffuse( .4,.8,.4 );
  group->add_primitive(tri);

  //a transformed axis aligned box

  transformation_group* t1 = new transformation_group();
  t1->set_transformation( translate_44<float>( 12.,0.,-20. ) * rotatex_44<float>( 45. ) * rotatez_44<float>( 45. ) );
  group->add_primitive( t1 );

  primitive* box2 = new box( tiny_vec<float,3>(-1.0, -1.0, -1.0) ,tiny_vec<float,3>(1.0,1.0f,1.0f ) );
  box2->set_material(new blinn_phong());
  box2->material()->set_diffuse(  1.0f, 1.0f, 1.0f  );
  //box2->material()->set_reflectivity(0.1f);
  t1->add_primitive(box2);


  //several different light sources

  //Point Light
  point_light* light0 = new point_light(  -2., 4., 4.  );
  light0->set_total_power( tiny_vec< si::power, 3 >( .6f, .6f, .3f ) * 3e2 );
  myscene->add_light(light0);

  //Spot light
//   spot_light* spot = new spot_light(  );
//   spot->set_total_power(0.8f,0.8f,0.8f);
//   spot->set_position(2., -1,13.f);
//   spot->set_spot_direction(0.3,-.01,-1.);
//   spot->set_spot_cutoff(10);
//   spot->set_spot_exponent(250);
//   myscene->add_light(spot);

  // Directional light
  directional_light *light1 = new directional_light( -1.,-1.,-.3 );
  light1->set_total_power( .1,.1,.1 );
  myscene->add_light(light1);


//Projector light with checker board image
/*  mat<tiny_vec<float,3> >  pattern(1024,768);
  for(int y = 0; y < (int)pattern.h(); y++)
  {
  for(int x = 0; x < (int)pattern.w(); x++)
  {
  if(((x/10)%2 == 0)^((y/10)%2 == 0))
  pattern(x,y).set(1.0f,1.0f,1.0f);
  else
  pattern(x,y).set(0.0f,0.0f,0.0f);
}
}
  projector_light* proj = new projector_light(pattern,20.0f,1024.0f/768.0f);
  proj->set_look_at(tiny_vec<float,3>(12,20,20),tiny_vec<float,3>(6,2,6),tiny_vec<float,3>(0,1,0));
  //  proj->disable_shadows();
  proj->set_total_power(0.8f,0.8f,0.8f);
  myscene->add_light(proj);*/


  return myscene;
}


shared_ptr< Scene > create_cornell_box_test_scene( tiny_vec< size_t, 2 > resolution )
{
  shared_ptr< Scene > myscene( new Scene );

  shared_ptr< Camera > view( new Camera );
  view->set_perspective(39.308, resolution[0] / float(resolution[1]), 0.01f, 1000);
  view->set_look_at(position<3>(278, 273, -800), position<3>(278, 273, 0), direction<3>(0, 1, 0));
  myscene->set_camera( view );

  myscene->set_background_radiance( radiance_rgb( 0.,0.,0.) );

  //root node of Scene
  primitive_group *group = new primitive_group();
  myscene->set_root( group );
  
  //several materials

  blinn_phong *redmat = new blinn_phong();
  redmat->set_diffuse( 0.366046f, 0.0371827f, 0.0416385f );
  redmat->set_diffuse( 0.63f, 0.06f, 0.04f );
  redmat->set_specular( 0.0f, 0.0, 0.0f );
//   redmat->set_diffuse( 0.1f, 0.1f, 0.1f );
//   redmat->set_specular(1.0f, 0.0, 0.0f);
//   redmat->set_shininess( 10 );
  
  blinn_phong *greenmat = new blinn_phong();
  greenmat->set_diffuse( 0.162928f, 0.408903f, 0.0833759f );
  greenmat->set_diffuse( 0.15f, 0.48f, 0.09f );
  greenmat->set_specular( 0.0f, 0.0, 0.0f );
//   greenmat->set_diffuse( 0.1f, 0.1f, 0.1f );
//   greenmat->set_specular( 0.0f, 1.f, 0.0f );
//   greenmat->set_shininess( 10 );
  
  blinn_phong *bluemat = new blinn_phong();
  bluemat->set_diffuse( 0.0f, 0.0f, 0.5f );
  bluemat->set_specular( 0.0f, 0.0, 0.0f );
  
  blinn_phong *whitemat = new blinn_phong();
//   whitemat->set_diffuse( 0.740063f, 0.742313f, 0.733934f );
   whitemat->set_diffuse( 0.76f, 0.75f, 0.5f );
  whitemat->set_specular(0.0f, 0.0, 0.0f);

  //   auto const light_color = tiny_vec< si::power, 3 >{ 0.780131,0.780409,0.775833 };
  auto const light_power = tiny_vec< si::power, 3>( 0.75f, 0.75f, 0.75f ) * 2000000;
  
  
  
  //Point Lights to "simulate" area light
  //for ( float j = -1.f; j < 3; j += 1.f )
  //for ( float i = -1.f; i < 3; i += 1.f )
  //{
  
  // point_light* light0 = new point_light( 278.0 + i*10, 548.0, 279.5 + j * 10 );
  // light0->set_total_power( tiny_vec<float, 3>( 1.0f, 0.80f, 0.5f ) * 0.035 );
  // myscene->add_light( light0 );
  //}
  
  area_light* area_light0 = new area_light( {213.f ,548.7f, 227.f }, { 130.f, 0.f, 0.f }, { 0.f, 0.f, 105 }, light_power  );
  myscene->add_light( area_light0 );

  // Single Point light
//     point_light* light0 = new point_light( 278.0, 548.7, 279.5 );
//     light0->set_total_power( light_power );
//     myscene->add_light( light0 );

  //coordinate system axis

  cylinder* cylx = new cylinder(tiny_vec<float,3>(0,0,0),tiny_vec<float,3>(50.0f,0,0),5.0f );
  cylx->set_material(redmat);
  cylx->set_name( "cylinder X" );
  cylx->set_finite(true);
  cylx->set_caps(true);
  group->add_primitive(cylx);
  
  cylinder* cyly = new cylinder(tiny_vec<float,3>(0,0,0),tiny_vec<float,3>(0,50.0f,0),5.0f );
  cyly->set_name( "cylinder Y" );
  cyly->set_material(greenmat);
  cyly->set_finite(true);
  cyly->set_caps(true);
  group->add_primitive(cyly);
  
  cylinder* cylz = new cylinder(tiny_vec<float,3>(0,0,0),tiny_vec<float,3>(0,0,50.0f),5.0f );
  cylz->set_material(bluemat);
  cylz->set_name( "cylinder Z" );
  cylz->set_finite(true);
  cylz->set_caps(true);
  group->add_primitive(cylz);
  
  /* CLASSIC CORNELL BOX */
  //floor
  primitive* floor = new plane( tiny_vec<float, 3>( 0, 1, 0 ), 0.0f );
  floor->set_name( "floor" );
  floor->set_material( whitemat );
  group->add_primitive( floor );
  
  //ceiling
  primitive* ceiling = new plane( tiny_vec<float, 3>( 0, -1, 0 ), -548.8f );
  ceiling->set_name( "ceiling" );
  ceiling->set_material( whitemat );
  group->add_primitive( ceiling );
  
  //back wall
  primitive* back_wall = new plane( tiny_vec<float, 3>( 0, 0, -1 ), -559.2f );
  back_wall->set_name( "back_wall" );
  back_wall->set_material( whitemat );
  group->add_primitive( back_wall );
  
  //right wall

  primitive* right_wall = new plane( tiny_vec<float, 3>( 1.0, 0, 0 ), 0.0f );
  right_wall->set_name( "right_wall" );
  right_wall->set_material( greenmat );
  group->add_primitive( right_wall );
  
  //left wall

  primitive* left_wall = new plane( tiny_vec<float, 3>( -1.0, 0, 0 ), -551.2f );
  left_wall->set_name( "left_wall" );
  left_wall->set_material( redmat );
  group->add_primitive( left_wall );
  
  // small block (original block is 165,723mm x 165,0mm x 167,045mm  (BxHxT))
  primitive* small_block = new box( tiny_vec<float, 3>( -82.8615, 0.0, -83.5225 ), tiny_vec<float, 3>( 82.8615, 165.0, 83.5225 ) );
  small_block->set_name( "small_block" );
  small_block->set_material( whitemat );
  
  transformation_group* t1 = new transformation_group();
  t1->set_transformation( translate_44<float>( 185.5, 0.0, 169 ) * rotatey_44<float>( -21.46 ) );
  t1->add_primitive( small_block );
  group->add_primitive( t1 );
  
  // tall block (original block is 165,424mm x 330mm x 167,335mm  (BxHxT))

  primitive* tall_block = new box( tiny_vec<float, 3>( -82.712, 0.0, -83.6675 ), tiny_vec<float, 3>( 82.712, 330.0, 83.6675 ) );
  tall_block->set_name( "tall_block" );
  tall_block->set_material( whitemat );
  
  transformation_group* t2 = new transformation_group();
  t2->set_transformation( translate_44<float>( 368.5, 0.0, 351.25 ) * rotatey_44<float>( 17.23 ) );
  t2->add_primitive( tall_block );
  group->add_primitive( t2 );
  
  return myscene;
}