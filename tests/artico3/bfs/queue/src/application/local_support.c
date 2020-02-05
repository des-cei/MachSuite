#include "bfs.h"
#include <string.h>
#include <unistd.h>
#include <sys/time.h> // struct timeval, gettimeofday()
#include "artico3.h"

int INPUT_SIZE = sizeof(struct bench_args_t);

#define ITERATIONS (1024)
#define ACCELERATORS (1)

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //~ bfs(args->nodes, args->edges, args->starting_node, args->level, args->level_counts);

  struct timeval t0, tf;
  float t;

  artico3_init();

  artico3_kernel_create("bfs", 32768, 2, 0);

  gettimeofday(&t0, NULL);
  for (int i = 0; i < ACCELERATORS; i++) {
      artico3_load("bfs", i, 0, 0, 0);
  }
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel loading : %.3f ms\n", t);

  a3data_t *edges = artico3_alloc(ITERATIONS * N_EDGES * sizeof *edges, "bfs", "edges", A3_P_I);
  a3data_t *pack = artico3_alloc(ITERATIONS * (3*N_NODES + N_LEVELS + 1) * sizeof *pack, "bfs", "pack", A3_P_IO);

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < N_EDGES; j++) {
      edges[i*N_EDGES + j] = args->edges[j].dst;
    }
    for (int j = 0; j < N_NODES; j++) {
      pack[i*(3*N_NODES + N_LEVELS + 1) + (0*N_NODES) + j] = args->nodes[j].edge_begin;
      pack[i*(3*N_NODES + N_LEVELS + 1) + (1*N_NODES) + j] = args->nodes[j].edge_end;
      pack[i*(3*N_NODES + N_LEVELS + 1) + (2*N_NODES) + 1 + j] = args->level[j];
    }
    pack[i*(3*N_NODES + N_LEVELS + 1) + (2*N_NODES)] = args->starting_node;
  }

  gettimeofday(&t0, NULL);
  artico3_kernel_execute("bfs", ITERATIONS, 1);
  artico3_kernel_wait("bfs");
  gettimeofday(&tf, NULL);
  t = ((tf.tv_sec - t0.tv_sec) * 1000.0) + ((tf.tv_usec - t0.tv_usec) / 1000.0);
  printf("Kernel execution : %.3f ms (average : %.3f ms)\n", t, t/ITERATIONS);

  for (int j = 0; j < N_NODES; j++) {

  }
  for (int j = 0; j < N_LEVELS; j++) {
    args->level[j] = pack[(2*N_NODES) + 1 + j];
    args->level_counts[j] = pack[(3*N_NODES) + 1 + j];
  }

  artico3_free("bfs", "edges");
  artico3_free("bfs", "pack");

  artico3_kernel_release("bfs");

  artico3_exit();
}

/* Input format:
%% Section 1
uint64_t[1]: starting node
%% Section 2
uint64_t[N_NODES*2]: node structures (start and end indices of edge lists)
%% Section 3
uint64_t[N_EDGES]: edges structures (just destination node id)
*/

void input_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  uint64_t *nodes;
  int64_t i;

  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Max-ify levels
  for(i=0; i<N_NODES; i++) {
    data->level[i]=MAX_LEVEL;
  }
  // Load input string
  p = readfile(fd);
  // Section 1: starting node
  s = find_section_start(p,1);
  parse_uint64_t_array(s, &data->starting_node, 1);

  // Section 2: node structures
  s = find_section_start(p,2);
  nodes = (uint64_t *)malloc(N_NODES*2*sizeof(uint64_t));
  parse_uint64_t_array(s, nodes, N_NODES*2);
  for(i=0; i<N_NODES; i++) {
    data->nodes[i].edge_begin = nodes[2*i];
    data->nodes[i].edge_end = nodes[2*i+1];
  }
  free(nodes);
  // Section 3: edge structures
  s = find_section_start(p,3);
  parse_uint64_t_array(s, (uint64_t *)(data->edges), N_EDGES);
  free(p);
}

void data_to_input(int fd, void *vdata) {
  uint64_t *nodes;
  int64_t i;

  struct bench_args_t *data = (struct bench_args_t *)vdata;
  // Section 1: starting node
  write_section_header(fd);
  write_uint64_t_array(fd, &data->starting_node, 1);
  // Section 2: node structures
  write_section_header(fd);
  nodes = (uint64_t *)malloc(N_NODES*2*sizeof(uint64_t));
  for(i=0; i<N_NODES; i++) {
    nodes[2*i]  = data->nodes[i].edge_begin;
    nodes[2*i+1]= data->nodes[i].edge_end;
  }
  write_uint64_t_array(fd, nodes, N_NODES*2);
  free(nodes);
  // Section 3: edge structures
  write_section_header(fd);
  write_uint64_t_array(fd, (uint64_t *)(&data->edges), N_EDGES);
}

/* Output format:
%% Section 1
uint64_t[N_LEVELS]: horizon counts
*/

void output_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);
  // Section 1: horizon counts
  s = find_section_start(p,1);
  parse_uint64_t_array(s, data->level_counts, N_LEVELS);
  free(p);
}

void data_to_output(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  // Section 1
  write_section_header(fd);
  write_uint64_t_array(fd, data->level_counts, N_LEVELS);
}

int check_data( void *vdata, void *vref ) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  struct bench_args_t *ref = (struct bench_args_t *)vref;
  int has_errors = 0;
  int i;

  // Check that the horizons have the same number of nodes
  for(i=0; i<N_LEVELS; i++) {
    has_errors |= (data->level_counts[i]!=ref->level_counts[i]);
  }

  // Return true if it's correct.
  return !has_errors;
}
