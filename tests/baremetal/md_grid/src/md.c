#include "md.h"

#define MIN(x,y) ( (x)<(y) ? (x) : (y) )
#define MAX(x,y) ( (x)>(y) ? (x) : (y) )

//~ void md( int32_t /* int */ n_points[blockSide][blockSide][blockSide],
         //~ dvector_t force[blockSide][blockSide][blockSide][densityFactor],
         //~ dvector_t position[blockSide][blockSide][blockSide][densityFactor] )
void md( int32_t n_points[blockSide][blockSide][blockSide],
         TYPE force_x[blockSide][blockSide][blockSide][densityFactor],
         TYPE force_y[blockSide][blockSide][blockSide][densityFactor],
         TYPE force_z[blockSide][blockSide][blockSide][densityFactor],
         TYPE position_x[blockSide][blockSide][blockSide][densityFactor],
         TYPE position_y[blockSide][blockSide][blockSide][densityFactor],
         TYPE position_z[blockSide][blockSide][blockSide][densityFactor]
       )
{
#pragma HLS interface s_axilite port=return
#pragma HLS INTERFACE m_axi depth=64 port=n_points offset=slave
#pragma HLS INTERFACE m_axi depth=640 port=force_x offset=slave
#pragma HLS INTERFACE m_axi depth=640 port=force_y offset=slave
#pragma HLS INTERFACE m_axi depth=640 port=force_z offset=slave
#pragma HLS INTERFACE m_axi depth=640 port=position_x offset=slave
#pragma HLS INTERFACE m_axi depth=640 port=position_y offset=slave
#pragma HLS INTERFACE m_axi depth=640 port=position_z offset=slave
  ivector_t b0, b1; // b0 is the current block, b1 is b0 or a neighboring block
  dvector_t p, q; // p is a point in b0, q is a point in either b0 or b1
  int32_t p_idx, q_idx;
  TYPE dx, dy, dz, r2inv, r6inv, potential, f;

  // Iterate over the grid, block by block
  loop_grid0_x: for( b0.x=0; b0.x<blockSide; b0.x++ ) {
  loop_grid0_y: for( b0.y=0; b0.y<blockSide; b0.y++ ) {
  loop_grid0_z: for( b0.z=0; b0.z<blockSide; b0.z++ ) {
  // Iterate over the 3x3x3 (modulo boundary conditions) cube of blocks around b0
  loop_grid1_x: for( b1.x=MAX(0,b0.x-1); b1.x<MIN(blockSide,b0.x+2); b1.x++ ) {
  loop_grid1_y: for( b1.y=MAX(0,b0.y-1); b1.y<MIN(blockSide,b0.y+2); b1.y++ ) {
  loop_grid1_z: for( b1.z=MAX(0,b0.z-1); b1.z<MIN(blockSide,b0.z+2); b1.z++ ) {
    // For all points in b0
    //~ dvector_t *base_q = position[b1.x][b1.y][b1.z];
    TYPE *base_q_x = &position_x[b1.x][b1.y][b1.z][0]; // position[b1.x][b1.y][b1.z];
    TYPE *base_q_y = &position_y[b1.x][b1.y][b1.z][0]; // position[b1.x][b1.y][b1.z];
    TYPE *base_q_z = &position_z[b1.x][b1.y][b1.z][0]; // position[b1.x][b1.y][b1.z];
    int q_idx_range = n_points[b1.x][b1.y][b1.z];
    loop_p: for( p_idx=0; p_idx<n_points[b0.x][b0.y][b0.z]; p_idx++ ) {
      //~ p = position[b0.x][b0.y][b0.z][p_idx];
      p.x = position_x[b0.x][b0.y][b0.z][p_idx]; // position[b0.x][b0.y][b0.z][p_idx];
      p.y = position_y[b0.x][b0.y][b0.z][p_idx]; // position[b0.x][b0.y][b0.z][p_idx];
      p.z = position_z[b0.x][b0.y][b0.z][p_idx]; // position[b0.x][b0.y][b0.z][p_idx];
      TYPE sum_x = force_x[b0.x][b0.y][b0.z][p_idx]; // force[b0.x][b0.y][b0.z][p_idx].x;
      TYPE sum_y = force_y[b0.x][b0.y][b0.z][p_idx]; // force[b0.x][b0.y][b0.z][p_idx].y;
      TYPE sum_z = force_z[b0.x][b0.y][b0.z][p_idx]; // force[b0.x][b0.y][b0.z][p_idx].z;
      // For all points in b1
      loop_q: for( q_idx=0; q_idx< q_idx_range ; q_idx++ ) {
        //~ q = *(base_q + q_idx);
        q.x = *(base_q_x + q_idx);
        q.y = *(base_q_y + q_idx);
        q.z = *(base_q_z + q_idx);

        // Don't compute our own
        if( q.x!=p.x || q.y!=p.y || q.z!=p.z ) {
          // Compute the LJ-potential
          dx = p.x - q.x;
          dy = p.y - q.y;
          dz = p.z - q.z;
          r2inv = 1.0/( dx*dx + dy*dy + dz*dz );
          r6inv = r2inv*r2inv*r2inv;
          potential = r6inv*(lj1*r6inv - lj2);
          // Update forces
          f = r2inv*potential;
          sum_x += f*dx;
          sum_y += f*dy;
          sum_z += f*dz;
        }
      } // loop_q
      force_x[b0.x][b0.y][b0.z][p_idx] = sum_x; // force[b0.x][b0.y][b0.z][p_idx].x = sum_x ;
      force_y[b0.x][b0.y][b0.z][p_idx] = sum_y; // force[b0.x][b0.y][b0.z][p_idx].y = sum_y ;
      force_z[b0.x][b0.y][b0.z][p_idx] = sum_z; // force[b0.x][b0.y][b0.z][p_idx].z = sum_z ;
    } // loop_p
  }}} // loop_grid1_*
  }}} // loop_grid0_*
}
