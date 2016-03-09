#include "Stone.h"

Vector2::Vector2() {
    x = 0.0f;
    y = 0.0f;
}

Vector2& Vector2::operator=(const Vector2 &p) {
    x = p.x; y = p.y;
    return *this;
}

Vector2 Vector2::operator +(const Vector2& p) const {
    return Vector2(x+p.x, y+p.y);
}

Vector2& Vector2::operator +=(const Vector2& p) {
    x += p.x;
    y += p.y;
    return *this;
}

Vector2 Vector2::operator -(const Vector2& p) const {
    return Vector2(x-p.x, y-p.y);
}

Vector2& Vector2::operator -=(const Vector2& p) {
    x -= p.x;
    y -= p.y;
    return *this;
}

Vector2 Vector2::operator *(float f) const {
    return Vector2(x*f, y*f);
}

Vector2& Vector2::operator *=(float f) {
    x *= f;
    y *= f;
    return *this;
}

Vector2 Vector2::operator /(float f) const {
    return Vector2(x/f, y/f);
}

Vector2& Vector2::operator/=(float f) {
    x /= f;
    y /= f;
    return *this;
}

Vector2 Vector2::operator-() const {
    return Vector2(-x, -y);
}

bool Vector2::operator==(const Vector2& p) const {
    return x == p.x && y == p.y;
}

bool Vector2::operator!=(const Vector2& p) const {
    return !(*this == p);
}

void Vector2::Normalize() {
    float magSq = x*x + y*y;
    if (magSq > 0.0f) { //防除0
        float oneOverMag = 1.0f / sqrt(magSq);
        x *= oneOverMag;
        y *= oneOverMag;
    }
}

float Vector2::operator *(const Vector2 &a) const {
    return x * a.x + y * a.y;
}

real Vector2::LenSqr() const {
    return (x*x + y*y);
}

void Vector2::Set(real px, real py) {
    x = px;
    y = py;
}

//================非成员函数====================

Vector2 operator*(real s, const Vector2 &v) {
    return Vector2(s * v.x, s * v.y);
}

float VectorMag(const Vector2 &a) {
    return sqrt(a.x*a.x + a.y*a.y);
}

float VectorMagSq(const Vector2 &a) {
    return (a.x*a.x + a.y*a.y);
}

float DistanceSquared(const Vector2 &a, const Vector2 &b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return (dx*dx + dy*dy);
}

float Distance(const Vector2 &a, const Vector2 &b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

float LineProjLen(const Vector2 &start, const Vector2 &end, const Vector2 &axis) {
    float p1 = start * axis;
    float p2 = end * axis;
    return (p1 - p2) > 0 ? (p1 - p2) : (p2 - p1);
}

float Cross(const Vector2 &a, const Vector2 &b) {
    return a.x * b.y - a.y * b.x;
}

Vector2 Cross(const Vector2 &a, float s) {
    return Vector2(s * a.y, -s * a.x);
}

Vector2 Cross(float s, const Vector2 &a) {
    return Vector2(-s * a.y, s * a.x);
}

Vector2 Min(const Vector2 &a, const Vector2 &b) {
    real xMin = min(a.x, b.x);
    real yMin = min(a.y, b.y);
    return Vector2(xMin, yMin);
}

Vector2 Max(const Vector2 &a, const Vector2 b) {
    real xMax = max(a.x, b.x);
    real yMax = max(a.y, b.y);
    return Vector2(xMax, yMax);
}

real GetRadiansBetween(const Vector2 &a, const Vector2 &b) {
    real c = (a * b) / (VectorMag(a) * VectorMag(b));
    return acos(c);
}

void PrintVec(string msg, const Vector2 &a) {
    cout << msg << " " << a.x << " : " << a.y << endl;
}

real Dot(const Vector2 &a, const Vector2 &b) {
    return a * b;
}