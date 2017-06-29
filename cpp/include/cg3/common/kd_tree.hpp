# pragma once
# include "cg3/common/primitive_group.hpp"
# include "cg3/common/box.hpp"

# ifdef _MSC_VER
  # pragma warning (disable: 4996)
# endif


//kd_tree_group
class kd_tree : public primitive_group
{
  public:
    //abstract class of a node of a kd-tree.
    class kd_node
    {
      public:
        virtual ~kd_node(){}
        virtual bool is_leaf() const=0;
    };

    //root node of the tree
    kd_node* root;
    // bounding box of the whole tree
    box bounds;
    //The maximum depth of the tree
    std::size_t max_depth;
    //The minimum number of primitives per leaf node.
    std::size_t min_size;

  public:
    kd_tree( std::size_t max_depth = 25, std::size_t  min_size = 4 );

    bool add_obj_mesh( std::string const& filename, bool scale = true );

    void add_primitive( primitive* p );

    void clear();

    void build_tree_spatial_median();

    ~kd_tree();

    // kD-tree traversal algorithm TA_rec_B by Vlastimil Havran
    // see Havran's dissertation thesis (Nov 2000), Appendix C

    virtual bool closest_intersection( intersection_info* hit, si::length min_lambda );
    
    virtual bool any_intersection( ray& r, si::length min_lambda, si::length max_lambda );

  protected:
    kd_node* build_tree_spatial_median(  std::vector<primitive*> &primitives, box &bounds, unsigned int depth );
};
