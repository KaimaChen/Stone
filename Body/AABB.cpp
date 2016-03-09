#include "Stone.h"

AABB::AABB(Vector2 pos, real hw, real hh) {
	Initialize();
	InitAABB(pos, hw, hh);
}

void AABB::InitAABB(Vector2 pos, real hw, real hh) {
	Vector2 vertices[4];
	vertices[0] = Vector2(-hw, hh);
	vertices[1] = Vector2(-hw, -hh);
	vertices[2] = Vector2(hw, -hh);
	vertices[3] = Vector2(hw, hh);

	mVerticesCount = 4;
	for(int i = 0; i < 4; i++) {
		mVertices[i] = vertices[i];
	}

	mPos = pos;

	if(!mIsStatic)
		CalculateMass(1.0f);
	SetOrient(0.0f);

	mMin = Vector2(mPos.x-hw, mPos.y-hh);
	mMax = Vector2(mPos.x+hw, mPos.y+hh);
}

void AABB::InitAABB(Vector2 min, Vector2 max) {
	real hw = (max.x - min.x) * 0.5f;
    real hh = (max.y - min.y) * 0.5f;
    Vector2 pos = (min + max) * 0.5f;
    InitAABB(pos, hw, hh);
}

void AABB::SetOrient(real o) {
	Polygon::SetOrient(0);
}

bool AABB::IsContainPoint(const Vector2 &p) const {
	return (p.x >= mMin.x) && (p.y >= mMin.y) &&
           (p.x <= mMax.x) && (p.y <= mMax.y);
}

void AABB::Add(const Vector2 &p) {
    if (p.x < mMin.x) mMin.x = p.x;
    if (p.y < mMin.y) mMin.y = p.y;
    if (p.x > mMax.x) mMax.x = p.x;
    if (p.y > mMax.y) mMax.y = p.y;

    InitAABB(mMin, mMax);
}

void AABB::Add(const AABB &box) {
    if (box.mMin.x < mMin.x) mMin.x = box.mMin.x;
    if (box.mMin.y < mMin.y) mMin.y = box.mMin.y;
    if (box.mMax.x > mMax.x) mMax.x = box.mMax.x;
    if (box.mMax.y > mMax.y) mMax.y = box.mMax.y;

    InitAABB(mMin, mMax);
}

Vector2 AABB::ClosestPointTo(const Vector2 &p) const {
    Vector2 r(p);

    if (r.x < mMin.x) {
        r.x = mMin.x;
    } else if (r.x > mMax.x) {
        r.x = mMax.x;
    }

    if (r.y < mMin.y) {
        r.y = mMin.y;
    } else if (r.y > mMax.y) {
        r.y = mMax.y;
    }

    return r;
}

void AABB::BoundCircle(const Vector2 &center, real radius) {
    mMin.x = center.x - radius;
    mMin.y = center.y - radius;
    mMax.x = center.x + radius;
    mMax.y = center.y + radius;

    InitAABB(mMin, mMax);
}