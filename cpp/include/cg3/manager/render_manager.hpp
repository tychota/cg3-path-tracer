# pragma once

# include "cg3/common/scene.hpp"
# include "cg3/common/renderer.hpp"
# include "cg3/manager/tile.hpp"
# include "cg3/manager/tiled_image.hpp"
# include "cg3/manager/strategy.hpp"

# include <map>
# include <memory>
# include <thread>
# include <mutex>
# include <atomic>

# if defined USE_RENDERMANAGER_SIGNALS
  # include <boost/signals2/signal.hpp>
# endif

class RenderManager
{
  public:
    RenderManager( tiny_vec< std::size_t, 2 > resolution
                  , std::shared_ptr< Renderer >
                  , std::shared_ptr< Strategy >
                  , std::size_t num_threads
                  );

    void render_image();

    void send_cancel_request();

    Tile::image_type get_image() const;

    # if defined USE_RENDERMANAGER_SIGNALS
      boost::signals2::signal< void ( Tile const ) >             signal_tile_completed;
      boost::signals2::signal< void ( Tile::image_type const ) > signal_image_completed;
    # endif

    tiny_vec< std::size_t, 2 > get_resolution() const { return _resolution; }

  protected:
    bool next_tile( Tile& t );

    void render_tile( Tile& t );

    void compose_tile( Tile const& t );

  private:
    std::atomic< bool > _cancel_request;
    std::shared_ptr< Renderer > _renderer;

    std::shared_ptr< Strategy > _strategy;
    std::vector< Tile > _tile_pool;
    mutable std::mutex  _tile_pool_mutex;

    tiny_vec< std::size_t, 2 > _resolution;
    TiledImage _image;
    mutable std::mutex _image_mutex;

    std::size_t _num_threads;
};
