#! /bin/bash

# Create output directories
mkdir -p diff/artico3
mkdir -p diff/sdsoc
mkdir -p diff/versus

# Check differences between original and ARTICo³                                               
diff -u ../aes/aes/aes.h               artico3/aes/aes/src/a3_aes/hls/aes.h                      > diff/artico3/aes.diff
diff -u ../aes/aes/aes.c               artico3/aes/aes/src/a3_aes/hls/aes.cpp                   >> diff/artico3/aes.diff
                                                                                               
diff -u ../bfs/bulk/bfs.h              artico3/bfs/bulk/src/a3_bfs/hls/bfs.h                     > diff/artico3/bfs_bulk.diff
diff -u ../bfs/bulk/bfs.c              artico3/bfs/bulk/src/a3_bfs/hls/bfs.cpp                  >> diff/artico3/bfs_bulk.diff
                                                                                               
diff -u ../bfs/queue/bfs.h             artico3/bfs/queue/src/a3_bfs/hls/bfs.h                    > diff/artico3/bfs_queue.diff
diff -u ../bfs/queue/bfs.c             artico3/bfs/queue/src/a3_bfs/hls/bfs.cpp                 >> diff/artico3/bfs_queue.diff
                                                                                               
diff -u ../fft/strided/fft.h           artico3/fft/strided/src/a3_fft/hls/fft.h                  > diff/artico3/fft_strided.diff
diff -u ../fft/strided/fft.c           artico3/fft/strided/src/a3_fft/hls/fft.cpp               >> diff/artico3/fft_strided.diff
                                                                                               
diff -u ../gemm/ncubed/gemm.h          artico3/gemm/ncubed/src/a3_gemm/hls/gemm.h                > diff/artico3/gemm_ncubed.diff
diff -u ../gemm/ncubed/gemm.c          artico3/gemm/ncubed/src/a3_gemm/hls/gemm.cpp             >> diff/artico3/gemm_ncubed.diff
                                                                                        
diff -u ../gemm/blocked/gemm.h         artico3/gemm/blocked/src/a3_gemm/hls/gemm.h               > diff/artico3/gemm_blocked.diff
diff -u ../gemm/blocked/gemm.c         artico3/gemm/blocked/src/a3_gemm/hls/gemm.cpp            >> diff/artico3/gemm_blocked.diff
                                                                                               
diff -u ../kmp/kmp/kmp.h               artico3/kmp/kmp/src/a3_kmp/hls/kmp.h                      > diff/artico3/kmp.diff
diff -u ../kmp/kmp/kmp.c               artico3/kmp/kmp/src/a3_kmp/hls/kmp.cpp                   >> diff/artico3/kmp.diff
                                                                                                
diff -u ../md/knn/md.h                 artico3/md/knn/src/a3_md/hls/md.h                         > diff/artico3/md_knn.diff
diff -u ../md/knn/md.c                 artico3/md/knn/src/a3_md/hls/md.cpp                      >> diff/artico3/md_knn.diff
                                                                                                
diff -u ../md/grid/md.h                artico3/md/grid/src/a3_md/hls/md.h                        > diff/artico3/md_grid.diff
diff -u ../md/grid/md.c                artico3/md/grid/src/a3_md/hls/md.cpp                     >> diff/artico3/md_grid.diff
                                                                                                
diff -u ../nw/nw/nw.h                  artico3/nw/nw/src/a3_nw/hls/nw.h                          > diff/artico3/nw.diff
diff -u ../nw/nw/nw.c                  artico3/nw/nw/src/a3_nw/hls/nw.cpp                       >> diff/artico3/nw.diff
                                                                                                
diff -u ../sort/merge/sort.h           artico3/sort/merge/src/a3_sort/hls/sort.h                 > diff/artico3/sort_merge.diff
diff -u ../sort/merge/sort.c           artico3/sort/merge/src/a3_sort/hls/sort.cpp              >> diff/artico3/sort_merge.diff
                                                                                                
diff -u ../sort/radix/sort.h           artico3/sort/radix/src/a3_sort/hls/sort.h                 > diff/artico3/sort_radix.diff
diff -u ../sort/radix/sort.c           artico3/sort/radix/src/a3_sort/hls/sort.cpp              >> diff/artico3/sort_radix.diff
                                                                                                
