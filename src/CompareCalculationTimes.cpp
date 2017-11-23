#include "Matrix.hpp"
#include "Multiply.hpp"
#include "SpecialMatrices.hpp"
#include <iostream>
#include <ctime>
#include <chrono>
#include <omp.h>
#include <time.h>


/*
  Get time scaling for different methods
*/

int main(int argc, char ** argv){

  clock_t tStart, tEnd;
  /*
    Naive
  */
  
  for (long i = 0; i < 2048; i += 256){

    Matrix<float> a = RandomUniformSquare<float>(i,1337,1337);
    Matrix<float> b(i,i);
    Matrix<float> c(i,i);

    //t = clock();
    tStart  = clock();
    NaiveMatrixMultiplySerial(a,b,c);
    tEnd = clock();
    

    std::cout << i << " " << (float)(tEnd - tStart)/CLOCKS_PER_SEC << std::endl;
  }

  for (long i = 0; i < 2048; i += 256){
    Matrix<float> a = RandomUniformSquare<float>(i,1337,1337);
    Matrix<float> b(i,i);
    Matrix<float> c(i,i);
    tStart  = clock();
    StrassenMatrixMultiplySerial(a,b,c);
    tEnd = clock();

    std::cout << i << " " << (float)(tEnd - tStart)/CLOCKS_PER_SEC << std::endl;

  }
}
