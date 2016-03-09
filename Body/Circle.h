#ifndef CIRCLE_H
#define CIRCLE_H

class Rigidbody;

class Circle : public Rigidbody {
private:
	real mRadius;
public:
	Circle(real r, Vector2 pos);
	Circle(const Circle &c1,const Circle &c2);
public:
	void SetRadius(real r);
	real GetRadius() const;
public:
	virtual void Draw() const;
	virtual void CalculateMass(real density);
	virtual BodyType GetType() const;
	virtual bool IsContainPoint(const Vector2 &p) const;
public:
    //@Func: 扩大半径以包围点
    void Add(const Vector2 &p);
	//@Func: 求到某点的最近距离
    Vector2 ClosestPointTo(const Vector2 &p) const;
    //@Func: 判断是否与线段相交，相交点放在points中
    bool IntersectWithLine(const Vector2 &start, const Vector2 &end) const;
    //@Func: 包围AABB
    void BoundAABB(const Vector2 &min, const Vector2 &max);
    //@Func: Bound two circles
    void BoundCircles(const Circle *c1,const Circle *c2);
};

#endif // CIRCLE_H
