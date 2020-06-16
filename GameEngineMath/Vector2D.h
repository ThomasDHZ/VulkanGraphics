#pragma once

template <class T>
struct Vector2D
{
	T x;
	T y;

	Vector2D()
	{
		x = 0;
		y = 0;
	}

	Vector2D(T XY)
	{
		x = XY;
		y = XY;
	}

	Vector2D(T X, T Y)
	{
		x = X;
		y = Y;
	}

};

