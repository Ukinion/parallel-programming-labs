#include "LinearVector.h"


int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    try
    {
        lab1::LinearVector<int> linearVector(constants::kTestVecSize);
        lab1::LinearVector<int> linearVector1(constants::kTestVecSize);
        lab1::Algorithm(linearVector, linearVector1);
    }
    catch (const exception::InvalidAlgorithmParameters& ex) { return 1; }

    MPI_Finalize();
    return 0;
}
