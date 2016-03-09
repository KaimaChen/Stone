#include "Stone.h"

//note: Box2d
void CircleVsCircle(Contact *contact, Circle *a, Circle *b) {
    Vector2 normal = b->GetPos() - a->GetPos();
    real disSqr = normal.LenSqr();
    real radiusSum = a->GetRadius() + b->GetRadius();

    // not collide
    if(disSqr >= radiusSum * radiusSum) return;

    // collide
    real distance = sqrt(disSqr);
    normal.Normalize();

    contact->pointsCount = 1;
    contact->normal = normal;
    contact->penetration = radiusSum - distance;
    contact->points[0] = contact->normal * a->GetRadius() + a->GetPos();
    if(Equal(distance, 0.0f))
        contact->normal = Vector2(0,1);
}


//TODO : refactor
void CircleVsPolygon(Contact *contact, Circle *c, Polygon *p) {
     //SAT
    real overlapLen = FLT_MAX;
    Vector2 contactNormal;

    //every edge of polygon
    for(int i = 0, j = p->GetCount()-1; i < p->GetCount(); j=i, i++) {
        Vector2 edge = p->GetPoint(i) - p->GetPoint(j);
        Vector2 normal = Vector2(edge.y, -edge.x);
        normal.Normalize();

        real len = FLT_MAX;
        bool isOverlap = CheckOverlapOnAxis(normal, p, c, len);
        if(!isOverlap) return;

        if(len < overlapLen) {
            overlapLen = len;
            contactNormal = normal;
        }
    }
    //for all vertices to circle's center
    for(int i = 0; i < p->GetCount(); i++) {
        Vector2 normal = p->GetPoint(i) - c->GetPos();
        normal.Normalize();

        real len = FLT_MAX;
        bool isOverlap = CheckOverlapOnAxis(normal, p, c, len);
        if(!isOverlap) return;

        if(len < overlapLen) {
            overlapLen = len;
            contactNormal = normal;
        }
    }

    Vector2 d = p->GetPos() - c->GetPos();
    if(d * contactNormal < 0)
        contactNormal *= -1;

    contact->normal = contactNormal;
    contact->penetration = overlapLen;
    contact->pointsCount = 1;
    contact->points[0] = c->GetPos() + contact->normal * c->GetRadius();
}

void PolygonVsCircle(Contact *contact, Polygon *p, Circle *c) {
     //SAT
    real overlapLen = FLT_MAX;
    Vector2 contactNormal;

    for(int i = 0, j = p->GetCount()-1; i < p->GetCount(); j=i, i++) {
        Vector2 edge = p->GetPoint(i) - p->GetPoint(j);
        Vector2 normal = Vector2(edge.y, -edge.x);
        normal.Normalize();

        real len = 0;
        bool isOverlap = CheckOverlapOnAxis(normal, p, c, len);
        if(!isOverlap) return;

        if(len < overlapLen) {
            overlapLen = len;
            contactNormal = normal;
        }
    }
    //for all vertices to circle's center
    for(int i = 0; i < p->GetCount(); i++) {
        Vector2 normal = p->GetPoint(i) - c->GetPos();
        normal.Normalize();

        real len = FLT_MAX;
        bool isOverlap = CheckOverlapOnAxis(normal, p, c, len);
        if(!isOverlap) return;

        if(len < overlapLen) {
            overlapLen = len;
            contactNormal = normal;
        }
    }

    Vector2 d = c->GetPos() - p->GetPos();
    if(d * contactNormal < 0)
        contactNormal *= -1;

    contact->normal = contactNormal;
    contact->penetration = overlapLen;
    contact->pointsCount = 1;
    contact->points[0] = c->GetPos() - contact->normal * c->GetRadius();
}

bool CircleVsEdge(Circle *c, Edge *e) {
    return c->IntersectWithLine(e->v1, e->v2);
}

