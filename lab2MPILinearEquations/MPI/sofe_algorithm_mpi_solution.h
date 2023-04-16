#ifndef MPI_SOFE_ALGORITHM_MPI_SOLUTION_H
#define MPI_SOFE_ALGORITHM_MPI_SOLUTION_H

#include "class_mpi_matrix.h"

void PrepareSystem(int, char*[], MPI_Matrix&, MPI_Vector&);
double GetNextAlpha(double, const MPI_Matrix&, const MPI_Vector&);MPI_Vector GetNextVectorX(const MPI_Vector&, double, const MPI_Vector&);
MPI_Vector GetNextVectorR(const MPI_Vector&, double, const MPI_Matrix&, const MPI_Vector&);
double GetNextBeta(double, const MPI_Vector&);
MPI_Vector GetNextVectorZ(const MPI_Vector&, double, const MPI_Vector&);
MPI_Vector ConjugatedGradientMethod(const MPI_Matrix&, const MPI_Vector&, const MPI_Vector&);
int LabMPI(int, char**);

#endif
