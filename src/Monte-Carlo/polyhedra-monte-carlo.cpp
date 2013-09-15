#include <stdio.h>
#include <time.h>
#include <cassert>
#include <math.h>
#include <stdlib.h>
#include <popt.h>
#include "handymath.h"
#include "vector3d.h"
#include "Monte-Carlo/polyhedra.h"


// -----------------------------------------------------------------------------
// Notes on conventions and definitions used
// -----------------------------------------------------------------------------
//
// All coordinates are cartesion.
//
// The coordinates x, y, z are always floating point numbers that refer to real
// locations within the cell.
//
// The coordinates x_i, y_i, z_i are always integers that refer to the bin number
// of the respective coordinate such that x_i refers to a bin of thickness dx
// centered at x.
//
// The symbols e, e_i, and de are used as general coordinates.
//
// Def: Iff two objects, a and b, are closer than a.R + b.R + neighborR + dn,
// then they are neighbors.
//
// Neighbors are used to drastically reduce the number of collision tests needed.
//
// Def: The neighborsphere of an object, a, is the sphere within which
// everything is a neighbor of a.
// Note that this sphere has a well defined center, but it does not have
// a well defined radius unless all obects are circumscribed by spheres of
// the same radius, but this does not affect anything.


// -----------------------------------------------------------------------------
// Global Constants
// -----------------------------------------------------------------------------

const poly_shape cube("cube");
const poly_shape tetrahedron("tetrahedron");
const poly_shape truncated_tetrahedron("truncated_tetrahedron");

// prints out a lot of extra information and performs extra computation
// should be false except when something is wrong
// NOTE: This can slow things down VERY much, depending on how much debug
// code is active
const bool debug = false;



// for debugging purposes
bool testcase = false;

// -----------------------------------------------------------------------------
// Functions
// -----------------------------------------------------------------------------

// States how long it's been since last took call.
static void took(const char *name);

// Saves the vertices of all polyhedra to a file.
inline void save_locations(const polyhedron *p, int N, const char *fname,
                           const double len[3], const char *comment="");

// Generates all of the figures for the talk instead of performing a normal run
// returns 0 unless there's an error
inline int generate_talk_figs();

// The following functions only do anything if debug is true:

// Prints the locations, rotations, radii, and shapes of every polyhedron
// As well as if any overlap or are outside the cell.
inline void print_all(const polyhedron *p, int N, double periodic[3]);

// Same as print_all, but only prints information for one polyhedron,
// and does not do overlap tests.
inline void print_one(const polyhedron &a, int id, const polyhedron *p, int N,
                      double periodic[3]);

// Only print those shapes that overlap or are outside the cell
// Also prints those they overlap with
inline void print_bad(const polyhedron *p, int N, double periodic[3]);

// Checks to make sure that every polyhedron is his neighbor's neighbor.
inline void check_neighbor_symmetry(const polyhedron *p, int N);

