#ifndef __MATRIX_HPP__
#define __MATRIX_HPP__

#include <iostream>
#include <thread>
#include <omp.h>
#include <cstdlib>
#include <cmath>
//#include "Multiply.hpp"

/*
  Define the class template for a matrix.
  We want a template to allow for different
  numerical precisions and complex numbers.
*/

template <class T>
class Matrix{
public:
  T ** data;
  long nRows;
  long nCols;

  // Constructor
  Matrix(long n, long m){
    nRows = n;
    nCols = m;

    // Dynamically allocate memory

    data = new T*[nRows];
    for (int i = 0; i < nRows; i++){
      data[i] = new T [nCols];
      for (int j = 0; j < nCols; j++){
	data[i][j] = 0;
      }
    }
  }

  // Alternate constructor if we have an array

  Matrix(long n, long m, T ** tmpData){
    nRows = n;
    nCols = m;
    data  = tmpData;
  }


  // Destructor
  ~Matrix(){

    // Free rows

    for (int i = 0; i < nRows; i++){
      delete data[i];
    }

    // Free columns
    delete data;
  }


  // Print matrix w/ std::cout

  void Print(){
    for (int i = 0; i < nRows; i++){
      std::cout << "[ ";
      for (int j = 0; j < nCols; j++){
	std::cout << data[i][j] << " ";
      }
      std::cout << "]\n";
    }
    std::cout << "\n";
  }

  // Create a matrix slice [iLow,iHigh) X [jLow, jHigh)

  Matrix<T> Submatrix(long iLow, long iHigh, long jLow, long jHigh){
    T ** newData;
    
    if (iLow < 0 || iHigh < 0 || jLow < 0 || jHigh < 0){
      std::cout << "Invalid slice: cannot have negative indices." << std::endl;
      exit(1);
    }

    else if (iLow > iHigh || jLow > jHigh){
      std::cout << "Invalid slice: iLow must be greater than iHigh, or jLow must be greater than jHigh"\
		<< std::endl;
    }

    else if (iHigh > nRows || jHigh > nCols){
      std::cout << "Invalid slice: iHigh and jHigh must not exceed matrix dimensions." << std::endl;
    }

    newData = new T*[iHigh - iLow];
    for (long i = iLow; i < iHigh; i++){
      newData[i - iLow] = new T[jHigh - jLow];
      for (long j = jLow; j < jHigh; j++){
	//std::cout << i  << "," << j << std::endl;
	newData[i - iLow][j - jLow] = data[i][j];
      }
    }
    return Matrix(iHigh - iLow,  jHigh - jLow, newData);

  }


  // Return a 2^N X 2^N matrix padded with zeros

  Matrix<T> PaddedWithZeros(){
    long n;
    T ** resultData;


    //if (nRows % 2 != 0 || nCols % 2 != 0){
    
    n = std::max(std::ceil(std::log2(nRows)),std::ceil(std::log2(nCols)));
    n = (long)std::pow(2,n);
    
    // std::cout << "Padding " << nRows << " X " << nCols << " matrix to be " 
    //	      << n << " X " << n << std::endl;
    
    resultData = new T*[n];
    for (long i = 0; i < n; i++){
      resultData[i] = new T [n];
      for (long j = 0; j < n; j ++){
	if (i < nRows && j < nCols)
	  resultData[i][j] = data[i][j];
	else
	  resultData[i][j] = 0;
      }
    }
    
    
    auto padded = Matrix<T>(n,n,resultData);
    return padded;
    
  }
};


/*
  The addition operator. Performs dimension check and then returns
  a new matrix that is the sum of two others.  Use += if you want 
  to increment.
*/

template <class T>
Matrix<T>  operator+(Matrix<T>  & firstMatrix, Matrix<T>  & secondMatrix){
  if (firstMatrix.nRows != secondMatrix.nRows ||
      firstMatrix.nCols != secondMatrix.nCols){
    std::cout << "Incompatible dimensions for addition. Exiting. " << std::endl;
    exit(1);
  }

  const long n = firstMatrix.nRows;
  const long m = firstMatrix.nCols;

  /*
    Allocate the new memory for the 
    new matrix and use cast constructor
  */
  T ** resultData;
  resultData = new T*[n];
  for (long i = 0; i < n; i++){
    resultData[i] = new T [m];
  }


#ifdef OPEN_MP
#pragma omp parallel for
#endif
  for (long i = 0; i < n; i++){
    for (long j = 0; j < m; j++){
      resultData[i][j] = firstMatrix.data[i][j] + secondMatrix.data[i][j];
    }
  }

  Matrix<T> res(n,m,resultData);
  return res;
}

template <class T> void NaiveMatrixMultiplySerial(Matrix<T> &a, Matrix<T> &b, Matrix<T> &c);
template <class T> void NaiveMatrixMultiplyOMPCPU(Matrix<T> &a, Matrix<T> &b, Matrix<T> &c);
template <class T> void NaiveMatrixMultiplyOMPGPU(Matrix<T> &a, Matrix<T> &b, Matrix<T> &c);
template <class T> void StrassenMatrixMultiplySerial(Matrix<T> &a, Matrix<T> &b, Matrix<T> &c);

template <typename T> void StrassenReconstruct(Matrix<T> &m1, Matrix<T> &m2, Matrix<T> &m3,\
                                               Matrix<T> &m4, Matrix<T> &m5, Matrix<T> &m6,\
                                               Matrix<T> &m7, Matrix<T> &result);


/*
  The subtraction operator. Performs dimension check
  and then returns a new matrix that is the difference of
  the other two. Use -= if you don't want to allocate new memory
  in decrementing.
*/

template <class T>
Matrix<T> operator-(Matrix<T> const& firstMatrix, Matrix<T> const &secondMatrix){
  if (firstMatrix.nRows != secondMatrix.nRows ||
      firstMatrix.nCols != secondMatrix.nCols){
    std::cout << "Incompatible dimensions for subtraction. Exiting. " << std::endl;
    exit(1);
  }

  const long n = firstMatrix.nRows;
  const long m = firstMatrix.nCols;



  /*
    Allocate the new memory for the new
    matrix and use cast constructor
  */
  T ** resultData;
  resultData = new T*[n];
  for (int i = 0; i < n; i++){
    resultData[i] = new T [m];
  }

#ifdef OPEN_MP
#pragma omp parallel for
#endif

  for (long i = 0; i < n; i++){
    for (long j = 0; j < m; j++){
      resultData[i][j] = firstMatrix.data[i][j] - secondMatrix.data[i][j];
    }
  }

  Matrix<T> res(n,m,resultData);
  return res;
}



/*
  Performs matrix multiplication. By default, we consider the Strassen
  algorithm with a globally specified minimum block size. A block size
  larger than the matrix will just result in naive matrix multiplication.
*/

template <class T>
Matrix<T> operator*(Matrix<T> &firstMatrix, Matrix<T> &secondMatrix){

  T ** resultData;
  resultData = new T*[firstMatrix.nRows];
  for (int i = 0; i < firstMatrix.nRows; i++){
    resultData[i] = new T [secondMatrix.nCols];
  }

  auto thirdMatrix = Matrix<T>(firstMatrix.nRows, secondMatrix.nCols);
  NaiveMatrixMultiplySerial<T>(firstMatrix, secondMatrix, thirdMatrix);

  return thirdMatrix;

}




// Compile from template
extern Matrix<float>  MAT_FILL_FLOAT;
extern Matrix<double> MAT_FILL_DOUBLE;

#endif
