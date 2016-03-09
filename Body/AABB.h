#ifndef AABB_H
#define AABB_H

#include "Define.h"
#include "Vector.h"
class Polygon;

class AABB : public Polygon {
private:
	Vector2 mMin, mMax;
	void InitAABB(Vector2 pos, real hw, real hh);
	void InitAABB(Vector2 min, Vector2 max);
public:
	AABB(Vector2 pos, real hw, real hh);
public:
	virtual void SetOrient(real o);
	virtual bool IsContainPoint(const Vector2 &p) const;
public:
	void Add(const Vector2 &p);
	void Add(const AABB &aabb);
	Vector2 ClosestPointTo(const Vector2 &p) const;
	void BoundCircle(const Vector2 &center, real radius);
};

#endif // AABB_H