int main(int argc, const char *argv[]) {
  took("Starting program");
  // -----------------------------------------------------------------------------
  // Define "Constants" -- set from arguments then unchanged
  // -----------------------------------------------------------------------------
  double periodic[3] = {0, 0, 0};
  double walls[3] = {0, 0, 0};
  bool fake_walls = false;
  unsigned long int seed = 0;
  int vertex_period = 0;
  char *shape_name = new char[1024];
  sprintf(shape_name, "truncated_tetrahedron");
  const poly_shape *shape = &truncated_tetrahedron;

  char *dir = new char[1024];
  sprintf(dir, "papers/polyhedra/figs/mc");
  char *filename = new char[1024];
  sprintf(filename, "[walls/periodic]-FF");
  char *structure = new char[1024];
  sprintf(structure, "ice");
  int N = 0;
  long iterations = 100000000000;
  long initialize_iterations = 50000;
  double R = 1;
  double neighborR = 0.5;
  double dr = 0.01;
  double de_density = 0.01;
  int totime = 0;
  bool talk = false;
  // scale and theta_scale aren't quite "constants" -- they are adjusted
  // during the initialization so that we have a reasonable acceptance rate
  double scale = 0.05;
  double theta_scale = 0.05;

  poptContext optCon;
  // ---------------------------------------------------------------------------
  // Set values from parameters
  // ---------------------------------------------------------------------------
  poptOption optionsTable[] = {
    {"N", 'N', POPT_ARG_INT, &N, 0, "Number of polyhedra to simulate", "N"},
    {"periodx", '\0', POPT_ARG_DOUBLE, &periodic[0], 0, "Periodic in x", "lenx"},
    {"periody", '\0', POPT_ARG_DOUBLE, &periodic[1], 0, "Periodic in y", "leny"},
    {"periodz", '\0', POPT_ARG_DOUBLE, &periodic[2], 0, "Periodic in z", "lenz"},
    {"wallx", '\0', POPT_ARG_DOUBLE, &walls[0], 0, "Walls in x", "lenx"},
    {"wally", '\0', POPT_ARG_DOUBLE, &walls[1], 0, "Walls in y", "leny"},
    {"wallz", '\0', POPT_ARG_DOUBLE, &walls[2], 0, "Walls in z\n", "lenz"},
    {"fake_walls", 'r', POPT_ARG_NONE, &fake_walls, 0,
     "Will cause collisions to occur with walls based on centers only", 0},
    {"iterations", 'i', POPT_ARG_LONG | POPT_ARGFLAG_SHOW_DEFAULT, &iterations, 0,
     "Number of iterations to run for", "iter"},
    {"initialize_iterations", '\0', POPT_ARG_LONG | POPT_ARGFLAG_SHOW_DEFAULT, &initialize_iterations, 0, "Number of iterations to run the initialization for", "iter"},
    {"filename", 'f', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &filename, 0,
     "Base of filename. Many files will be generated, and will automatically contain \
information on the number and type of polyhedra, as well as file extensions", "name"},
    {"dir", 'd', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &dir, 0,
     "Directory to save to", "dir"},
    {"shape", '\0', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &shape_name, 0,
     "Type of polyhedra to use. Can be one of [cube | tetrahedron | truncated_tetrahedron]",
     "shape"},
    {"R", 'R', POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &R, 0,
     "Size of the sphere that circumscribes each polyhedron", "R"},
    {"neighborR", '\0', POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &neighborR, 0,
     "Neighbor radius, used to drastically reduce collision detections", "neighborR"},
    {"dr", '\0', POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &dr, 0,
     "Differential radius change used in pressure calculation", "dr"},
    {"de_density", '\0', POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &de_density, 0,
     "Resolution of density file", "de"},
    {"scale", '\0', POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &scale, 0,
     "Standard deviation for translations of polyhedra", "scale"},
    {"theta_scale", '\0', POPT_ARG_DOUBLE | POPT_ARGFLAG_SHOW_DEFAULT, &theta_scale, 0,
     "Standard deviation for rotations of polyhedra", "theta"},
    {"seed", 's', POPT_ARG_LONG | POPT_ARGFLAG_SHOW_DEFAULT, &seed, 0,
     "Seed for the random number generator", "seed"},
    {"save_vertices", 'v', POPT_ARG_INT, &vertex_period, 0,
     "Periodically saves the vertex locations of the polyhedra, where period is a number \
of iterations", "period"},
    {"time", 't', POPT_ARG_INT, &totime, 0,
     "Timing information will be displayed", "interval"},
    {"talk", '\0', POPT_ARG_NONE, &talk, 0,
     "Generate figures for talk. Will not perform a normal run."},
    {"structure", '\0', POPT_ARG_STRING | POPT_ARGFLAG_SHOW_DEFAULT, &structure, 0,
    "Structure to use for truncated tetrahedra. Does nothing for other shapes. \
Can be one of [ice | arsenic]"},
    POPT_AUTOHELP
    POPT_TABLEEND
  };
  optCon = poptGetContext(NULL, argc, argv, optionsTable, 0);
  poptSetOtherOptionHelp(optCon, "[OPTION...]\nNumber of polyhedra and \
periodicity/dimensions are not set by default and are required.\n");

  int c = 0;
  // go through arguments, set them based on optionsTable
  while((c = poptGetNextOpt(optCon)) >= 0);
  if (c < -1) {
    fprintf(stderr, "\n%s: %s\n", poptBadOption(optCon, 0), poptStrerror(c));
    return 1;
  }
  poptFreeContext(optCon);

  if (talk) {
    printf("Generating data for polyhedra talk figures.\n");
    return generate_talk_figs();
  }
  // ---------------------------------------------------------------------------
  // Verify we have reasonable arguments and set secondary parameters
  // ---------------------------------------------------------------------------
  const bool real_walls = !fake_walls;
  const double len[3] = {periodic[0] + walls[0], periodic[1] + walls[1],
                         periodic[2] + walls[2]};
  if (strcmp(shape_name, "cube") == 0) shape = &cube;
  else if (strcmp(shape_name, "tetrahedron") == 0) shape = &tetrahedron;
  else if (strcmp(shape_name, "truncated_tetrahedron") == 0) shape = &truncated_tetrahedron;
  else {
    fprintf(stderr, "\nInvalid shape.\n");
    return 1;
  }
  if (N <= 0 || iterations < 0 || R <= 0 || neighborR <= 0 || dr <= 0 || scale < 0 ||
      theta_scale < 0 || vertex_period < 0) {
    fprintf(stderr, "\nAll parameters must be positive.\n");
    return 1;
  }
  for(int i=0; i<3; i++) {
    const char dim = i == 0 ? 'x' : i == 1 ? 'y' : 'z';
    if (periodic[i] != 0 && walls[i] != 0) {
      fprintf(stderr, "\nThe cell can't both be periodic and have walls in the %c dimension!\n", dim);
      return 1;
    }
    if (periodic[i] + walls[i] <= 0) {
      fprintf(stderr, "\nThe cell needs to have some size in the %c dimension!\n", dim);
      return 1;
    }
  }
  const double eta = (double)N*shape->volume*R*R*R/len[0]/len[1]/len[2];
  if (eta > 1) {
    fprintf(stderr, "\nYou're trying to cram too many polyhedra into the cell. They will never fit. Filling fraction: %g\n", eta);
  }
  // If a filename was not selected, make a default
  if (strcmp(filename, "[walls/periodic]-FF") == 0) {
    if (walls[0] + walls[1] + walls[2] > 0) // has walls in some dimension
      sprintf(filename, "walls-%04.2f", eta);
    else
      sprintf(filename, "periodic-%04.2f", eta);
    printf("\nNo filename selected, so using the default: %s\n", filename);
  }
  printf("----------------------------------------------------------------------\n");
  printf("Running %s with parameters:\n", argv[0]);
  int cond = 0;
  for(int i=1; i<argc; i++) {
    if(argv[i][0] == '-') printf("\n");
    printf("%s ", argv[i]);
  }
  printf("\n");
  if (totime > 0) printf("Timing information will be displayed.\n");
  if (debug) printf("DEBUG MODE IS ENABLED!\n");
  else printf("Debug mode disabled\n");
  printf("----------------------------------------------------------------------\n\n");

  // ---------------------------------------------------------------------------
  // Define variables
  // ---------------------------------------------------------------------------
  const int density_bins = round((len[0] + len[1] + len[2])/de_density);
  long *density_histogram = new long[3*density_bins]();
  polyhedron *polyhedra = new polyhedron[N];
  long dZ = 0;

  // Initialize the random number generator with our seed
  random::seed(seed);

  // ---------------------------------------------------------------------------
  // Set up the initial grid of polyhedra
  // ---------------------------------------------------------------------------
  // Approximate the maximum number of neighbors a polyhedron could have
  // hokey guess, but it should always be high enough
  const double neighbor_sphere_vol = 4.0/3.0*M_PI*uipow(2.5*R+neighborR, 3) - shape->volume;
  int max_neighbors = min(N, neighbor_sphere_vol / shape->volume);

  for(int i=0; i<N; i++) {
    polyhedra[i].mypoly = shape;
    polyhedra[i].R = R;
  }
  int nx = ceil(pow((double)N*len[0]*len[0]/len[1]/len[2], 1.0/3.0));
  int ny = ceil((double)len[1]/len[0]*nx);
  int nz = ceil((double)len[2]/len[0]*nx);
  while (nx*ny*nz < N) {
    nx ++;
    ny ++;
    nz ++;
  }
  const double xspace = len[0]/double(nx);
  const double yspace = len[1]/double(ny);
  const double zspace = len[2]/double(nz);
  printf("Setting up grid with polyhedra: (%i, %i, %i) and space: (%g, %g, %g)\n",
         nx, ny, nz, xspace, yspace, zspace);
  int max_attempts = 1;
  if (shape == &cube) {
    bool *spot_used = new bool[nx*ny*nz]();
    for(int i=0; i<N; i++) {
      int x, y, z;
      do {
        x = floor(random::ran()*nx);
        y = floor(random::ran()*ny);
        z = floor(random::ran()*nz);
      } while(spot_used[x*ny*nz + y*nz + z]);
      spot_used[x*ny*nz + y*nz + z] = true;

      polyhedra[i].pos[0] = (x + 0.5)*xspace;
      polyhedra[i].pos[1] = (y + 0.5)*yspace;
      polyhedra[i].pos[2] = (z + 0.5)*zspace;
    }
    delete[] spot_used;
  }
  else if (shape == &truncated_tetrahedron) {
    if (strcmp(structure, "arsenic") == 0) {
      // form a lattice that will be able to give us the maximum filling fraction
      // which is 207/208
      // Based on the paper Crystalline Assemblies... by Damasceno et al.

      const double fac = R/sqrt(11.0)/3.0*1.1;
      // lattice vectors:
      const vector3d e1 = vector3d(-4, 8, 12)*fac;
      const vector3d e2 = vector3d(12, -4, 8)*fac;
      const vector3d e3 = vector3d(8, 12, -4)*fac;

      const vector3d offset = vector3d(6, 6, 6)*fac;
      nx *= 2;
      ny *= 2;
      nz *= 2;
      bool *spot_used = new bool[nx*ny*nz]();
      int x=0, y=0, z=0;
      for(int i=0; i<N; i+=2) {
        do {
          do {
            x = floor(random::ran()*nx);
            y = floor(random::ran()*ny);
            z = floor(random::ran()*nz);
          } while (spot_used[x*ny*nz + y*nz + z]);
          spot_used[x*ny*nz + y*nz + z] = true;

          x -= nx/2;
          y -= ny/2;
          z -= nz/2;
          polyhedra[i].pos = offset + x*e1 + y*e2 + z*e3;
          if (i < N-1) {
            polyhedra[i+1].pos = x*e1 + y*e2 + z*e3;
            polyhedra[i+1].rot = rotation(M_PI, vector3d(1, 1, 0));
          }
        } while((x*e1 + y*e2 + z*e3 + offset)[0] + R > len[0] || (x*e1 + y*e2 + z*e3 + offset)[1] + R > len[1] || (x*e1 + y*e2 + z*e3 + offset)[2] + R > len[2] || (x*e1 + y*e2 + z*e3)[0] < 0 || (x*e1 + y*e2 + z*e3)[1] < 0 || (x*e1 + y*e2 + z*e3)[2] - R < 0);
      }
      delete[] spot_used;
    }
    else if (strcmp(structure, "ice") == 0) {
      const double fac = R/sqrt(11.0)*1.1;
      // lattice vectors:
      const vector3d e1 = vector3d(0, 4, 4)*fac;
      const vector3d e2 = vector3d(4, 0, 4)*fac;
      const vector3d e3 = vector3d(4, 4, 0)*fac;

      const vector3d offset = vector3d(2, 2, 2)*fac;
      bool out_of_cell;
      int x=0, y=0, z=0;
      nx *= 4;
      ny *= 4;
      nz *= 4;
      bool *spot_used = new bool[nx*ny*nz]();
      for(int i=0; i<N; i+=2) {
        int attempt_counter = 0;
        do {
          x = floor(random::ran()*nx);
          y = floor(random::ran()*ny);
          z = floor(random::ran()*nz);
          const vector3d pos = (x-nx/2)*e1 + (y-ny/2)*e2 + (z-nz/2)*e3;
          out_of_cell = false;
          for(int j=0; j<3; j++) {
            if (pos[j] < 0 || pos[j] + offset[j] > len[j] - R)
              out_of_cell = true;
          }
          attempt_counter ++;
        } while(spot_used[x*ny*nz + y*nz + z] || out_of_cell);
        spot_used[x*ny*nz + y*nz + z] = true;
        x = x - nx/2;
        y = y - ny/2;
        z = z - nz/2;
        polyhedra[i].pos = x*e1 + y*e2 + z*e3;
        polyhedra[i].rot = rotation(M_PI, vector3d(1, 1, 0));
        if(i < N-1) {
          polyhedra[i+1].pos = offset + x*e1 + y*e2 + z*e3;
        }
        bool overlaps = false;
        max_attempts = max(attempt_counter, max_attempts);
        if (attempt_counter > 10000)
          printf("Polyhedron %i took %i attempts to place.\n", i, attempt_counter);
      }
      delete[] spot_used;
    }
    //polyhedra[1].rot = rotation(M_PI/4, vector3d(1, 1, 0));
  }
  took("Placement");
  printf("The most attempts for a polyhedron was %i.\n", max_attempts);

  // ---------------------------------------------------------------------------
  // Save the initial configuration for troubleshooting
  // ---------------------------------------------------------------------------
  char *vertices_fname = new char[1024];
  sprintf(vertices_fname, "%s/vertices/%s-vertices-%s-%i-%i.dat", dir, filename, shape->name, N, -1);
  save_locations(polyhedra, N, vertices_fname, len);
  delete[] vertices_fname;

  int most_neighbors =
    initialize_neighbor_tables(polyhedra, N, neighborR + 2*dr, max_neighbors, periodic);
  if (most_neighbors < 0) {
    fprintf(stderr, "The guess of %i max neighbors was too low. Exiting.\n", max_neighbors);
    return 1;
  }
  printf("Neighbor tables initialized. The most neighbors is %i, whereas the max allowed is %i.\n", most_neighbors, max_neighbors);

  print_all(polyhedra, N, periodic);
  print_bad(polyhedra, N, periodic);

  // ---------------------------------------------------------------------------
  // Make sure no polyhedra are overlapping
  // ---------------------------------------------------------------------------
  for(int i=0; i<N; i++) {
    if (!in_cell(polyhedra[i], walls, real_walls)) {
      printf("Oops, this is embarassing. I seem to have placed some things outside our cell.\n");
      printf("You might want to look into that.\n");
      return 17;
    }
    for(int j=i+1; j<N; j++) {
      if (overlap(polyhedra[i], polyhedra[j], periodic)) {
        printf("ERROR in initial placement. We have overlaps!!!\n");
        printf("AHHHHHH I DON'T KNOW WHAT TO DO!@!!!!1111\n");
        return 19;
      }
    }
  }
  fflush(stdout);

  // ---------------------------------------------------------------------------
  // Initialization of cell
  // ---------------------------------------------------------------------------
  long totalmoves = 0, workingmoves = 0, old_totalmoves = 0, old_workingmoves = 0;
  long neighbor_updates = 0, neighbor_informs = 0;
  double avg_neighbors = 0;

  for(long iteration=1; iteration<=initialize_iterations; iteration++) {
    // ---------------------------------------------------------------
    // Move each polyhedron once
    // ---------------------------------------------------------------
    for(int i=0; i<N; i++) {
      totalmoves ++;
      int move_val = move_one_polyhedron(i, polyhedra, N, periodic, walls, real_walls,
                                        neighborR, scale, theta_scale, max_neighbors, dr);
      workingmoves += move_val & 1;
      neighbor_updates += (move_val & 2) > 0;
      neighbor_informs += (move_val & 4) > 0;
    }
    // ---------------------------------------------------------------
    // fine-tune scale so that the acceptance rate will be reasonable
    // ---------------------------------------------------------------
    if (iteration % 100 == 0) {
      const double acceptance_rate =
        (double)(workingmoves-old_workingmoves)/(totalmoves-old_totalmoves);
      old_workingmoves = workingmoves;
      old_totalmoves = totalmoves;
      if (acceptance_rate < 0.5) {
        scale /= 1.1;
        theta_scale /= 1.1;
      } else if (acceptance_rate > 0.7) {
        scale *= 1.1;
        theta_scale *= 1.1;
      }
    }
    // ---------------------------------------------------------------
    // Print out timing information if desired
    // ---------------------------------------------------------------
    if (totime > 0 && iteration % totime == 0) {
      char *iter = new char[1024];
      sprintf(iter, "%i iterations", totime);
      took(iter);
      delete[] iter;
      printf("Iteration %li, acceptance rate of %g, scale: %g.\n", iteration, (double)workingmoves/totalmoves, scale);
      printf("We've had %g updates per kilomove and %g informs per kilomove, for %g informs per update.\n", 1000.0*neighbor_updates/totalmoves, 1000.0*neighbor_informs/totalmoves, (double)neighbor_informs/neighbor_updates);
      const long checks_without_tables = totalmoves*N;
      int total_neighbors = 0;
      for(int i=0; i<N; i++) {
        total_neighbors += polyhedra[i].num_neighbors;
        most_neighbors = max(polyhedra[i].num_neighbors, most_neighbors);
      }
      avg_neighbors = double(total_neighbors)/N;
      const long checks_with_tables = totalmoves*avg_neighbors + N*neighbor_updates;
      printf("We've done about %.3g%% of the distance calculations we would have done without tables.\n", 100.0*checks_with_tables/checks_without_tables);
      printf("The max number of neighbors is %i, whereas the most we've seen is %i.\n", max_neighbors, most_neighbors);
      printf("Neighbor radius is %g and avg. number of neighbors is %g.\n\n", neighborR, avg_neighbors);
      fflush(stdout);
    }
  }
  took("Initialization");

  // ---------------------------------------------------------------
  // Clear the pressure file
  // ---------------------------------------------------------------
  char *pressure_fname = new char[1024];
  sprintf(pressure_fname, "%s/%s-pressure-%s-%i.dat", dir, filename, shape->name, N);
  FILE *pressureout = fopen((const char *)pressure_fname, "w");
  fprintf(pressureout, "\
# period: (%5.2f, %5.2f, %5.2f), walls: (%5.2f, %5.2f, %5.2f), de_density: %g\n\
# seed: %li, R: %f, scale: %g, theta_scale: %g, real_walls: %i\n",
          periodic[0], periodic[1], periodic[2],
          walls[0], walls[1], walls[2], de_density, seed, R,
          scale, theta_scale, real_walls);
  const double dV = N*shape->volume*(uipow(R+dr, 3) - uipow(R, 3));
  fprintf(pressureout, "0 0 %g #dV\n", dV);
  fprintf(pressureout, "# total moves    pressure     dZ\n");
  delete[] pressure_fname;
  fclose(pressureout);

  // ---------------------------------------------------------------------------
  // MAIN PROGRAM LOOP
  // ---------------------------------------------------------------------------
  clock_t output_period = CLOCKS_PER_SEC*60; // start at outputting every minute
  clock_t max_output_period = clock_t(CLOCKS_PER_SEC)*60*60; // top out at one hour interval
  clock_t last_output = clock(); // when we last output data

  int frame = 0;
  totalmoves = 0, workingmoves = 0, old_totalmoves = 0, old_workingmoves = 0;
  neighbor_updates = 0, neighbor_informs = 0;

  for(long iteration=1; iteration<=iterations; iteration++) {
    // ---------------------------------------------------------------
    // Move each polyhedron once
    // ---------------------------------------------------------------
    for(int i=0; i<N; i++) {
      totalmoves ++;
      int move_val = move_one_polyhedron(i, polyhedra, N, periodic, walls, real_walls,
                                         neighborR, scale, theta_scale, max_neighbors, dr);
      workingmoves += move_val & 1;
    }
    // ---------------------------------------------------------------
    // Add data to historams
    // ---------------------------------------------------------------
    for(int i=0; i<N; i++) {
      // Density histogram:
      const int x_i = floor(polyhedra[i].pos[0]/de_density);
      const int y_i = floor(polyhedra[i].pos[1]/de_density);
      const int z_i = floor(polyhedra[i].pos[2]/de_density);
      density_histogram[x_i] ++;
      density_histogram[int(round(len[0]/de_density)) + y_i] ++;
      density_histogram[int(round((len[0] + len[1])/de_density)) + z_i] ++;
    }
    // ---------------------------------------------------------------
    // Get pressure info - this is slow, might want to do it less often
    // ---------------------------------------------------------------
    for(int i=0; i<N; i++) {
      dZ += overlaps_with_any(polyhedra[i], polyhedra, periodic, true, dr);
    }
    dZ /= 2; // since we check each thing against each of its neighbors, we end up
             // double counting. Ideally, we would count half as much instead of dividing
    // ---------------------------------------------------------------
    // Save to file
    // ---------------------------------------------------------------
    const clock_t now = clock();
    if ((now > last_output + output_period) || iteration==iterations) {
      last_output = now;
      assert(last_output);
      if (output_period < max_output_period/2) output_period *= 2;
      else if (output_period < max_output_period) output_period = max_output_period;
      const double secs_done = double(now)/CLOCKS_PER_SEC;
      const int seconds = int(secs_done) % 60;
      const int minutes = int(secs_done / 60) % 60;
      const int hours = int(secs_done / 3600) % 24;
      const int days = int(secs_done / 86400);
      printf("Saving data after %i days, %02i:%02i:%02i, %li iterations complete.\n",
             days, hours, minutes, seconds, iteration);
      fflush(stdout);

      char *headerinfo = new char[4096];
      sprintf(headerinfo, "\
# period: (%5.2f, %5.2f, %5.2f), walls: (%5.2f, %5.2f, %5.2f), de_density: %g\n\
# seed: %li, R: %f, scale: %g, theta_scale: %g, real_walls: %i\n\
# iteration: %li, workingmoves: %li, totalmoves: %li, acceptance rate: %g\n",
              periodic[0], periodic[1], periodic[2],
              walls[0], walls[1], walls[2], de_density, seed, R,
              scale, theta_scale, real_walls, iteration, workingmoves,
              totalmoves, double(workingmoves)/totalmoves);

      // Saving density in each of the x, y, z dimensions
      char *density_fname = new char[1024];
      sprintf(density_fname, "%s/%s-density-%s-%i.dat", dir, filename, shape->name, N);
      FILE *densityout = fopen((const char *)density_fname, "w");
      delete[] density_fname;
      fprintf(densityout, "%s", headerinfo);
      fprintf(densityout, "\n#e       xdensity   ydensity   zdensity   histograms in x,y,z order\n");
      const int xbins = round(len[0]/de_density);
      const int ybins = round(len[1]/de_density);
      const int zbins = round(len[2]/de_density);
      int maxbin = max(max(xbins, ybins), zbins);
      for(int e_i = 0; e_i < maxbin; e_i ++) {
        const double e = (e_i + 0.5)*de_density;
        const double xshell_volume = len[1]*len[2]*de_density;
        const double yshell_volume = len[0]*len[2]*de_density;
        const double zshell_volume = len[0]*len[1]*de_density;
        // If we have a non-cubic cell, then this makes the density negative
        // in the dimensions where it does not exist.
        const long xhist = e_i>=xbins ? -totalmoves : density_histogram[e_i];
        const long yhist = e_i>=ybins ? -totalmoves : density_histogram[xbins + e_i];
        const long zhist = e_i>=zbins ? -totalmoves : density_histogram[xbins + ybins + e_i];
        const double xdensity = (double)xhist*N/totalmoves/xshell_volume;
        const double ydensity = (double)yhist*N/totalmoves/yshell_volume;
        const double zdensity = (double)zhist*N/totalmoves/zshell_volume;
        fprintf(densityout, "%6.3f   %8.5f   %8.5f   %8.5f   %li %li %li\n", e, xdensity, ydensity, zdensity, xhist, yhist, zhist);
      }
      fclose(densityout);
      delete[] headerinfo;

      // Save pressure
      char *pressure_fname = new char[1024];
      sprintf(pressure_fname, "%s/%s-pressure-%s-%i.dat", dir, filename, shape->name, N);
      FILE *pressureout = fopen((const char *)pressure_fname, "a");
      delete[] pressure_fname;
      const double dV = N*shape->volume*(uipow(R+dr, 3) - uipow(R, 3));
      const double pressure = dZ/dV/totalmoves;
      fprintf(pressureout, "%li %g %li\n", totalmoves, pressure, dZ);
      old_totalmoves = totalmoves;
      dZ = 0;
      fclose(pressureout);
    }
    // ---------------------------------------------------------------
    // Save locations of vertices if desired
    // ---------------------------------------------------------------
    if (vertex_period > 0 && iteration % vertex_period == 0) {
      printf("Saving vertex locations. Frame: %i. Iteration: %li.\n", frame, iteration);
      char *vertices_fname = new char[1024];
      sprintf(vertices_fname, "%s/vertices/%s-vertices-%s-%i-%i.dat", dir, filename, shape->name, N, frame);
      char *comment = new char[1024];
      sprintf(comment, "period: %i, iteration: %li", vertex_period, iteration);
      save_locations(polyhedra, N, vertices_fname, len, comment);
      delete[] vertices_fname;
      frame ++;
    }
  }
  // ---------------------------------------------------------------------------
  // END OF MAIN PROGRAM LOOP
  // ---------------------------------------------------------------------------
  print_bad(polyhedra, N, periodic);

  delete[] polyhedra;
  delete[] density_histogram;
  return 0;
}
// -----------------------------------------------------------------------------
// END OF MAIN
// -----------------------------------------------------------------------------

