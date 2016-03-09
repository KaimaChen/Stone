#ifndef MATRIX_H
#define MATRIX_H

#include "Define.h"
class Vector;

class Mat22 {
public:
	real m00, m01;
	real m10, m11;
public:
	Mat22() {}
	Mat22(real radians);
	Mat22(real a, real b, real c, real d);
public:
	void Set(real radians);
	Mat22 Abs() const;
	Vector2 AxisX() const;
	Vector2 AxisY() const;
	Mat22 Transpose() const;
public:
	const Vector2 operator*(const Vector2 &rhs) const;
	const Mat22 operator*(const Mat22 &rhs) const;
};

#endif // MATRIX_H