diff -u ../spmv/crs/spmv.h             artico3/spmv/crs/src/a3_spmv/hls/spmv.h                   > diff/artico3/spmv_crs.diff
diff -u ../spmv/crs/spmv.c             artico3/spmv/crs/src/a3_spmv/hls/spmv.cpp                >> diff/artico3/spmv_crs.diff
                                                                                                
diff -u ../spmv/ellpack/spmv.h         artico3/spmv/ellpack/src/a3_spmv/hls/spmv.h               > diff/artico3/spmv_ellpack.diff
diff -u ../spmv/ellpack/spmv.c         artico3/spmv/ellpack/src/a3_spmv/hls/spmv.cpp            >> diff/artico3/spmv_ellpack.diff
                                                                                                
diff -u ../stencil/stencil2d/stencil.h artico3/stencil/stencil2d/src/a3_stencil/hls/stencil.h    > diff/artico3/stencil_stencil2d.diff
diff -u ../stencil/stencil2d/stencil.c artico3/stencil/stencil2d/src/a3_stencil/hls/stencil.cpp >> diff/artico3/stencil_stencil2d.diff
                                                                                                
diff -u ../stencil/stencil3d/stencil.h artico3/stencil/stencil3d/src/a3_stencil/hls/stencil.h    > diff/artico3/stencil_stencil3d.diff
diff -u ../stencil/stencil3d/stencil.c artico3/stencil/stencil3d/src/a3_stencil/hls/stencil.cpp >> diff/artico3/stencil_stencil3d.diff
                                                                                                
diff -u ../viterbi/viterbi/viterbi.h   artico3/viterbi/viterbi/src/a3_viterbi/hls/viterbi.h      > diff/artico3/viterbi.diff
diff -u ../viterbi/viterbi/viterbi.c   artico3/viterbi/viterbi/src/a3_viterbi/hls/viterbi.cpp   >> diff/artico3/viterbi.diff

# Check differences between original and SDSoC
diff -u ../aes/aes/aes.h               baremetal/aes/src/aes.h                    > diff/sdsoc/aes.diff
diff -u ../aes/aes/aes.c               baremetal/aes/src/aes.c                   >> diff/sdsoc/aes.diff

diff -u ../bfs/bulk/bfs.h              baremetal/bfs_bulk/src/bfs.h               > diff/sdsoc/bfs_bulk.diff
diff -u ../bfs/bulk/bfs.c              baremetal/bfs_bulk/src/bfs.c              >> diff/sdsoc/bfs_bulk.diff

diff -u ../bfs/queue/bfs.h             baremetal/bfs_queue/src/bfs.h              > diff/sdsoc/bfs_queue.diff
diff -u ../bfs/queue/bfs.c             baremetal/bfs_queue/src/bfs.c             >> diff/sdsoc/bfs_queue.diff

diff -u ../fft/strided/fft.h           baremetal/fft_strided/src/fft.h            > diff/sdsoc/fft_strided.diff
diff -u ../fft/strided/fft.c           baremetal/fft_strided/src/fft.c           >> diff/sdsoc/fft_strided.diff

diff -u ../gemm/ncubed/gemm.h          baremetal/gemm_ncubed/src/gemm.h           > diff/sdsoc/gemm_ncubed.diff
diff -u ../gemm/ncubed/gemm.c          baremetal/gemm_ncubed/src/gemm.c          >> diff/sdsoc/gemm_ncubed.diff

diff -u ../gemm/blocked/gemm.h         baremetal/gemm_blocked/src/gemm.h          > diff/sdsoc/gemm_blocked.diff
diff -u ../gemm/blocked/gemm.c         baremetal/gemm_blocked/src/gemm.c         >> diff/sdsoc/gemm_blocked.diff

diff -u ../kmp/kmp/kmp.h               baremetal/kmp/src/kmp.h                    > diff/sdsoc/kmp.diff
diff -u ../kmp/kmp/kmp.c               baremetal/kmp/src/kmp.c                   >> diff/sdsoc/kmp.diff

diff -u ../md/knn/md.h                 baremetal/md_knn/src/md.h                  > diff/sdsoc/md_knn.diff
diff -u ../md/knn/md.c                 baremetal/md_knn/src/md.c                 >> diff/sdsoc/md_knn.diff

diff -u ../md/grid/md.h                baremetal/md_grid/src/md.h                 > diff/sdsoc/md_grid.diff
diff -u ../md/grid/md.c                baremetal/md_grid/src/md.c                >> diff/sdsoc/md_grid.diff

