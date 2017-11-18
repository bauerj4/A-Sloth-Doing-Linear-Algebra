#include "../include/Matrix.hpp"

/*
  This program performs some tests for our
  linear algebra routines.
*/

void Test0();

int main(int argc, char ** argv){

  // Test 0: Matrix addition

  Test0();

  return 0; // Success
}


void Test0(){
  short passed;
  long n,m;

  n = m = 7;
  Matrix<float> matA(n,m);
  Matrix<float> matB(n,m);
  //Matrix<float> matC(n,m);

  /*
    Some data for matB
  */

  for (int i = 0; i < n; i++){
    for (int j = 0; j < m; j++){
      if ((i + j) % 3 == 0)
	matB.data[i][j] = 1;
    }
  }

  passed = 1;

  /*
    Check result of adding 0 matrix
  */

  auto matC = matA - matB;

  for (int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      if (matC.data[i][j] != -matB.data[i][j])
	passed = 0;
    }
  }

  std::cout << "Test 1 (Matrix Subtraction): ";
  if (passed)
    std::cout << "PASSED" << std::endl;
  else
    std::cout << "FAILED" << std::endl;
}
