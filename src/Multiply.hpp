#ifndef __MULTIPLY_HPP__
#define __MULTIPLY_HPP__


#include "Matrix.hpp"
#include "LinAlgOptions.hpp"
#include <iostream>
#include <omp.h>

//template <class T> void NaiveMatrixMultiplySerial(Matrix<T> &a, Matrix<T> &b, Matrix<T> &c);
template <class T> void NaiveMatrixMultiplySerial(Matrix<T> &a, Matrix<T> &b, Matrix<T> &c){

  if (c.nRows != a.nRows || c.nCols != b.nCols || a.nCols != b.nRows){
    std::cout << "Incompatible multiplication dimensions. Exiting." << std::endl;
    exit(1);
  }

  /*
    Apply the mathematical definition of matrix multiplication
  */

  for (int i = 0; i < a.nRows; i++){
    for (int j = 0; j < b.nCols; j++){
      for (int k = 0; k < a.nCols; k++){
	c.data[i][j] += a.data[i][k] * b.data[k][j];
      }
    }
  }
}


/*
  Use OpenMP to parallelize the Naive calculation
*/

template <class T> void NaiveMatrixMultiplyOMPCPU(Matrix<T> &a, Matrix<T> &b, Matrix<T> &c){

  if (c.nRows != a.nRows || c.nCols != b.nCols || a.nCols != b.nRows){
    std::cout << "Incompatible multiplication dimensions. Exiting." << std::endl;
    exit(1);
  }

  /*                                                        
    Apply the mathematical definition of matrix multiplication                                                      
  */

#ifdef OPEN_MP
#pragma omp parallel for
#endif // OPEN_MP
  for (int i = 0; i < a.nRows; i++){
    for (int j = 0; j < b.nCols; j++){
      for (int k = 0; k < a.nCols; k++){
        c.data[i][j] += a.data[i][k] * b.data[k][j];
      }
    }
  }
}


template <class T> void StrassenMatrixMultiplySerial(Matrix<T> &a, Matrix<T> &b, Matrix<T> &c){

  long thisDim;
  /*
    Use the submatrix function to partition the matrices
  */

  auto aPad = a.PaddedWithZeros();
  auto bPad = b.PaddedWithZeros();
  auto cPad = c.PaddedWithZeros();


  if (std::min(a.nRows, b.nCols) <= STRASSEN_MIN_DIM){
    NaiveMatrixMultiplySerial(a,b,c);
  }

  else{


    auto a11  = aPad.Submatrix(0, aPad.nRows/2, 0, aPad.nCols/2);    
    auto a21  = aPad.Submatrix(aPad.nRows/2, aPad.nRows, 0, aPad.nCols/2);
    auto a12  = aPad.Submatrix(0.,aPad.nRows/2,aPad.nCols/2,aPad.nCols);
    auto a22  = aPad.Submatrix(aPad.nRows/2,aPad.nRows,aPad.nCols/2,aPad.nCols);


    auto b11  = bPad.Submatrix(0, bPad.nRows/2, 0, bPad.nCols/2);
    auto b21  = bPad.Submatrix(bPad.nRows/2, bPad.nRows, 0, bPad.nCols/2);
    auto b12  = bPad.Submatrix(0.,bPad.nRows/2,bPad.nCols/2,bPad.nCols);
    auto b22  = bPad.Submatrix(bPad.nRows/2,bPad.nRows,bPad.nCols/2,bPad.nCols);


    thisDim   = aPad.nRows/2;
    
    auto m1   = Matrix<T>(thisDim, thisDim);
    auto m2   = Matrix<T>(thisDim, thisDim);
    auto m3   = Matrix<T>(thisDim, thisDim);
    auto m4   = Matrix<T>(thisDim, thisDim);
    auto m5   = Matrix<T>(thisDim, thisDim);
    auto m6   = Matrix<T>(thisDim, thisDim);
    auto m7   = Matrix<T>(thisDim, thisDim);
    

    /*
      Recursive call 1
    */
    auto a11PA22  = a11 + a22;
    auto b11PB22  = b11 + b22;
    
    StrassenMatrixMultiplySerial(a11PA22, b11PB22, m1);

    /*
      Recurseive call 2
    */

    auto a21PA22  = a21 + a22;

    StrassenMatrixMultiplySerial(a21PA22, b11, m2);

    /*
      Recursive call 3
    */


    auto b12MB22 = b12 - b22;

    StrassenMatrixMultiplySerial(a11, b12MB22, m3);


    /*
      Recursive call 4
    */

    auto b21MB11 = b21 - b11;

    StrassenMatrixMultiplySerial(a22, b21MB11,m4);


    /*
      Recursive call 5
    */

    auto a11PA12 = a11 + a12;

    StrassenMatrixMultiplySerial(a11PA12,b22,m5);


    /*
      Recursive call 6
    */

    auto a21MA11 = a21 - a11;
    auto b11PB12 = b11 + b12;

    StrassenMatrixMultiplySerial(a21MA11,b11PB12,m6);


    /*
      Recursive call 7
    */


    auto a12MA22 = a12 - a22;
    auto b21PB22 = b21 + b22;

    StrassenMatrixMultiplySerial(a12MA22, b21PB22,m7);


    /*
      Reconstruct the matrix
    */

    StrassenReconstruct(m1,m2,m3,m4,m5,m6,m7,c);

    /*
    m1.Print();
    m2.Print();
    m3.Print();
    m4.Print();
    m5.Print();
    m6.Print();
    m7.Print();
    */
  }
  
}

