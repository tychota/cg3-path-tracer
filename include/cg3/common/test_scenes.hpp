# pragma once

# include "cg3/common/scene.hpp"

# include <memory>

std::shared_ptr< Scene > create_rt_test_scene( tiny_vec< size_t, 2 > resolution );

std::shared_ptr< Scene > create_pt_test_scene( tiny_vec< size_t, 2 > resolution );

std::shared_ptr< Scene > create_cornell_box_test_scene( tiny_vec< size_t, 2 > resolution );
