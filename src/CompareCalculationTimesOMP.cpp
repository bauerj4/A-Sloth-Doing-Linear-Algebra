#include "Matrix.hpp"
#include "Multiply.hpp"
#include "SpecialMatrices.hpp"
#include <iostream>
#include <ctime>
#include <chrono>
#include <omp.h>
#include <time.h>
#include <vector>

/*
  Get time scaling for different methods
*/

int main(int argc, char ** argv){

  clock_t tStart, tEnd;
  /*
    Naive
  */

  std::vector<long> sizes(13);

  sizes[0]  = 1;
  sizes[1]  = 2;
  sizes[2]  = 4;
  sizes[3]  = 8;
  sizes[4]  = 16;
  sizes[5]  = 32;
  sizes[6]  = 64;
  sizes[7]  = 128;
  sizes[8]  = 256;
  sizes[9]  = 512;
  sizes[10] = 1024;
  sizes[11] = 2048;
  sizes[12] = 4096;
  //  sizes[13] = 8192;
  
  for (auto i: sizes){

    Matrix<float> a = RandomUniformSquare<float>(i,1337,1337);
    Matrix<float> b(i,i);
    Matrix<float> c(i,i);

    //t = clock();
    tStart  = clock();
    NaiveMatrixMultiplyOMPCPU(a,b,c);
    tEnd = clock();
    

    std::cout << i << " " << (float)(tEnd - tStart)/CLOCKS_PER_SEC << std::endl;
  }

  for (auto i: sizes){
    Matrix<float> a = RandomUniformSquare<float>(i,1337,1337);
    Matrix<float> b(i,i);
    Matrix<float> c(i,i);
    tStart  = clock();
    StrassenMatrixMultiplyOMPCPU(a,b,c);
    tEnd = clock();

    std::cout << i << " " << (float)(tEnd - tStart)/CLOCKS_PER_SEC << std::endl;

  }
}
