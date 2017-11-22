#ifndef __SPECIALMATRICES_HPP__
#define __SPECIALMATRICES_HPP__

#include "Matrix.hpp"
#include "SpecialMatrices.hpp"
#include <time.h>
#include <stdlib.h>

/*
  This file generates special matrices (i.e. identity matrix, Householder, etc.)
*/


/*
  Identity matrix
*/


template <typename T>
Matrix<T> Identity(const long n){
  T ** a;

  a = new T*[n];
  for (long i = 0; i < n; i++){
    a[i] = new T[n];
    for (long j = 0; j < n; j++){
      if (i == j){
	a[i][j] = 1;
      }
	
    }
  }

  return Matrix<T>(n,n,a);
}


/*
  Random square matrix between 0 and max 
*/

template <typename T>
Matrix<T> RandomUniformSquare(const long n, int max, int seed){
  T ** a;

  srand(seed);

  a = new T*[n];
  for (long i = 0; i < n; i++){
    a[i] = new T[n];
  }

  for (long i = 0; i < n; i++){
    for (long j = 0; j < n; j++){
      a[i][j] = rand() % (max + 1);
    }
  }
  
  return Matrix<T>(n,n,a);
}


#endif
