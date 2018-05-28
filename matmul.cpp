#include <stdlib.h>
#include "matmul.h"
#define WIDTH 16

Matrix Allocate2ndMatrix(int height, int width, int init);

void tiled_matmul(float*, const float*, const float*, unsigned int, unsigned int);

void one_tile(float*, const float*, const float*, unsigned in, unsigned int, unsigned int);

void matmul( float*, const float*, const float*, unsigned int, unsigned int, unsigned int);

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
void matmul(float* C, const float* A, const float* B, unsigned int hA, 
    unsigned int wA, unsigned int wB)
{
  for (unsigned int i = 0; i < hA; ++i)
    for (unsigned int j = 0; j < wB; ++j) {
      double sum = 0;
      for (unsigned int k = 0; k < wA; ++k) {
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
  for(unsigned int j = 0; j < M.width; j++)
  {
	for(unsigned int i = 0; i < M.height; i++)
	{
   	 M.elements[j*M.height + i] = (rand() / (float)RAND_MAX);
	}
  }
  return M;
}

void tiled_matmul (float *C, const float *A, const float *B, unsigned int h,
		unsigned int w){
	for(unsigned int m = 0; m < h/WIDTH; m++){
		for(unsigned int n = 0; n < w/WIDTH; n++){
			one_tile(C, A, B, n, m, WIDTH);
		}
	}
}

void one_tile(float *C, const float *A, const float *B, 
		unsigned int tile_xindex, unsigned int tile_yindex,
		unsigned int tile_width){
	for(unsigned int j = 0; j < tile_width; j++){
		for(unsigned int i = 0; i < tile_width; i++){
			double sum = 0;
			for(unsigned int k = 0; k < tile_width; k++){
				double a = A[(k * tile_width + tile_yindex) * tile_width + i];
				double b = B[(tile_xindex * tile_width + j) * tile_width + tile_yindex];
				sum += a * b;
			}
			C[((tile_xindex * tile_width + j) * tile_width + tile_yindex) * tile_width + i] = (float)sum;
		}
	}
}
