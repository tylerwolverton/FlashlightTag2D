#pragma once
#include "Vector2D.h"
#include <iostream>
#include <vector>

// A 4x4 matrix class in row major form
template <class T>
class Matrix4
{
public:
    static const int MATRIX_SIZE = 4;

    Matrix4() 
    {
        T newElements[MATRIX_SIZE][MATRIX_SIZE] = 
                   { {1,0,0,0},
                     {0,1,0,0},
                     {0,0,1,0},
                     {0,0,0,1} };

        copyElementArray(newElements, Elements);
    }

    Matrix4(T newElements[MATRIX_SIZE][MATRIX_SIZE])
    {
        /*T newElements[MATRIX_SIZE][MATRIX_SIZE] =
        { { 1,0,0,0 },
        { 0,1,0,0 },
        { 0,0,1,0 },
        { 0,0,0,1 } };
*/
        copyElementArray(newElements, Elements);
    }

    Matrix4(const Matrix4<T>& inMatrix)
    {
        copyElementArray(inMatrix.Elements, Elements);
    }

    Matrix4<T>& operator=(const Matrix4<T>& inMatrix)
    {
        Matrix4<T> newMatrix;
        copyElementArray(inMatrix.Elements, Elements);
        return *this;
    }

    ~Matrix4() {}

    T Elements[MATRIX_SIZE][MATRIX_SIZE];

    std::unique_ptr<T[]> GetPtrToFlattenedData()
    {
        std::unique_ptr<T[]> pflat(new T[MATRIX_SIZE * MATRIX_SIZE]);
        T* flat = new T[MATRIX_SIZE * MATRIX_SIZE];
        //std::cout << "Flattened ****************************" << std::endl;
        int k = 0;
        for (int i = 0; i < MATRIX_SIZE; i++)
        {
            for (int j = 0; j < MATRIX_SIZE; j++)
            {
                //(pflat.get())[k] = Elements[i][j];
                flat[k] = Elements[i][j];
                ++k;

                //std::cout << pflat[k] << ", ";
                //std::cout << "flat[k]: " << flat[k-1] << ", Elements[i][j]: " << Elements[i][j] <<std::endl;
            }
        }

        //std::cout << "****************************" << std::endl;
        return std::unique_ptr<T[]>(flat);
    }

    Matrix4<T> Translate(Vector2D<T> pos)
    {
        Matrix4<T> newMatrix(*this);

        newMatrix.Elements[3][0] += pos.x;
        newMatrix.Elements[3][1] += pos.y;

        return newMatrix;
    }

    Matrix4<T> Scale(Vector2D<T> size)
    {
        Matrix4<T> newMatrix(*this);

        newMatrix.Elements[0][0] *= size.x;
        newMatrix.Elements[1][1] *= size.y;

        return newMatrix;
    }

    Matrix4<T> operator*(const Matrix4<T>& inMatrix)
    {
        Matrix4<T> newMatrix;
        for (int i = 0; i < MATRIX_SIZE; i++)
        {
            for (int j = 0; j < MATRIX_SIZE; j++)
            {
                newMatrix.Elements[i][j] =
                    Elements[i][0] * inMatrix.Elements[0][j] +
                    Elements[i][1] * inMatrix.Elements[1][j] +
                    Elements[i][2] * inMatrix.Elements[2][j] +
                    Elements[i][3] * inMatrix.Elements[3][j];
            }
        }

        return newMatrix;
    }

    static std::unique_ptr<Matrix4<T>> CreateOrthoMatrix(float left, float right, float top, float bottom, float near, float far)
    {
        T newElements[MATRIX_SIZE][MATRIX_SIZE] =
                      { { 2/(right - left),0,0,0 },
                        { 0,2/(top - bottom),0,0 },
                        { 0,0,-2/(far - near),0 },
                        { -(right + left)/(right - left),-(top + bottom)/(top - bottom),-(far + near)/(far - near),1 } };
        
        std::unique_ptr<Matrix4<T>> pProj(new Matrix4<T>(newElements));

        //Matrix4<T> proj;
        //copyElementArray(newElements, proj.Elements);

        return pProj;
    }

    void Print()
    {
        std::cout << "****************************" << std::endl;
        for (int i = 0; i < MATRIX_SIZE; i++)
        {
            for (int j = 0; j < MATRIX_SIZE; j++)
            {
                std::cout << Elements[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "****************************" << std::endl;
    }

private:
    void copyElementArray(const T sourceElements[MATRIX_SIZE][MATRIX_SIZE], T targetElements[MATRIX_SIZE][MATRIX_SIZE])
    {
        for (int i = 0; i < MATRIX_SIZE; i++)
        {
            for (int j = 0; j < MATRIX_SIZE; j++)
            {
                targetElements[i][j] = sourceElements[i][j];
            }
        }
    }
};

