#include "cg3/common/kd_tree.hpp"

#include "cg3/common/box.hpp"
#include "cg3/common/triangle.hpp"
#include "cg3/common/obj_reader.hpp"
#include "cg3/common/blinn_phong.hpp"
#include "cg3/common/timing.hpp"

using namespace std;

//class of an inner node of a kd-tree.
class kd_split : public kd_tree::kd_node
{
private:
	
	//left child node of the current node.
	kd_node* left;
	//right child node of the current node.
	kd_node* right;
	//split axis. Values 0, 1 and 2 refer to the x-,y- and z-axis respectively.
	const int axis;
	//position of the split plane on the split axis.
	const float location;

public:
	//constructor
	kd_split(kd_node* lchild, kd_node* rchild, int axis, float location)
		    : left(lchild),right(rchild),axis(axis),location(location)
    {
		
	}

	//destructor
    virtual ~kd_split()
	{
		delete left; 
		delete right; 
	}
          
	int get_split_axis() const { return axis; }
	
	float get_split_location() const { return location; }
   
	//Function for getting the left child node.
	kd_node* lchild() { return left; }
    //Function for getting the right child node.
	kd_node* rchild() { return right; }
	//Function for determining if the node is a leaf node.
    virtual bool is_leaf() const { return false; }

};

	//Class of a leaf node of a kd-tree.
class kd_leaf : public kd_tree::kd_node
{
private:
	//Number of primitives in the leaf node.
    std::size_t size;
	//List of all primitives in the leaf node.
    primitive** prim_list;

public:	
	kd_leaf(const std::vector<primitive*> &primitives)
	{
		size = primitives.size();
		prim_list = new primitive*[size];
		for(unsigned int i=0; i<size; i++)
			prim_list[i]=primitives[i];
             
	}

	
	~kd_leaf()
	{
		if(prim_list != NULL)
			delete [] prim_list;
	}

	
	virtual bool is_leaf() const { return true; }

	primitive** get_primitives() const { return prim_list; }

	size_t get_prim_list_size() const { return size; }

};

kd_tree::kd_tree( size_t max_depth, size_t min_size)
: root( nullptr )
, max_depth( max_depth )
, min_size( min_size )
{}

bool kd_tree::add_obj_mesh( std::string const& filename, bool scale)
{
  blinn_phong* meshmat = new blinn_phong();
  meshmat->set_diffuse(  .7f,.7f,.7f);
  meshmat->set_specular( .7f,.7f,.7f);
  meshmat->set_shininess( 100 );

  obj_reader reader;
  if(reader.read_obj(filename))
  {
    auto v1 = tiny_vec<float,3>(4,-2,4);
    auto v2 = tiny_vec<float,3>(8,6,8);

    if( scale )
      reader.rescale( v1,v2 );

    triangle *tri;
    for(unsigned i = 0; i < reader.faces.size(); i++)
    {
      if( reader.faces[i].vertex_indices.size() == 3 )
      {
          if( reader.faces[i].normal_indices.size() == 3 )
          {
            tri = new triangle( reader.vertices[reader.faces[i].vertex_indices[0]],
                                reader.vertices[reader.faces[i].vertex_indices[1]],
                                reader.vertices[reader.faces[i].vertex_indices[2]],
                                reader.normals[reader.faces[i].normal_indices[0]],
                                reader.normals[reader.faces[i].normal_indices[1]],
                                reader.normals[reader.faces[i].normal_indices[2]] );
          }
          else
          {
            tri = new triangle( reader.vertices[reader.faces[i].vertex_indices[0]],
                                reader.vertices[reader.faces[i].vertex_indices[1]],
                                reader.vertices[reader.faces[i].vertex_indices[2]] );
          }
          tri->set_material( meshmat );
          add_primitive( tri );
        }
    }
    std::cout << "ready\n";
    reader.show_stats();
    std::cout << "center: "<<bounds.get_center() << " extends:" <<bounds.get_extends()<<std::endl;
  }
  else 
  {
    return false;
  }

  return true;
}

void kd_tree::add_primitive( primitive* p )
{
  auto bb = p->calc_bounds();
  bounds.extend(bb);
  primitives.push_back(p);
}

void kd_tree::clear()
{
  primitive_group::clear();
  if(root)
      delete root;
  root = nullptr;
}

