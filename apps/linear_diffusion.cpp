# include <iostream>
# include <istream>
# include <fstream>
# include <iomanip>

# include <vector>
# include <algorithm>
# include <cmath>
# include <limits>

using namespace std;

// with reflecting boundary
class Levelset
{
  public:
    Levelset( size_t m, size_t n) : _size_m( m ), _size_n( n ), _heights( (m+2)*(n+2), 0. ) {}

    size_t size_m() const { return _size_m; }
    size_t size_n() const { return _size_n; }

    vector< double > const& height_vector() const { return _heights; }

    // Get reference to value from grid location (i,j). i = 0,...,size_m+1  j = 0,...,size_n+1.
    // If i = 0 or i = size_m+1 or j = 0 or j = size_n+1 the point is part of the reflecting boundary
    double &      operator() ( size_t const i, size_t const j )       { return z( i, j ); }
    double const& operator() ( size_t const i, size_t const j ) const { return z( i, j ); }

    // Creates an extra one-pixel "reflecting boundary" (Neumann boundary condition) by copying neighboring image boundary pixels.
    // This ensures that derivatives at image boundaries vanish. 
    // With zero derivatives, no diffusive flux will leave the image so that the average greyvalue is preserved. 
    void update_reflecting_boundary()
    {
      // corners
      z( 0, 0         ) = z( 1, 1       );
      z( 0, _size_n+1 ) = z( 1, _size_n );

      z( _size_m+1, 0         ) = z( _size_m, 1       );
      z( _size_m+1, _size_n+1 ) = z( _size_m, _size_n );

      // edges
      for( size_t i = 1; i <= _size_m; i++ ) 
      {
        z( i, 0         ) = z( i, 1       );
        z( i, _size_n+1 ) = z( i, _size_n ); 
      }

      for( size_t j = 1; j <= _size_n; j++ ) 
      { 
        z(         0, j ) = z(       1, j ); 
        z( _size_m+1, j ) = z( _size_m, j ); 
      }
    }

    // compute the value of the discretized laplacian at grid location (i,j).
    double const  laplacian ( size_t i, size_t j ) const
    {
      return     z( i-1, j ) + z( i, j-1 )
           +     z( i+1, j ) + z( i, j+1 )
           - 4 * z( i, j );
    }

    // apply one step of explicit integration of the diffusion equation.
    void apply_explicit_linear_diffusion_step( double const& ts )
    {
      double const d = 1;

      Levelset result = *this/*( size_m(), size_n() )*/;
      for( size_t i = 1; i <= _size_m; i++ )
        for( size_t j = 1; j <= _size_n; j++ )
          result( i, j ) = z( i, j ) + d * ts * laplacian( i, j );

      _heights = move( result.height_vector() );

      update_reflecting_boundary();
    }

    // print average, minimum and maximum greyvalue of the levelset.
    void print_stats() const
    {
      double sum = 0.;
      double mi = numeric_limits< double >::max();
      double ma = numeric_limits< double >::min();

      for( size_t i = 1; i <= _size_m; i++ )
      {
        for( size_t j = 1; j <= _size_n; j++ )
        {
          mi = min( mi, z( i, j ) );
          ma = max( ma, z( i, j ) );
          sum += z( i, j );
        }
      }

      clog << "avg " << ( sum / ( size_m() * size_n() ) )
           << " min "<< mi << " max " << ma << endl;
    }

  private:
    // Get reference to value at grid location (i,j). i = 0,...,size_m+1  j = 0,...,size_n+1.
    // If i = 0 or i = size_m+1 or j = 0 or j = size_n+1 the point is part of the reflecting boundary
    double      & z ( size_t const& i, size_t const& j )       { return _heights[ i * (_size_n+2) + j ]; }
    double const& z ( size_t const& i, size_t const& j ) const { return _heights[ i * (_size_n+2) + j ]; }

    size_t _size_m;
    size_t _size_n;
    vector< double > _heights;
};

Levelset read_levelset_from_pgm( ifstream& file )
{
  string token;
  if( file >> token && token == "P2" )
  {
    file.ignore( 10, '\n' );
    while( file.peek() == '#' ) file.ignore( numeric_limits<streamsize>::max(), '\n' );

    int size_m, size_n;
    double max_grey_value;
    if( file >> size_n >> size_m >> max_grey_value )
    {
      Levelset result( size_m, size_n );

      for( size_t i = 1; i <= size_m; i++ )
        for( size_t j = 1; j <= size_n; j++ )
          if( file >> result( i,j ) ) // read pixel value
            result( i,j ) /= max_grey_value; // normalize pixel value
          else
            cerr << "Could not read pixel (" << i << ", " << j << ")." << endl;

      result.update_reflecting_boundary();

      return result;
    }
  }
  return Levelset( 0, 0 );
}

void write_levelset_to_pgm( Levelset const& l, string const& filename, string const& comment = "", size_t const max_grey_value = 255 )
{
  ofstream file( filename );

  if( file and file << "P2" << endl )
  {
    file << "# " << comment << endl;

    if( file and file << (l.size_n()+2) << " " << (l.size_m()+2) << endl  )
    {
      // maximum grey value
      file << max_grey_value << endl;

      for( size_t i = 0; i <= l.size_m()+1; i++ )
      {
        for( size_t j = 0; j <= l.size_n()+1; j++ )
        {
          if( not ( file << int( round( l( i,j ) * max_grey_value ) ) << ' ' ) )
            cerr << "Could not write pixel (" << i << "," << j << ")." << endl;
        }
        file << endl;
      }
    }
  }
  file.close();
}

void write_levelset_to_txt( Levelset const& l, string const& filename, string const& comment = "", size_t const max_grey_value = 255 )
{
  ofstream file( filename );

  if( file and file << "P2" << endl )
  {
    for( size_t i = 0; i <= l.size_m()+1; i++ )
    {
      for( size_t j = 0; j <= l.size_n()+1; j++ )
      {
        if( not ( file << l( i,j ) << ' ' ) )
          cerr << "Could not write pixel (" << i << "," << j << ")." << endl;
      }
      file << endl;
    }
  }
  file.close();
}


int main( int argc, char** argv )
{
  string const init_filename = "diffusion_input.pgm";
  ifstream init_file( init_filename.c_str() );
  Levelset l = read_levelset_from_pgm( init_file );
  l.print_stats();
  write_levelset_to_pgm( l, "in" );

  double const time_step = 0.1;
  for( size_t i = 0; i < 2001; i++ )
  { 
    l.apply_explicit_linear_diffusion_step( time_step );

    l.print_stats();

    if( i % 200 == 0 )
    { 
      write_levelset_to_pgm( l, "out" + to_string( i ) + ".pgm" );
    }
  }

  return 0;
}