inline void print_all(const polyhedron *p, int N, double periodic[3]) {
  if (debug) {
    for (int i=0; i<N; i++) {
      char *pos = new char[1024];
      char *rot = new char[1024];
      p[i].pos.tostr(pos);
      p[i].rot.tostr(rot);
      printf("%4i: %s, R: %4.2f, %i neighbors: ", i, p[i].mypoly->name, p[i].R, p[i].num_neighbors);
      for(int j=0; j<min(10, p[i].num_neighbors); j++)
        printf("%i ", p[i].neighbors[j]);
      if (p[i].num_neighbors > 10)
        printf("...");
      printf("\n      pos:          %s\n      rot: %s\n", pos, rot);
    }
    printf("\n");
    fflush(stdout);
  }
}

inline void print_one(const polyhedron &a, int id, const polyhedron *p, int N, double periodic[3]) {
  if (debug) {
    char *pos = new char[1024];
    char *rot = new char[1024];
    a.pos.tostr(pos);
    a.rot.tostr(rot);
    printf("%4i: %s, R: %4.2f, %i neighbors: ", id, a.mypoly->name, a.R, a.num_neighbors);
    for(int j=0; j<min(10, a.num_neighbors); j++)
      printf("%i ", a.neighbors[j]);
    if (a.num_neighbors > 10)
      printf("...");
    printf("\n      pos:          %s\n      rot: %s\n", pos, rot);
    for (int j=0; j<N; j++) {
      if (j != id && overlap(a, p[j], periodic)) {
        p[j].pos.tostr(pos);
        p[j].rot.tostr(rot);
        printf("\t  Overlaps with %i", j);
        printf(": %s   %s\n", pos, rot);
      }
    }
  }
  printf("\n");
  fflush(stdout);
}

