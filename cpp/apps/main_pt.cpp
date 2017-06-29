# include "cg3/manager/render_manager.hpp"
# include "cg3/pathtracer/pathtracer.hpp"

# include "cg3/common/test_scenes.hpp"
# include "cg3/common/image_io.hpp"

using namespace std;

int main(int argc, char** argv) 
{
  tiny_vec< size_t, 2 > resolution{ size_t(512), size_t(512) };

  //auto my_scene = create_pt_test_scene( resolution );
  auto my_scene = create_cornell_box_test_scene( resolution );

  shared_ptr< Pathtracer > tracer( new Pathtracer( my_scene ) );

  shared_ptr< Strategy > tiling( new Strategy( { size_t(4), size_t(4) }, 32 ) );

  RenderManager manager( resolution, tracer, tiling, 8 );

  manager.render_image();

  cout << "writing image...";
  write_bmp( manager.get_image(), "image_pt.bmp" );
  cout << "ready" << endl;

  return 0;
}
