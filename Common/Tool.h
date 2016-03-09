/*
Desc: Some useful tool function
*/

#ifndef TOOLS_H_INCLUDED
#define TOOLS_H_INCLUDED

class Rigidbody;
class Edge;

//@Func: make angle to be radians
real Angle2Radian(real angle);

//@Func: use cos(angle) rather than cos(radians)
real cos_angle(real angle);

//@Func: use sin(angle) rather than sin(radians)
real sin_angle(real angle);

//@Func: to find is num between border1 and border2
bool IsBetween(real num, real border1, real border2);

//@Func: get random number between start and end
real Random(real start, real end);

//@Func: make sure the val is between low and up
real Clamp(real low, real up, real val);

//@Func: to find is a equal to b(because a and b is real, we can't use a == b)
bool Equal(real a, real b);

//@Func: use to avoid shaking
bool BiasGreaterThan(real a, real b);

//@Func: draw a line between a and b;
void DrawLine(Rigidbody *a, Rigidbody *b);

//@Func: draw edge
void DrawEdge(Edge *e);

//@Func: render string on screen
void RenderString(int x, int y, const char *s);

//@Func: convert double to string
string Double2String(double value);

#endif // TOOLS_H_INCLUDED