void kd_tree::build_tree_spatial_median()
{
  std::cout << "building kdtree...";
  tic();
  root = build_tree_spatial_median(primitives,bounds,0);
  std::cout << "ready\n";
  toc();
}

kd_tree::kd_node* kd_tree::build_tree_spatial_median(  std::vector<primitive*> &primitives, box &bounds, unsigned int depth )
{
  if( depth >= max_depth || primitives.size() <= min_size ) 
  {
      return (kd_node*)(new kd_leaf(primitives));
  }

    // find split axis
  tiny_vec<float,3> extends = bounds.get_extends();

  int axis = depth%3;



    // find split location
  
  float location = 0.5f * (bounds.get_min()[axis] + bounds.get_max()[axis]);
  

    // create child bounds
    box lbounds = bounds;
    lbounds.get_max()[axis] = location;
    box rbounds = bounds;
    rbounds.get_min()[axis] = location;

  // create child primitive lists
    std::vector<primitive*> lprim, rprim;
  for( std::vector<primitive*>::const_iterator it = primitives.begin();
          it != primitives.end(); it++ )
    {
        std::pair< position<3>, position<3> > child_bounds = (*it)->calc_bounds();

        if( child_bounds.first[axis] < location ) 
                    lprim.push_back(*it);
        if( child_bounds.second[axis] > location ) 
                    rprim.push_back(*it);

            if( child_bounds.first[axis]==location &&
                    child_bounds.second[axis]==location ) 
            {
                  lprim.push_back(*it);
                  rprim.push_back(*it);
          }

    }

  return new kd_split( build_tree_spatial_median( lprim, lbounds, depth+1 ),
                                  build_tree_spatial_median( rprim, rbounds, depth+1 ),axis, location 
                     );
}


	kd_tree::~kd_tree()
	{
		clear();		
	}

	
 
 // kD-tree traversal algorithm TA_rec_B by Vlastimil Havran
 // see Havran's dissertation thesis (Nov 2000), Appendix C

//Structure of a stack element.
struct stack_entry
{
	kd_tree::kd_node* _node; 
    tiny_vec<float,3> pos;  
    float lambda;        
    int prev;       
	stack_entry(kd_tree::kd_node* n, tiny_vec<float,3> pos, float t) : _node(n),pos(pos),lambda(t) { }
    stack_entry() { }
};

