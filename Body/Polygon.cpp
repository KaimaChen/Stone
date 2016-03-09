#include "Stone.h"

Polygon::Polygon(Vector2 pos) {
	mPos = pos;
	Initialize();
	CalculateMass(1.0f);
	SetOrient(0.0f);
}

Polygon::Polygon(Vector2 *vertices, unsigned count, Vector2 pos) {
	assert(count > 2 && count <= MAX_POLYGON_VERTEX);	

	mVerticesCount = count;
	for(int i = 0; i < count; i++) {
		mVertices[i] = vertices[i];
	}

	mPos = pos;

	Initialize();
	CalculateMass(1.0f);
	SetOrient(0.0f);
}

void Polygon::Draw() const {
	glColor3f(r, g, b);
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < mVerticesCount; i++) {
		Vector2 p = mAffine * mVertices[i] + mPos;
		glVertex2f(p.x, p.y);
	}
	glEnd();

	glColor3f(1,0,0);
	glBegin(GL_LINE_STRIP);
		Vector2 edge  = GetPoint(0) - GetPoint(1);
		Vector2 normal = Vector2(-edge.y, edge.x);
		normal.Normalize();
		real d1 = VectorMag(edge) / 2.0f;
		real d2 = VectorMag(GetPoint(0) - mPos);
		real d = sqrt(d2*d2 - d1*d1);
		Vector2 v = mPos + normal * d;
		glVertex2f(mPos.x, mPos.y);
		glVertex2f(v.x, v.y);
	glEnd();

	//Draw point at poss
    glColor3f(0,0,1);
    glPointSize(5);
    glBegin(GL_POINTS);
        glVertex2f(mPos.x, mPos.y);
    glEnd();
}

void Polygon::CalculateMass(real density) {
	assert(density > 0);

	real area = 0.0f;
	Vector2 centroid(0,0);
	real inertia = 0.0f;
	const real oneThird = 1.0f / 3.0f;

	//we have to make sure the vertices is counter-clockwise
	//otherwise the area will be negative
	for(int i = 0; i < mVerticesCount; i++) {
		// area = sum of triangles' vector area
		Vector2 p1 = mVertices[i];
		int next = (i+1 > mVerticesCount-1) ? 0 : i+1;
		Vector2 p2 = mVertices[next];
		real triangleArea = 0.5f * Cross(p1, p2);
		area += triangleArea;

		// Inertia TODO
		real x = (p1.x * p1.x) + (p1.x * p2.x) + (p2.x * p2.x);
		real y = (p1.y * p1.y) + (p1.y * p2.y) + (p2.y * p2.y);
		inertia += (0.25f * oneThird * Cross(p1,p2)) * (x + y);

		// centroid TODO
		centroid += triangleArea * oneThird * (p1 + p2);
	}

	centroid *= 1.0f / area;
	for(int i = 0; i < mVerticesCount; i++)
		mVertices[i] -= centroid;

	mMass = area * density;
	mInverseMass = (mMass > 0) ? 1.0f/mMass : 0.0f;
	mInertia = inertia * density;
	mInverseInertia = (mInertia > 0) ? 1.0f/mInertia : 0.0f;
}

void Polygon::SetOrient(real orient) {
	mOrient = orient;
	mAffine.Set(orient);
}

BodyType Polygon::GetType() const {
	return T_Polygon;
}

void Polygon::SetCount(unsigned c) {
	mVerticesCount = c;
}

unsigned Polygon::GetCount() const {
	return mVerticesCount;
}

Vector2 Polygon::GetPoint(int index) const {
	assert(index >= 0 && index < mVerticesCount);
	return mAffine * mVertices[index] + mPos;
}

Vector2 Polygon::GetPointNotAffine(int index) const {
	assert(index >= 0 && index < mVerticesCount);
	return mVertices[index];
}

Mat22 Polygon::GetAffine() const {
	return mAffine;
}

//@Func: the vertex in polygon is ccw(this is the condition the func will work)
//		 if the p is in polygon, then cross(BA, pB) so all > 0, direct up
//		 if the p is not in polygon, then some cross(BA, pB) should < 0, direct down
bool Polygon::IsContainPoint(const Vector2 &p) const {
	for(int i = 0, j = GetCount()-1; i < GetCount(); j=i, i++) {
        Vector2 edge = GetPoint(i) - GetPoint(j);
        Vector2 p2j = p - GetPoint(j);

        if(Cross(edge, p2j) < 0) return false;
    }

    return true;
}