inline void print_bad(const polyhedron *p, int N, double periodic[3]) {
  if (debug) {
    for (int i=0; i<N; i++) {
      bool incell = true; //in_cell(p[i], walls, real_walls); fixme
      bool overlaps = false;
      for (int j=0; j<N; j++) {
        if (j != i && overlap(p[i], p[j], periodic)) {
          overlaps = true;
          break;
        }
      }
      if (!incell || overlaps) {
        char *pos = new char[1024];
        char *rot = new char[1024];
        p[i].pos.tostr(pos);
        p[i].rot.tostr(rot);
        printf("%s %4i: %s   %s R: %4.2f\n", p[i].mypoly->name, i, pos, rot, p[i].R);
        if (!incell)
          printf("\t  Outside cell!\n");
        for (int j=0; j<N; j++) {
          if (j != i && overlap(p[i], p[j], periodic)) {
            p[j].pos.tostr(pos);
            p[j].rot.tostr(rot);
            printf("\t  Overlaps with %i", j);
            printf(": %s   %s\n", pos, rot);
          }
        }
        delete[] pos;
        delete[] rot;
      }
    }
  }
  fflush(stdout);
}

inline void check_neighbor_symmetry(const polyhedron *p, int N) {
  if (debug) {
    for(int i=0; i<N; i++) {
      for(int j=0; j<p[i].num_neighbors; j++) {
        const int k = p[i].neighbors[j];
        bool is_neighbor = false;
        for (int l=0; l<p[k].num_neighbors; l++) {
          if (p[k].neighbors[l] == i) {
            is_neighbor = true;
            break;
          }
        }
        if(!is_neighbor) {
          printf("NEIGHBOR TABLE ERROR: %i has %i as a neighbor, but %i does not reciprocate!!!\n", i, k, k);
          testcase = true;
        }
      }
    }
  }
}
static void took(const char *name) {
  assert(name); // so it'll count as being used...
  static clock_t last_time = clock();
  clock_t t = clock();
  double seconds = (t-last_time)/double(CLOCKS_PER_SEC);
  if (seconds > 120) {
    printf("%s took %.0f minutes and %g seconds.\n", name, seconds/60, fmod(seconds,60));
  } else {
    printf("%s took %g seconds..\n", name, seconds);
  }
  fflush(stdout);
  last_time = t;
}

