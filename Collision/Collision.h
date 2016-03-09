#ifndef COLLISION_H
#define COLLISION_H

#include "Define.h"
#include "Vector.h"
#include <vector>
using namespace std;

class Circle;
class Polygon;
struct Contact;
struct Rigidbody;
class Edge;

void CircleVsCircle(Contact *contact, Circle *a, Circle *b);
void CircleVsPolygon(Contact *contact, Circle *a, Polygon *b);
void PolygonVsCircle(Contact *contact, Polygon *a, Circle *b);
void PolygonVsPolygon(Contact *contact, Polygon *a, Polygon *b);

bool CircleVsEdge(Circle *c, Edge *e);
bool PolygonVsEdge(Polygon *p, Edge *e);

class Edge {
public:
    Vector2 max;
    Vector2 v1;
    Vector2 v2;
public:
    Edge() {}
    Edge(Vector2 m, Vector2 v1, Vector2 v2)
        : max(m), v1(v1), v2(v2) {}
    Edge(Vector2 v1, Vector2 v2)
        : v1(v1), v2(v2) {}

    Vector2 GetEdge() {
        return v2 - v1;
    }

    Vector2 Max() {
        return v1;
    }
};

class ClippedPoints {
public:
    vector<Vector2> points;

    ClippedPoints() {};

    void Add(Vector2 p) {
        points.push_back(p);
    }

    void Remove(Vector2 p) {
        int index = 0;
        for(auto i = points.begin(); i < points.end(); i++) {
            if(*i == p) {
                points.erase(i);
                return;
            }
            index++;
        }
    }

    int Length() {
        return points.size();
    }
    
    Vector2 operator[](int index) {
        return points[index];
    }
};

// --helper function-- //

//@Func: Proj the polygon's points on the axis, and find the overlap length
bool CheckOverlapOnAxis(Vector2 axis, Polygon *p1, Polygon *p2, real &overlapLen);
bool CheckOverlapOnAxis(Vector2 axis, Polygon *p, Circle *c, real &overlapLen);
//@Func: Cal the overlap length 
bool IsOverlap(real min1, real max1, real min2, real max2, real &overlapLen);
//@Func: Find the nearest edge to the contact
Edge FindNearestEdge(Polygon *p, Vector2 axis);
//@Func: Find the biggest proj len of index on aixs
int FindFurthest(Polygon *poly, Vector2 axis);
//@Func: Clip the edge to find the specific contact points
ClippedPoints Clip(Vector2 v1, Vector2 v2, Vector2 normal, real offset);
//@Func: Find the contact points between polygons
ClippedPoints FindPolygonPoints(Polygon *p1, Polygon *p2, Vector2 normal);

#endif // COLLISION_H