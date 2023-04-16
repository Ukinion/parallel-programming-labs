#include "matrix_generation.h"


int main(int argc, char* argv[])
{
    int N = std::stoi(argv[1]);
    int load = std::stoi(argv[2]);

    GenerateVectorFile(N, 1);
    GenerateSymmetricMatrixFile(N, load, 1);

    return 0;
}
