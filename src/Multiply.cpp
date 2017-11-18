#include "../include/Matrix.hpp"
#include "../include/Multiply.hpp"
#include <iostream>

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