bool kd_tree::closest_intersection(intersection_info* hit,float min_lambda )
{
  // check if ray can hit the Scene at all
  std::pair<float,float> tt = bounds.intersect( hit->incoming_ray() );
  if( tt.first > tt.second) return false;

  float tnear = tt.first;
  float tfar = tt.second;
  float tsplit;

  kd_node* ffar;
  kd_node* current = root;

  tiny_vec<float,3> orig = hit->incoming_ray().origin();
  tiny_vec<float,3> dir = hit->incoming_ray().dir();
  tiny_vec<float,3> inv_dir = hit->incoming_ray().inv_dir();

  // add entry point to stack
  int entry = 0;
  stack_entry stack[50];
  stack[entry].lambda = tnear;

  // distinguish between internal and external origin
  if( tnear >= min_lambda)
  {
      stack[entry].pos = orig + tnear * dir;
      
  }
  else
      stack[entry].pos = orig + min_lambda * dir;

  // add exit point to stack
  int exit = 1;

  if( tfar > hit->lambda() )
    tfar = hit->lambda();
  stack[exit].lambda = tfar;
  stack[exit].pos = orig + tfar * dir;
  stack[exit]._node = nullptr;




  // traverse the tree until ray hits object or leaves the Scene
  while( current != nullptr )
  {
    // traverse until a leaf is found
    while( !current->is_leaf() )
    {
      float split = ((kd_split*)current)->get_split_location();
      int axis = ((kd_split*)current)->get_split_axis();

      if( stack[entry].pos[axis] <= split )
      {
        if( stack[exit].pos[axis] <= split )
        {
            current = ((kd_split*)current)->lchild();
            continue;
        }
        if(stack[exit].pos[axis] == split)
        {
            current = ((kd_split*)current)->rchild();
            continue;
        }
        ffar = ((kd_split*)current)->rchild();
        current = ((kd_split*)current)->lchild();
      }
      else
      {
        if( split < stack[exit].pos[axis] )
        {
              current = ((kd_split*)current)->rchild();
              continue;
        }
        ffar = ((kd_split*)current)->lchild();
        current = ((kd_split*)current)->rchild();
      }

      tsplit = (split - orig[axis]) * inv_dir[axis];

      int tmp = exit;
      exit++;
      if(exit==entry) exit++;
      stack[exit].prev = tmp;
      stack[exit].lambda = tsplit;
      stack[exit]._node = ffar;
      stack[exit].pos = orig + tsplit * dir;
      stack[exit].pos[axis] = split;
    }
    //current node is leaf node

    si::length t_old = hit->lambda();
    hit->set_lambda( min(stack[exit].lambda,hit->lambda()) );

    // now a leaf has been found
    // intersect with all primitives in leaf node and ignore intersections
    // where the hitpoint is not between the entry and the exit point.
    bool hitx = false;
    primitive** prim = ((kd_leaf*)current)->get_primitives();
    for( int i=0; i<(int)((kd_leaf*)current)->get_prim_list_size(); i++ )
    {
      bool bt = prim[i]->closest_intersection( hit, stack[entry].lambda );
      if(bt)
        hitx = bt;
    }

    if( hitx )
    {
        return true;
    }

    hit->set_lambda( t_old );
    entry = exit;

    current = (kd_node*) stack[exit]._node;
    exit = stack[entry].prev;
  }

  return false;
}

 bool kd_tree::any_intersection( ray & r, si::length min_lambda, si::length max_lambda )
{
// check if ray can hit the Scene at all
  std::pair< si::length, si::length > tt = bounds.intersect(r);
  if( tt.first > tt.second) return false;

  si::length tnear = tt.first;
  si::length tfar = tt.second;
  si::length tsplit;

  kd_node* ffar;
  kd_node* current = root;

  position<3> orig = r.origin();
  direction<3> dir = r.dir();
  direction<3> inv_dir = r.inv_dir();

  // add entry point to stack
  int entry = 0;
  stack_entry stack[50];
  stack[entry].lambda = tnear;

  // distinguish between internal and external origin
  if( tnear >= 0)
      stack[entry].pos = orig + tnear*dir;		
  else
      stack[entry].pos = orig + min_lambda*dir;

  // add exit point to stack
  int exit = 1;

  if( tfar > max_lambda )
    tfar =max_lambda;
  stack[exit].lambda = tfar;
  stack[exit].pos = orig + tfar*dir;
  stack[exit]._node = NULL;

    // traverse the tree until ray hits object or leaves the Scene
  while( current != nullptr )
  {
    // traverse until a leaf is found
    while( ! current->is_leaf() )
    {
      si::length split = ((kd_split*)current)->get_split_location();
      int axis = ((kd_split*)current)->get_split_axis();

      if( stack[entry].pos[axis] <= split )
      {
        if( stack[exit].pos[axis] <= split )
        {
            current = ((kd_split*)current)->lchild();
            continue;
        }
        if(stack[exit].pos[axis] == split)
        {
            current = ((kd_split*)current)->rchild();
            continue;
        }
        ffar = ((kd_split*)current)->rchild();
        current = ((kd_split*)current)->lchild();
      }
      else
      {
        if( split < stack[exit].pos[axis] )
        {
              current = ((kd_split*)current)->rchild();
              continue;
        }
        ffar = ((kd_split*)current)->lchild();
        current = ((kd_split*)current)->rchild();
      }

      tsplit = (split - orig[axis]) * inv_dir[axis];

      int tmp = exit;
      exit++;
      if(exit==entry) exit++;
      stack[exit].prev = tmp;
      stack[exit].lambda = tsplit;
      stack[exit]._node = ffar;
      stack[exit].pos = orig + tsplit * dir;
      stack[exit].pos[axis] = split;
    }
    //current node is leaf node

    // now a leaf has been found
    // intersect with all primitives in leaf node and ignore intersections
    // where the hitpoint is not between the entry and the exit point.

    primitive** prim = ((kd_leaf*)current)->get_primitives();
    for( int i=0; i<(int)((kd_leaf*)current)->get_prim_list_size(); i++ )
    {
      if( prim[i]->any_intersection( r, min_lambda, max_lambda ) )
          return true;
    }

    entry = exit;

    current = (kd_node*) stack[exit]._node;
    exit = stack[entry].prev;
  }

  return false;
}