void save_locations(const polyhedron *p, int N, const char *fname, const double len[3], const char *comment) {
  FILE *out = fopen((const char *)fname, "w");
  fprintf(out, "# %s\n", comment);
  fprintf(out, "%g %g %g\n", len[0], len[1], len[2]);
  for(int i=0; i<N; i++) {
    fprintf(out, "%6.2f %6.2f %6.2f ", p[i].pos[0], p[i].pos[1], p[i].pos[2]);
    for(int j=0; j<p[i].mypoly->nvertices; j++) {
      const vector3d vertex =
        p[i].rot.rotate_vector(p[i].mypoly->vertices[j]*p[i].R) + p[i].pos;
      fprintf(out, "%6.2f %6.2f %6.2f ", vertex[0], vertex[1], vertex[2]);
    }
    fprintf(out, "\n");
  }
  fclose(out);
}

int generate_talk_figs() {
  // Generate background image for title slide
  int N = 7;
  polyhedron *p = new polyhedron[N];
  const double len[3] = {20, 20, 20};
  const double R = 1;
  for(int i=0; i<N; i++) {
    p[i].R = R;
  }
  p[0].mypoly = &truncated_tetrahedron;
  p[1].mypoly = &cube;
  p[2].mypoly = &truncated_tetrahedron;
  p[3].mypoly = &tetrahedron;
  p[4].mypoly = &cube;
  p[5].mypoly = &truncated_tetrahedron;
  p[6].mypoly = &tetrahedron;

  const double periodic[3] = {0, 0, 0};
  const double walls[3] = {5, 5, 5};
  const bool real_walls = true;
  const int max_neighbors = N;
  const double scale = 1.5;
  const double theta_scale = M_PI/4.0;
  const double neighborR = .5;

  for(int i=0; i<N; i++) {
    p[i].pos = vector3d(random::ran(), random::ran(), random::ran())*3;
  }
  initialize_neighbor_tables(p, N, neighborR, max_neighbors, periodic);

  for(int j=0; j<100; j++) {
    for(int i=0; i<N; i++) {
      move_one_polyhedron(i, p, N, periodic, walls, real_walls,
                          neighborR, scale, theta_scale, max_neighbors, 0);
    }
  }
  for(int i=0; i<N; i++) {
    p[i].pos += vector3d(1, -4, 0);
  }
  save_locations(p, N, "talks/polyhedra/dat/background.dat", len);

  delete[] p;
  // Generate image of ice structure
  int nx = 3;
  int ny = 3;
  int nz = 3;

  N = 80;
  p = new polyhedron[N];


  for(int i=0; i<N; i++) {
    p[i].R = R;
    p[i].mypoly = &truncated_tetrahedron;
  }
  const double fac = R/sqrt(11.0)*1.05;
  // lattice vectors:
  const vector3d e1 = vector3d(0, 4, 4)*fac;
  const vector3d e2 = vector3d(4, 0, 4)*fac;
  const vector3d e3 = vector3d(4, 4, 0)*fac;
  const vector3d offset = vector3d(2, 2, 2)*fac;

  int x=0, y=0, z=0;
  nx = 6;
  ny = 6;
  nz = 6;
  double rad = 3.5;
  bool *spot_used = new bool[nx*ny*nz]();
  for(int i=0; i<N; i+=2) {
    int x, y, z;
    do {
      x = floor(random::ran()*nx);
      y = floor(random::ran()*ny);
      z = floor(random::ran()*nz);
    } while(spot_used[x*ny*nz + y*nz + z] || ((x-nx/2)*e1 + (y-ny/2)*e2 + (z-nz/2)*e3).norm() > rad);
    spot_used[x*ny*nz + y*nz + z] = true;
    x = x - nx/2;
    y = y - ny/2;
    z = z - nz/2;
    p[i].pos = x*e1 + y*e2 + z*e3;
    p[i].rot = rotation(M_PI, vector3d(1, 1, 0));
    if(i < N-1) {
      p[i+1].pos = offset + x*e1 + y*e2 + z*e3;
    }
  }
  save_locations(p, N, "talks/polyhedra/dat/ice-structure.dat", len);

  delete[] spot_used;
  delete[] p;

  return 0;
}
