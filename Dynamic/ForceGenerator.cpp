#include "Stone.h"


//=======================重力发生器===========================
Vector2 GravityGenerator::CreateForce() {
    //return mGravity * mMass;
    return mGravity;
}

//=======================阻力发生器============================
Vector2 DragGenerator::CreateForce() {
    Vector2 force = mBody->GetVelocity();

    real dragCoeff = VectorMag(force);
    dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

    force.Normalize();
    force *= -dragCoeff;
    
    return force;
}

//============================浮力发生器==================================
Vector2 Buoyancy::CreateForce() {
    real depth = mBody->GetPos().y;

    //一点都没碰到水
    if(depth >= mMaxDepth + mWaterHeight) return Vector2(0,0);

    Vector2 force(0,0);

    //完全浸入水中
    if(depth <= mWaterHeight - mMaxDepth) {
        force.y = mLiquidDensity * mVolume;
        
        return force;
    }

    //浸入部分
    force.y = mLiquidDensity * mVolume * (mWaterHeight - depth) / mMaxDepth;
    
    return force;
}

//=======================弹力发生器（一端固定，类似弹簧）==================
Vector2 AnchoredSpring::CreateForce() {
    Vector2 force = mBody->GetPos() - mAnchor;

    real magnitude = VectorMag(force);
    magnitude = (magnitude - mRestLength);
    magnitude *= mSpringConstant;

    force.Normalize();
    force *= -magnitude;
    
    return force * 2;
}

//==========================弹力发生器（一端固定，类似弹力绳）====================
Vector2 AnchoredBungee::CreateForce() {
    Vector2 force = mBody->GetPos() - mAnchor;

    //如果物体到端点的距离小于静止距离，则不产生弹力
    real magnitude = VectorMag(force);
    if(magnitude <= mRestLength) return Vector2(0,0);

    magnitude -= mRestLength;
    magnitude *= mSpringConstant;

    force.Normalize();
    force *= -magnitude;
    
    return force * 1.8;
}

//===========================弹力发生器（弹簧）=============================
Vector2 Spring::CreateForce1() {
    Vector2 force = mFirst->GetPos() - mSecond->GetPos();

    real magitude = VectorMag(force);
    magitude = (magitude - mRestLength);
    magitude *= mSpringConstant;

    force.Normalize();
    force *= -magitude;
    
    return force;
}

Vector2 Spring::CreateForce2() {
    return this->CreateForce1() * (-1);
}

//===========================弹力发生器（弹力绳）==============================
Vector2 Bungee::CreateForce1() {
    Vector2 force = mFirst->GetPos() - mSecond->GetPos();

    real magitude = VectorMag(force);

    if(magitude <= mRestLength) return Vector2(0,0);

    magitude = mSpringConstant * (magitude - mRestLength);

    force.Normalize();
    force *= -magitude;
    
    return force;
}

Vector2 Bungee::CreateForce2() {
    return this->CreateForce1() * (-1);
}