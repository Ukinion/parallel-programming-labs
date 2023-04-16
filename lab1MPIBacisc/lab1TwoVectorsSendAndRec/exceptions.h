#ifndef LAB1TWOVECTORSSENDANDREC_EXCEPTIONS_H
#define LAB1TWOVECTORSSENDANDREC_EXCEPTIONS_H


#ifndef LAB1TWOVECTORS_EXCEPTIONS_H
#define LAB1TWOVECTORS_EXCEPTIONS_H

#include <exception>
#include <iostream>


namespace exception
{
    class UnsupportedVectorSize : public std::exception
    {
    public:
        UnsupportedVectorSize() { std::cout << "Error!\nVector size must be greater or equal zero!\n"; }
    };

    class UnsupportedVectorType : public std::exception
    {
    public:
        UnsupportedVectorType() { std::cout << "Error!\nCannot operate with this type!\n"; }
    };

    class InvalidAlgorithmParameters : public std::exception
    {
    public:
        InvalidAlgorithmParameters() { std::cout << "Error!\nVectors size must be equal.\n"; }
    };

}


#endif


#endif //LAB1TWOVECTORSSENDANDREC_EXCEPTIONS_H
