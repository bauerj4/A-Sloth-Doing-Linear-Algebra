#include "../src/Matrix.hpp"
#include "../src/Multiply.hpp"
#include "../src/SpecialMatrices.hpp"

void Test0();

int main(int argc, char ** argv){

  Test0();

  return 0; // success
}

void Test0(){
  float ** a;

  a = new float*[5];
  for (long i = 0; i < 5; i++){
    a[i] = new float[5];
    for (int j = 0; j < 5; j++){
      a[i][j] = i + j + 1;
    }
  }

  auto A = Matrix<float>(5,5,a);
  auto B = A * A;
  auto I = Identity<float>((const long)5);

  A.Print();
  B.Print();
  //I.Print();
  //A.Print();
  //B.Print();

  /*
    Generate simple matrix
  */

  Matrix<float> C(5,5);

  StrassenMatrixMultiplySerial(A,A,C);
  C.Print();
}
