#ifndef POLYGON_H
#define POLYGON_H

#include "Matrix.h"
class Rigidbody;

class Polygon : public Rigidbody {
protected:
	Vector2 mVertices[MAX_POLYGON_VERTEX];
	unsigned mVerticesCount;
	Mat22 mAffine;
public:
	Polygon() {}
	Polygon(Vector2 pos);
	Polygon(Vector2 *, unsigned, Vector2 pos);
public:
	virtual void Draw() const;
	virtual void CalculateMass(real density);
	virtual void SetOrient(real o);
	virtual BodyType GetType() const;
	virtual bool IsContainPoint(const Vector2 &p) const;
public:
	unsigned GetCount() const;
	void SetCount(unsigned c);
	Vector2 GetPoint(int index) const;
	Vector2 GetPointNotAffine(int index) const;
	Mat22 GetAffine() const;
};

#endif // POLYGON_H
