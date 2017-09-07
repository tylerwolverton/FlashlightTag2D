#pragma once
#include "Vector2D.h"
#include <vector>

// A 4x4 matrix class in row major form
template <class T>
class Matrix4
{
public:
	Matrix4() 
	{
		Elements = { {1,0,0,0},
					 {0,1,0,0},
					 {0,0,1,0},
					 {0,0,0,1} };
	}

	Matrix4(const Matrix4<T>& inMatrix)
	{
		Elements = inMatrix.Elements;
	}

	~Matrix4() {}

	std::vector<std::vector<T> > Elements;

	Matrix4<T> Translate(Vector2D<T> pos)
	{
		Matrix4<T> newMatrix(*this);

		newMatrix.Elements[3][0] += pos.x;
		newMatrix.Elements[3][1] += pos.y;

		return newMatrix;
	}

	Matrix4<T> operator*(const Matrix4<T> inMatrix)
	{
		Matrix4<T> newMatrix;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				newMatrix.Elements[i][j] =
					this.Elements[i][0] * inMatrix.Elements[0][j] +
					this.Elements[i][1] * inMatrix.Elements[1][j] +
					this.Elements[i][2] * inMatrix.Elements[2][j] +
					this.Elements[i][3] * inMatrix.Elements[3][j];
			}
		}

		return newMatrix;
	}

private:

};