diff -u ../nw/nw/nw.h                  baremetal/nw_nw/src/nw.h                   > diff/sdsoc/nw.diff
diff -u ../nw/nw/nw.c                  baremetal/nw_nw/src/nw.c                  >> diff/sdsoc/nw.diff
                                       
diff -u ../sort/merge/sort.h           baremetal/sort_merge/src/sort.h            > diff/sdsoc/sort_merge.diff
diff -u ../sort/merge/sort.c           baremetal/sort_merge/src/sort.c           >> diff/sdsoc/sort_merge.diff
                                                                                 
diff -u ../sort/radix/sort.h           baremetal/sort_radix/src/sort.h            > diff/sdsoc/sort_radix.diff
diff -u ../sort/radix/sort.c           baremetal/sort_radix/src/sort.c           >> diff/sdsoc/sort_radix.diff
                                                                                 
diff -u ../spmv/crs/spmv.h             baremetal/spmv_crs/src/spmv.h              > diff/sdsoc/spmv_crs.diff
diff -u ../spmv/crs/spmv.c             baremetal/spmv_crs/src/spmv.c             >> diff/sdsoc/spmv_crs.diff
                                                                                 
diff -u ../spmv/ellpack/spmv.h         baremetal/spmv_ellpack/src/spmv.h          > diff/sdsoc/spmv_ellpack.diff
diff -u ../spmv/ellpack/spmv.c         baremetal/spmv_ellpack/src/spmv.c         >> diff/sdsoc/spmv_ellpack.diff

diff -u ../stencil/stencil2d/stencil.h baremetal/stencil_stencil2d/src/stencil.h  > diff/sdsoc/stencil_stencil2d.diff
diff -u ../stencil/stencil2d/stencil.c baremetal/stencil_stencil2d/src/stencil.c >> diff/sdsoc/stencil_stencil2d.diff

diff -u ../stencil/stencil3d/stencil.h baremetal/stencil_stencil3d/src/stencil.h  > diff/sdsoc/stencil_stencil3d.diff
diff -u ../stencil/stencil3d/stencil.c baremetal/stencil_stencil3d/src/stencil.c >> diff/sdsoc/stencil_stencil3d.diff
                                       
diff -u ../viterbi/viterbi/viterbi.h   baremetal/viterbi/src/viterbi.h            > diff/sdsoc/viterbi.diff
diff -u ../viterbi/viterbi/viterbi.c   baremetal/viterbi/src/viterbi.c           >> diff/sdsoc/viterbi.diff

# Check differences between ARTICo³ and SDSoC
diff -u artico3/aes/aes/src/a3_aes/hls/aes.h                     baremetal/aes/src/aes.h                    > diff/versus/aes.diff
diff -u artico3/aes/aes/src/a3_aes/hls/aes.cpp                   baremetal/aes/src/aes.c                   >> diff/versus/aes.diff
                                                                                                           
diff -u artico3/bfs/bulk/src/a3_bfs/hls/bfs.h                    baremetal/bfs_bulk/src/bfs.h               > diff/versus/bfs_bulk.diff
diff -u artico3/bfs/bulk/src/a3_bfs/hls/bfs.cpp                  baremetal/bfs_bulk/src/bfs.c              >> diff/versus/bfs_bulk.diff
                                                                                                           
diff -u artico3/bfs/queue/src/a3_bfs/hls/bfs.h                   baremetal/bfs_queue/src/bfs.h              > diff/versus/bfs_queue.diff
diff -u artico3/bfs/queue/src/a3_bfs/hls/bfs.cpp                 baremetal/bfs_queue/src/bfs.c             >> diff/versus/bfs_queue.diff
                                                                                                           
diff -u artico3/fft/strided/src/a3_fft/hls/fft.h                 baremetal/fft_strided/src/fft.h            > diff/versus/fft_strided.diff
diff -u artico3/fft/strided/src/a3_fft/hls/fft.cpp               baremetal/fft_strided/src/fft.c           >> diff/versus/fft_strided.diff
                                                                                                           
diff -u artico3/gemm/ncubed/src/a3_gemm/hls/gemm.h               baremetal/gemm_ncubed/src/gemm.h           > diff/versus/gemm_ncubed.diff
diff -u artico3/gemm/ncubed/src/a3_gemm/hls/gemm.cpp             baremetal/gemm_ncubed/src/gemm.c          >> diff/versus/gemm_ncubed.diff
                                                                                                           
