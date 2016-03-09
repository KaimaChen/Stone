/*
Desc: rigidbody is a important class
note: 参考 RandyGaul.net, Box2d
*/
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

class Vector2;

enum BodyType {
    T_Circle,
    T_Polygon,
    T_AABB,
};

class Rigidbody {
protected:
    Vector2 mPos;
    Vector2 mVelocity;
    Vector2 mForce;
    real mOrient;
    real mAngularVelocity;
    real mTorque;
    real mMass;
    real mInverseMass;
    real mInertia;
    real mInverseInertia;
    real mRestitution;
    real mStaticFriction;
    real mDynamicFriction;
    bool mUseGravity;
    bool mIsStatic;
    real r,g,b;

// --open func-- //
public:
    void ApplyImpulse(const Vector2& impulse, const Vector2& contactPos);
    void SetStatic();
    void Initialize();
    void UpdateVelocity();
    void UpdateState();
    void AddForce(const Vector2 &f);//TODO this is add acceleration
    void AddForce2(const Vector2 &f); 
    void ClearForce();
    void SetColor(real r, real g, real b);

// --virtual func-- //
public:
    virtual void SetOrient(real o);
    virtual void Draw() const = 0;
    virtual void CalculateMass(real) = 0;
    virtual BodyType GetType() const = 0;
    virtual bool IsContainPoint(const Vector2 &p) const = 0;

// --Set && Get-- //
public:
    void SetPos(Vector2 p);
    Vector2 GetPos() const;
    void SetUseGravity(bool);
    bool GetUseGravity() const;
    void SetVelocity(Vector2 v);
    Vector2 GetVelocity() const;
    void SetInverseInertia(real ii);
    real GetinverseInertia() const;
    void SetInverseMass(real im);
    real GetInverseMass() const;
    real GetMass() const;
    void SetAngularVelocity(real av);
    real GetAngularVelocity() const;
    void SetStaticFriction(real sf);
    real GetStaticFriction() const;
    void SetDynamicFriction(real df);
    real GetDynamicFriction() const;
    void SetRestitution(real s);
    real GetRestitution() const;
    Vector2 GetForce() const;
};

#endif // RIGIDBODY_H