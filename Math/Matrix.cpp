#include "Stone.h"

Mat22::Mat22(real radians) {
	Set(radians);
}

Mat22::Mat22(real a, real b, real c, real d) 
	: m00(a), m01(b), m10(c), m11(d) {}

void Mat22::Set(real radians) {
	real c = cos(radians);
	real s = sin(radians);

	m00 = c; m01 = -s;
	m10 = s; m11 = c;
}

Mat22 Mat22::Abs() const {
	return Mat22(abs(m00), abs(m01), abs(m10), abs(m11));
}

Vector2 Mat22::AxisX() const {
	return Vector2(m00, m10);
}

Vector2 Mat22::AxisY() const {
	return Vector2(m01, m11);
}

Mat22 Mat22::Transpose() const {
	return Mat22(m00, m10, m01, m11);
}

const Vector2 Mat22::operator*(const Vector2 &rhs) const {
	return Vector2(m00*rhs.x + m01*rhs.y, m10*rhs.x + m11*rhs.y);
}

const Mat22 Mat22::operator*(const Mat22 &rhs) const {
	return Mat22(
		m00*rhs.m00 + m01*rhs.m10,
		m00*rhs.m01 + m01*rhs.m11,
		m10*rhs.m00 + m11*rhs.m10,
		m10*rhs.m01 + m11*rhs.m11
	);
}