diff -u artico3/gemm/blocked/src/a3_gemm/hls/gemm.h              baremetal/gemm_blocked/src/gemm.h          > diff/versus/gemm_blocked.diff
diff -u artico3/gemm/blocked/src/a3_gemm/hls/gemm.cpp            baremetal/gemm_blocked/src/gemm.c         >> diff/versus/gemm_blocked.diff
                                                                                                           
diff -u artico3/kmp/kmp/src/a3_kmp/hls/kmp.h                     baremetal/kmp/src/kmp.h                    > diff/versus/kmp.diff
diff -u artico3/kmp/kmp/src/a3_kmp/hls/kmp.cpp                   baremetal/kmp/src/kmp.c                   >> diff/versus/kmp.diff
                                                                                                           
diff -u artico3/md/knn/src/a3_md/hls/md.h                        baremetal/md_knn/src/md.h                  > diff/versus/md_knn.diff
diff -u artico3/md/knn/src/a3_md/hls/md.cpp                      baremetal/md_knn/src/md.c                 >> diff/versus/md_knn.diff
                                                                                                           
diff -u artico3/md/grid/src/a3_md/hls/md.h                       baremetal/md_grid/src/md.h                 > diff/versus/md_grid.diff
diff -u artico3/md/grid/src/a3_md/hls/md.cpp                     baremetal/md_grid/src/md.c                >> diff/versus/md_grid.diff
                                                                                                           
diff -u artico3/nw/nw/src/a3_nw/hls/nw.h                         baremetal/nw_nw/src/nw.h                   > diff/versus/nw.diff
diff -u artico3/nw/nw/src/a3_nw/hls/nw.cpp                       baremetal/nw_nw/src/nw.c                  >> diff/versus/nw.diff
                                                                                                           
diff -u artico3/sort/merge/src/a3_sort/hls/sort.h                baremetal/sort_merge/src/sort.h            > diff/versus/sort_merge.diff
diff -u artico3/sort/merge/src/a3_sort/hls/sort.cpp              baremetal/sort_merge/src/sort.c           >> diff/versus/sort_merge.diff
                                                                                                           
diff -u artico3/sort/radix/src/a3_sort/hls/sort.h                baremetal/sort_radix/src/sort.h            > diff/versus/sort_radix.diff
diff -u artico3/sort/radix/src/a3_sort/hls/sort.cpp              baremetal/sort_radix/src/sort.c           >> diff/versus/sort_radix.diff
                                                                                                           
diff -u artico3/spmv/crs/src/a3_spmv/hls/spmv.h                  baremetal/spmv_crs/src/spmv.h              > diff/versus/spmv_crs.diff
diff -u artico3/spmv/crs/src/a3_spmv/hls/spmv.cpp                baremetal/spmv_crs/src/spmv.c             >> diff/versus/spmv_crs.diff
                                                                                                           
diff -u artico3/spmv/ellpack/src/a3_spmv/hls/spmv.h              baremetal/spmv_ellpack/src/spmv.h          > diff/versus/spmv_ellpack.diff
diff -u artico3/spmv/ellpack/src/a3_spmv/hls/spmv.cpp            baremetal/spmv_ellpack/src/spmv.c         >> diff/versus/spmv_ellpack.diff

diff -u artico3/stencil/stencil2d/src/a3_stencil/hls/stencil.h   baremetal/stencil_stencil2d/src/stencil.h  > diff/versus/stencil_stencil2d.diff
diff -u artico3/stencil/stencil2d/src/a3_stencil/hls/stencil.cpp baremetal/stencil_stencil2d/src/stencil.c >> diff/versus/stencil_stencil2d.diff

diff -u artico3/stencil/stencil3d/src/a3_stencil/hls/stencil.h   baremetal/stencil_stencil3d/src/stencil.h  > diff/versus/stencil_stencil3d.diff
diff -u artico3/stencil/stencil3d/src/a3_stencil/hls/stencil.cpp baremetal/stencil_stencil3d/src/stencil.c >> diff/versus/stencil_stencil3d.diff

diff -u artico3/viterbi/viterbi/src/a3_viterbi/hls/viterbi.h     baremetal/viterbi/src/viterbi.h            > diff/versus/viterbi.diff
diff -u artico3/viterbi/viterbi/src/a3_viterbi/hls/viterbi.cpp   baremetal/viterbi/src/viterbi.c           >> diff/versus/viterbi.diff
