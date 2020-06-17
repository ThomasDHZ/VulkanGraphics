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

	T& operator[](int i)
	{
		assert(i < 2);
		return((&x)[i]);
	}

	const T& operator[](int i) const
	{
		assert(i < 2);
		return((&x)[i]);
	}

	Vector2D& operator*=(int scaler)
	{
		x = scaler;
		y = scaler;
		return *this;
	}

	Vector2D& operator*(int scaler)
	{
		return Vector2D(x * scaler, y * scaler);
	}

	Vector2D& operator/=(int scaler)
	{
		scaler = 1.0f / scaler;
		x = scaler;
		y = scaler;
		return *this;
	}
};

typedef Vector2D<int> IVec2D;
typedef Vector2D<float> Vec2D;