//TODO not so correct
bool PolygonVsEdge(Polygon *p, Edge *e) {
    Vector2 edge = e->v1 - e->v2;
    Vector2 normal = Vector2(edge.y, -edge.x);
    normal.Normalize();
    real val = e->v1 * normal;

    real min = p->GetPoint(0) * normal;
    real max = min;
    for(int i = 1; i < p->GetCount(); i++) {
        real d = p->GetPoint(i) * normal;
        if(d < min)
            min = d;
        if(d > max)
            max = d;
    }

    //return IsBetween(val, min, max);
    if(!IsBetween(val, min, max)) return false;

    edge.Normalize();
    real val1 = e->v1 * edge;
    real val2 = e->v2 * edge;
    min = (val1 < val2) ? val1 : val2;
    max = (val1 > val2) ? val1 : val2;

    real pmin = p->GetPoint(0) * edge;
    real pmax = pmin;
    for(int i = 1; i < p->GetCount(); i++) {
        real d = p->GetPoint(i) * edge;
        if(d < pmin)
            pmin = d;
        if(d > pmax)
            pmax = d;
    } 

    return IsOverlap(min, max, pmin, pmax, val);
}








void PolygonVsPolygon(Contact *contact, Polygon *p1, Polygon *p2) {
    real overlapLen = FLT_MAX;
    Vector2 contactNormal;

    //Attention: for(int i = 0; i < count-1; i++) is not work
    for(int i = 0, j = p1->GetCount()-1; i < p1->GetCount(); j=i, i++) {
        Vector2 edge = p1->GetPoint(i) - p1->GetPoint(j);
        Vector2 normal = Vector2(-edge.y, edge.x);
        normal.Normalize();

        real len = 0;
        bool isOverlap = CheckOverlapOnAxis(normal, p1, p2, len);
        if(!isOverlap)  return;

        if(len < overlapLen) {
            overlapLen = len;
            contactNormal = normal;
        }
    }

    for(int i = 0, j = p2->GetCount()-1; i < p2->GetCount(); j=i, i++) {
        Vector2 edge = p2->GetPoint(i) - p2->GetPoint(j);
        Vector2 normal = Vector2(-edge.y, edge.x);
        normal.Normalize();

        real len = 0;
        bool isOverlap = CheckOverlapOnAxis(normal, p1, p2, len);
        if(!isOverlap)  return;

        if(len < overlapLen) {
            overlapLen = len;
            contactNormal = normal;
        }
    }

    contact->penetration = overlapLen;

    Vector2 D = p1->GetPos() - p2->GetPos();
    //make sure the direction is true
    //because the normal can come from the other polygon
    if(D *contactNormal < 0.0f)
        contact->normal = contactNormal;
    else
        contact->normal = contactNormal * (-1);

    ClippedPoints cp;
    cp = FindPolygonPoints(p2, p1, contact->normal);
    contact->pointsCount = cp.Length();
    for(int  i = 0; i < cp.Length(); i++) {
        contact->points[i] = cp.points[i];
    }
}

//@Func: Proj the polygon and circle's points on the axis, and find the overlap length
bool CheckOverlapOnAxis(Vector2 axis, Polygon *p, Circle *c, real &overlapLen) {
    real min1 = p->GetPoint(0) * axis;
    real max1 = min1;
    for(int i = 1; i < p->GetCount(); i++) {
        real len = p->GetPoint(i) * axis;
        if(len < min1)
            min1 = len;
        if(len > max1)
            max1 = len;
    }

    real min2 = c->GetPos() * axis - c->GetRadius();
    real max2 = c->GetPos() * axis + c->GetRadius();

    return IsOverlap(min1, max1, min2, max2, overlapLen);
}

//@Func: Proj the polygon's points on the axis, and find the overlap length
bool CheckOverlapOnAxis(Vector2 axis, Polygon *p1, Polygon *p2, real &overlapLen) {
    real min1 = p1->GetPoint(0) * axis;
    real max1 = min1;
    for(int i = 1; i < p1->GetCount(); i++) {
        real len = p1->GetPoint(i) * axis;
        if(len > max1)
            max1 = len;
        if(len < min1)
            min1 = len;
    }

    real min2 = p2->GetPoint(0) * axis;
    real max2 = min2;
    for(int i = 1; i < p2->GetCount(); i++) {
        real len = p2->GetPoint(i) * axis;
        if(len > max2)
            max2 = len;
        if(len < min2)
            min2 = len;
    }

    return IsOverlap(min1, max1, min2, max2, overlapLen);
}

