/*
Implementations based on:
Harish and Narayanan. "Accelerating large graph algorithms on the GPU using CUDA." HiPC, 2007.
Hong, Oguntebi, Olukotun. "Efficient Parallel Graph Exploration on Multi-Core CPU and GPU." PACT, 2011.
*/

#include "bfs.h"

//~ void bfs(node_t nodes[N_NODES], edge_t edges[N_EDGES],
            //~ node_index_t starting_node, level_t level[N_NODES],
            //~ edge_index_t level_counts[N_LEVELS])
void bfs(edge_index_t nodes_begin[N_EDGES], edge_index_t nodes_end[N_EDGES], node_index_t edges[N_EDGES], node_index_t starting_node[N_EDGES], level_t level[N_EDGES], edge_index_t level_counts[N_EDGES])
{
#pragma HLS interface s_axilite port=return
#pragma HLS INTERFACE m_axi depth=256 port=nodes_begin offset=slave
#pragma HLS INTERFACE m_axi depth=256 port=nodes_end offset=slave
#pragma HLS INTERFACE m_axi depth=4096 port=edges offset=slave
#pragma HLS INTERFACE m_axi depth=1 port=starting_node offset=slave
#pragma HLS INTERFACE m_axi depth=256 port=level offset=slave
#pragma HLS INTERFACE m_axi depth=10 port=level_counts offset=slave
  node_index_t n;
  edge_index_t e;
  level_t horizon;
  edge_index_t cnt;

  level[starting_node[0]] = 0; //level[starting_node] = 0;
  level_counts[0] = 1;

  loop_horizons: for( horizon=0; horizon<N_LEVELS; horizon++ ) {
    cnt = 0;
    // Add unmarked neighbors of the current horizon to the next horizon
    loop_nodes: for( n=0; n<N_NODES; n++ ) {
      if( level[n]==horizon ) {
        edge_index_t tmp_begin = nodes_begin[n]; //nodes[n].edge_begin;
        edge_index_t tmp_end = nodes_end[n]; //nodes[n].edge_end;
        loop_neighbors: for( e=tmp_begin; e<tmp_end; e++ ) {
          node_index_t tmp_dst = edges[e]; //edges[e].dst;
          level_t tmp_level = level[tmp_dst];

          if( tmp_level ==MAX_LEVEL ) { // Unmarked
            level[tmp_dst] = horizon+1;
            ++cnt;
          }
        }
      }
    }
    if( (level_counts[horizon+1]=cnt)==0 )
      break;
  }
}
