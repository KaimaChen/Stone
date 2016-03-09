#include "Stone.h"

real Angle2Radian(real angle) {
    return PI * angle / 180;
}

real cos_angle(real angle) {
    real radian = Angle2Radian(angle);
    return cos(radian);
}

real sin_angle(real angle) {
    real radian = Angle2Radian(angle);
    return sin(radian);
}

bool IsBetween(real num, real border1, real border2) {
    real little = min(border1, border2);
    real big = max(border1, border2);
    if( little <= num && num <= big )
        return true;
    else
        return false;
}

real Random(real start, real end) {
    return start + (end - start) * rand() / (RAND_MAX + 1.0);
}

real Clamp(real low, real up, real val) {
    assert(low <= up);

    if(val < low)
        return low;
    if(val > up)
        return up;
    
    return val;
}

bool Equal(real a, real b) {
    return abs(a - b) <= PRECITION;
}

bool BiasGreaterThan(real a, real b)
{
  const real k_biasRelative = 0.95f;
  const real k_biasAbsolute = 0.01f;
  return a >= b * k_biasRelative + a * k_biasAbsolute;
}

void DrawLine(Rigidbody *a, Rigidbody *b) {
    glBegin(GL_LINE_STRIP);
        glVertex2f(a->GetPos().x, a->GetPos().y);
        glVertex2f(b->GetPos().x, b->GetPos().y);
    glEnd();
}

void DrawEdge(Edge *e) {
    glBegin(GL_LINE_STRIP);
        Vector2 v1 = e->v1;
        Vector2 v2 = e->v2;
        glVertex2f(v1.x, v1.y);
        glVertex2f(v2.x, v2.y);
    glEnd();
}

void RenderString(int x, int y, const char *s) {
    glColor3f(1,0,0);
    glRasterPos2i(x, y);
    uint l = (uint)strlen(s);
    for(int i = 0; i < l; ++i)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(s + i));
}

string Double2String(double value) {
    std::ostringstream o;
    if (!(o << value))
        return "";
    return o.str();
}