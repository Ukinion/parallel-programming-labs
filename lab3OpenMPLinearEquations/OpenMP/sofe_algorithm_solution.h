#ifndef NOMPI_SOFE_ALGORITHM_SOLUTION_H
#define NOMPI_SOFE_ALGORITHM_SOLUTION_H

#include "class_matrix.h"
#include "mpi.h"

double GetNextAlpha(double, const Matrix &, const Vector&);
Vector GetNextVectorX(const Vector&, double, const Vector&);
Vector GetNextVectorR(const Vector&, double, const Vector&, const Vector&);
double GetNextBeta(double, const Vector&);
Vector GetNextVectorZ(const Vector&, double, const Vector&);
Vector ConjugatedGradientMethod(const Vector&, const Vector&, const Vector&);
int LabOpenMP(int, char**);

#endif
