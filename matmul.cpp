#include <stdlib.h>
#include "matmul.h"
#define TILE_WIDTH_SMALL 16
#define TILE_WIDTH_LARGE 32

//  by using "typedef", we don't have to write "unsigned int" every single time.
//  makes our code way more neat!
typedef unsigned int uint;

Matrix Allocate2ndMatrix(int height, int width, int init);

void tiled_matmul(float*, const float*, const float*, uint, uint);

void one_tile(float*, const float*, const float*,  uint, uint, uint, uint);

void matmul( float*, const float*, const float*, uint, uint, uint);

////////////////////////////////////////////////////////////////////////////////
//! C = A * B
//! @param C          result matrix
//! @param A          matrix A
//! @param B          matrix B
//! @param hA         height of matrix A
//! @param wB         width of matrix B
////////////////////////////////////////////////////////////////////////////////

/* You'll need to modify this function such that matrix B is accessed
* correctly once you change the memory layout to column-major. */
void matmul(float* C, const float* A, const float* B, uint hA, uint wA, uint wB)
{
   for (uint i = 0; i < hA; ++i)
      for (uint j = 0; j < wB; ++j) {
         double sum = 0;
         for (uint k = 0; k < wA; ++k) {
            double a = A[i * wA + k];
            double b = B[j * hA + k];
            sum += a * b;
         }
         C[i * wA + j] = (float)sum;
      }
}

// Allocate a matrix of dimensions height*width
Matrix Allocate2ndMatrix(int height, int width)
{
   Matrix M;
   M.width = M.pitch = width;
   M.height = height;
   int size = M.width * M.height;
   M.elements = NULL;

   M.elements = (float*) malloc(size*sizeof(float));

   /* This is a row-major allocation and initialization.
   * You need to modify this function which is only called
   * for Matrix B such that a column-major ordering is
   * performed. */

   //  this is now col major!
   for(uint i = 0; i < M.height; i++) {
      for(uint j = 0; j < M.width; j++) {
         M.elements[j*M.height + i] = (rand() / (float)RAND_MAX);
      }
   }
   return M;
}

void tiled_matmul (float *C, const float *A, const float *B, uint h, uint w) {
   for(uint m = 0; m < h/TILE_WIDTH_SMALL; m++){
      for(uint n = 0; n < w/TILE_WIDTH_SMALL; n++){
         one_tile(C, A, B, n, m, TILE_WIDTH_SMALL, w);
      }
   }
}

void one_tile(float *C, const float *A, const float *B, uint tile_xindex,
   uint tile_yindex, uint tile_width, uint mat_width) {
      for(uint j = 0; j < tile_width; j++){
         for(uint i = 0; i < tile_width; i++){
            double sum = 0;
            for(uint k = 0; k < mat_width; k++){
               double a = A[(tile_yindex * tile_width + i) * mat_width + k];
		double b = B [(tile_xindex * tile_width + j) * mat_width + k];
               sum += a * b;
            }
            C[(tile_yindex * tile_width + i) * mat_width + tile_xindex * tile_width + j] = (float)sum;
         }
      }
   }
