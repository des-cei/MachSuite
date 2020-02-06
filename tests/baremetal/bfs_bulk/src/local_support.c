#include "bfs.h"
#include <string.h>
#include <unistd.h>
#include <ff.h>      // FIL, f_xxx()
#include <sds_lib.h> // sds_clock_counter()

int INPUT_SIZE = sizeof(struct bench_args_t);
float t;

void run_benchmark( void *vargs ) {
  struct bench_args_t *args = (struct bench_args_t *)vargs;
  //~ bfs(args->nodes, args->edges, args->starting_node, args->level, args->level_counts);

  edge_index_t nodes_begin[N_EDGES], nodes_end[N_EDGES], level_counts[N_EDGES];
  node_index_t edges[N_EDGES];
  node_index_t starting_node[N_EDGES];
  uint32_t level[N_EDGES];
  static uint32_t aux1[N_EDGES];
  static edge_index_t aux2[N_EDGES];
  static char flag = 0;
  unsigned long long t0, tf;

  for (int j = 0; j < N_NODES; j++) {
    nodes_begin[j] = args->nodes[j].edge_begin;
    nodes_end[j] = args->nodes[j].edge_end;
  }
  if (flag == 0) {
	for (int j = 0; j < N_NODES; j++) aux1[j] = args->level[j];
	memcpy(aux2, args->level_counts, sizeof aux2);
  }
  flag = 1;
  memcpy(level, aux1, sizeof level);
  memset(level_counts, 0, sizeof level_counts) ;//memcpy(level_counts, aux2, sizeof level_counts);
  for (int j = 0; j < N_EDGES; j++) {
    edges[j] = args->edges[j].dst;
  }
  starting_node[0] = args->starting_node;

  t0 = 2 * sds_clock_counter(); // AR# 67763
  bfs(nodes_begin, nodes_end, edges, starting_node, level, level_counts);
  tf = 2 * sds_clock_counter(); // AR# 67763
  t += (float)(tf-t0)/(sds_clock_frequency() / 1000);

  for (int j = 0; j < N_NODES; j++) {
    args->level[j] = level[j];
  }
  for (int j = 0; j < N_LEVELS; j++) {
    args->level_counts[j] = level_counts[j];
  }
}

/* Input format:
%% Section 1
uint32_t[1]: starting node
%% Section 2
uint32_t[N_NODES*2]: node structures (start and end indices of edge lists)
%% Section 3
uint32_t[N_EDGES]: edges structures (just destination node id)
*/

void input_to_data(FIL fd, void *vdata) { // void input_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  uint32_t *nodes;
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
  parse_uint32_t_array(s, &data->starting_node, 1);

  // Section 2: node structures
  s = find_section_start(p,2);
  nodes = (uint32_t *)malloc(N_NODES*2*sizeof(uint32_t));
  parse_uint32_t_array(s, nodes, N_NODES*2);
  for(i=0; i<N_NODES; i++) {
    data->nodes[i].edge_begin = nodes[2*i];
    data->nodes[i].edge_end = nodes[2*i+1];
  }
  free(nodes);
  // Section 3: edge structures
  s = find_section_start(p,3);
  parse_uint32_t_array(s, (uint32_t *)(data->edges), N_EDGES);
  free(p);
}

void data_to_input(FIL fd, void *vdata) { // void data_to_input(int fd, void *vdata) {
  uint32_t *nodes;
  int64_t i;

  struct bench_args_t *data = (struct bench_args_t *)vdata;
  // Section 1: starting node
  write_section_header(fd);
  write_uint32_t_array(fd, &data->starting_node, 1);
  // Section 2: node structures
  write_section_header(fd);
  nodes = (uint32_t *)malloc(N_NODES*2*sizeof(uint32_t));
  for(i=0; i<N_NODES; i++) {
    nodes[2*i]  = data->nodes[i].edge_begin;
    nodes[2*i+1]= data->nodes[i].edge_end;
  }
  write_uint32_t_array(fd, nodes, N_NODES*2);
  free(nodes);
  // Section 3: edge structures
  write_section_header(fd);
  write_uint32_t_array(fd, (uint32_t *)(&data->edges), N_EDGES);
}

/* Output format:
%% Section 1
uint32_t[N_LEVELS]: horizon counts
*/

void output_to_data(FIL fd, void *vdata) { // void output_to_data(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  char *p, *s;
  // Zero-out everything.
  memset(vdata,0,sizeof(struct bench_args_t));
  // Load input string
  p = readfile(fd);
  // Section 1: horizon counts
  s = find_section_start(p,1);
  parse_uint32_t_array(s, data->level_counts, N_LEVELS);
  free(p);
}

void data_to_output(FIL fd, void *vdata) { // void data_to_output(int fd, void *vdata) {
  struct bench_args_t *data = (struct bench_args_t *)vdata;
  // Section 1
  write_section_header(fd);
  write_uint32_t_array(fd, data->level_counts, N_LEVELS);
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
