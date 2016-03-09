/*
Desc: 向量类，向量可以当作点Point来使用
Note：参考《3D数学基础》
*/

#ifndef VECTOR2_H_INCLUDED
#define VECTOR2_H_INCLUDED

#include "Define.h"
#include <string>
using std::string;

class Vector2 {
public:
    float x;
    float y;
public:
    Vector2();
    Vector2(float px, float py) : x(px), y(py) {}

    Vector2 &operator =(const Vector2 &);
    Vector2 operator +(const Vector2&) const;
    Vector2& operator +=(const Vector2&);
    Vector2 operator -(const Vector2&) const;
    Vector2& operator -=(const Vector2&);
    Vector2 operator *(float) const;
    float operator *(const Vector2 &) const;
    Vector2& operator *=(float);
    Vector2 operator /(float) const;
    Vector2& operator /=(float);
    Vector2 operator -() const;
    bool operator ==(const Vector2&) const;
    bool operator !=(const Vector2&) const;
    
    void Set(real px, real py);
    void Normalize();
    real LenSqr() const;
};


//================非成员函数====================

//@Func: multiply vector and real
Vector2 operator*(real s, const Vector2 &v);

//@Func:向量模
float VectorMag(const Vector2 &a);

//@Func:Get the Magnitude square of vector a
float VectorMagSq(const Vector2 &a);

//@Func: 两点距离的平方
float DistanceSquared(const Vector2 &a, const Vector2 &b);

//@Func:两点间距离
float Distance(const Vector2 &a, const Vector2 &b);

//@Func: get the line proj len on axis
float LineProjLen(const Vector2 &start, const Vector2 &end, const Vector2 &axis);

//@Func: vectorA cross vectorB
float Cross(const Vector2 &a, const Vector2 &b);

//@Func: vector cross scale
Vector2 Cross(const Vector2 &a, float s);

//@Func: scale cross vector
Vector2 Cross(float s, const Vector2 &a);

//@Func: get the smaller part of two vector
Vector2 Min(const Vector2 &a, const Vector2 &b);

//@Func: get the bigger part of two vector
Vector2 Max(const Vector2 &a, const Vector2 b);

//@Func: get the radians between a and b
real GetRadiansBetween(const Vector2 &a, const Vector2 &b);

//------------------Test function--------------------------

//@Func:
void PrintVec(string msg, const Vector2 &a);

real Dot(const Vector2 &a, const Vector2 &b);


#endif // VECTOR2_H_INCLUDED