//@Func: Cal the overlap length 
bool IsOverlap(real min1, real max1, real min2, real max2, real &overlapLen) {
    overlapLen = 0;

    if(min1 > max2)
        return false;
    if(max1 < min2)
        return false;

    real biggerMin = max(min1, min2);
    real smallerMax = min(max1, max2);
    overlapLen = smallerMax - biggerMin;
    return true;
}

//@Func: Find the biggest proj len of index on aixs
int FindFurthest(Polygon *poly, Vector2 axis) {
    int furthestIndex = 0;
    real max = poly->GetPoint(0) * axis;

    for(int i = 1; i < poly->GetCount(); i++) {
        real len = poly->GetPoint(i) * axis;
        if(len > max) {
            max = len;
            furthestIndex = i;
        }
    }   

    return furthestIndex;
}

//@Func: Find the nearest edge to the contact
Edge FindNearestEdge(Polygon *p, Vector2 axis) {
    int furthestIndex = FindFurthest(p, axis);

    int nextIndex = furthestIndex+1;
    if(nextIndex > p->GetCount()-1) nextIndex = 0;

    int prevIndex = furthestIndex-1;
    if(prevIndex < 0) prevIndex = p->GetCount()-1;

    Vector2 left = p->GetPoint(furthestIndex) - p->GetPoint(nextIndex);
    Vector2 right = p->GetPoint(furthestIndex) - p->GetPoint(prevIndex);

    if((left * axis) < (right * axis)) {
        Vector2 e1 = p->GetPoint(furthestIndex);
        Vector2 e2 = p->GetPoint(nextIndex);
        return Edge(e1, e1, e2);
    } else {
        Vector2 e1 = p->GetPoint(furthestIndex);
        Vector2 e2 = p->GetPoint(prevIndex);
        return Edge(e1, e2, e1);
    }
}

//@Func: Clip the edge to find the specific contact points
ClippedPoints Clip(Vector2 v1, Vector2 v2, Vector2 n, real o) {
    ClippedPoints cp = ClippedPoints();

    //the proj on n is bigger than offset
    real d1 = n * v1 - o;
    real d2 = n * v2 - o;

    if(d1 >= 0.0f) cp.Add(v1);
    if(d2 >= 0.0f) cp.Add(v2);

    if(d1 * d2 < 0.0f) {
        Vector2 e = v2 - v1;
        //Find the point at offset
        real u = d1 / (d1 - d2);
        e *= u;
        e += v1;
        cp.Add(e);
    }

    return cp;
}

//TODO
ClippedPoints FindPolygonPoints(Polygon *p1, Polygon *p2, Vector2 n) {
    Edge e1 = FindNearestEdge(p1, -n);
    Edge e2 = FindNearestEdge(p2, n);

    Edge ref, inc;
    bool flip = false;
    if(abs(e1.GetEdge() * n) <= abs(e2.GetEdge() * n)) {
        ref = e1;
        inc = e2;
    } else {
        ref = e2;
        inc = e1;
        flip = true;
    }

    Vector2 refv = ref.GetEdge();
    refv.Normalize();

    real o1 = refv * (ref.v1);
    //clip 1
    ClippedPoints cp0 = Clip(inc.v1, inc.v2, refv, o1);
    if(cp0.Length() < 2) return ClippedPoints();

    real o2 = refv * (ref.v2);
    ClippedPoints cp = Clip(cp0[0], cp0[1], -refv, -o2);
    if(cp.Length() < 2) return ClippedPoints();

    Vector2 refNorm = n;
    if(flip)
        refNorm *= -1;

    real max = refNorm * (ref.max);

    real depthArr[2];
    depthArr[0] = refNorm * cp[0] - max;
    depthArr[1] = refNorm * cp[1] - max;
    //clip3
    if(depthArr[0] < 0.0) {
        cp.Remove(cp[0]);
    }
    if(depthArr[1] < 0.0) {
        cp.Remove(cp[1]);
    }

    return cp;
}