template <class T> void StrassenMatrixMultiplyOMPCPU(Matrix<T> &a, Matrix<T> &b, Matrix<T> &c){

  long thisDim;
  auto aPad = a.PaddedWithZeros();
  auto bPad = b.PaddedWithZeros();
  auto cPad = c.PaddedWithZeros();


  if (std::min(a.nRows, b.nCols) <= STRASSEN_MIN_DIM){
    NaiveMatrixMultiplyOMPCPU(a,b,c);
  }

  else{


    auto a11  = aPad.Submatrix(0, aPad.nRows/2, 0, aPad.nCols/2);
    auto a21  = aPad.Submatrix(aPad.nRows/2, aPad.nRows, 0, aPad.nCols/2);
    auto a12  = aPad.Submatrix(0.,aPad.nRows/2,aPad.nCols/2,aPad.nCols);
    auto a22  = aPad.Submatrix(aPad.nRows/2,aPad.nRows,aPad.nCols/2,aPad.nCols);

    auto b11  = bPad.Submatrix(0, bPad.nRows/2, 0, bPad.nCols/2);
    auto b21  = bPad.Submatrix(bPad.nRows/2, bPad.nRows, 0, bPad.nCols/2);
    auto b12  = bPad.Submatrix(0.,bPad.nRows/2,bPad.nCols/2,bPad.nCols);
    auto b22  = bPad.Submatrix(bPad.nRows/2,bPad.nRows,bPad.nCols/2,bPad.nCols);


    thisDim   = aPad.nRows/2;

    auto m1   = Matrix<T>(thisDim, thisDim);
    auto m2   = Matrix<T>(thisDim, thisDim);
    auto m3   = Matrix<T>(thisDim, thisDim);
    auto m4   = Matrix<T>(thisDim, thisDim);
    auto m5   = Matrix<T>(thisDim, thisDim);
    auto m6   = Matrix<T>(thisDim, thisDim);
    auto m7   = Matrix<T>(thisDim, thisDim);

    /*                                  
      Recursive call 1                                                                                             
    */
    auto a11PA22  = a11 + a22;
    auto b11PB22  = b11 + b22;

    StrassenMatrixMultiplyOMPCPU(a11PA22, b11PB22, m1);

    /*                            
      Recurseive call  2                
    */

    auto a21PA22  = a21 + a22;

    StrassenMatrixMultiplyOMPCPU(a21PA22, b11, m2);


    /*      
      Recursive call 3  
    */


    auto b12MB22 = b12 - b22;

    StrassenMatrixMultiplyOMPCPU(a11, b12MB22, m3);


    /*          
      Recursive call 4   
    */

    auto b21MB11 = b21 - b11;

    StrassenMatrixMultiplyOMPCPU(a22, b21MB11,m4);


    /*      
      Recursive call 5  
    */

    auto a11PA12 = a11 + a12;

    StrassenMatrixMultiplyOMPCPU(a11PA12,b22,m5);


    /* 
      Recursive call 6          
    */

    auto a21MA11 = a21 - a11;
    auto b11PB12 = b11 + b12;

    StrassenMatrixMultiplyOMPCPU(a21MA11,b11PB12,m6);


    /*   
      Recursive call 7     
    */


    auto a12MA22 = a12 - a22;
    auto b21PB22 = b21 + b22;

    StrassenMatrixMultiplyOMPCPU(a12MA22, b21PB22,m7);


    /*    
      Reconstruct the matrix        
    */

    StrassenReconstruct(m1,m2,m3,m4,m5,m6,m7,c);



  }
}
template <typename T> void StrassenReconstruct(Matrix<T> &m1, Matrix<T> &m2, Matrix<T> &m3,\
					       Matrix<T> &m4, Matrix<T> &m5, Matrix<T> &m6,\
					       Matrix<T> &m7, Matrix<T> &result){

  long nRows, nCols;
  long dim = m1.nRows;

  long c1Count, c2Count, c3Count, c4Count;
  nRows = result.nRows;
  nCols = result.nCols;

  c1Count = c2Count = c3Count = c4Count = 0;
  for (long i = 0; i < 2 * dim; i++){
    for (long j = 0; j < 2 * dim; j++){
      if (i < nRows && j < nCols && i < dim && j < dim){ // We are in C11

	result.data[i][j] = m1.data[i][j] + m4.data[i][j] - m5.data[i][j] + m7.data[i][j];
	c1Count++;
      }

      else if (i < nRows && j < nCols && i < dim && j >= dim){ // We are in C12
	result.data[i][j] = m3.data[i][j-dim] + m5.data[i][j-dim];
	c2Count++;
      }

      else if (i < nRows && j < nCols && i >= dim && j < dim){ // We are in C21
	result.data[i][j] = m2.data[i-dim][j] + m4.data[i-dim][j];
	c3Count++;
      }

      else if (i < nRows && j < nCols && i >= dim && j >= dim){ // We are in C22
	result.data[i][j] = m1.data[i-dim][j-dim] - m2.data[i-dim][j-dim]
	  + m3.data[i-dim][j-dim] + m6.data[i-dim][j-dim];
	c4Count++;
      }
    }
  }
  //std::cout << c1Count << "," << c2Count << "," << c3Count << "," << c4Count << std::endl;
}
#endif
