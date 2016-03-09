#include "Stone.h"

void Rigidbody::Initialize() {
    mVelocity = Vector2(0,0);
    mForce = Vector2(0,0);

    mAngularVelocity = 0;
    mTorque = 0;
    mOrient = Random( -PI, PI );
    
    mStaticFriction = 0.5f;
    mDynamicFriction = 0.3f;
    mRestitution = 0.4f;

    r = 1, g = 1, b = 0.7;
    mUseGravity = true;
    mIsStatic = false;
}

void Rigidbody::ApplyImpulse(const Vector2 &impulse, const Vector2 &contactPos) {
	mVelocity += mInverseMass * impulse;
	mAngularVelocity += mInverseInertia * Cross(contactPos, impulse);
}

void Rigidbody::SetStatic() {
    mIsStatic = true;
	mInertia = 0.0f;
	mInverseInertia = 0.0f;
	mMass = 0.0f;
	mInverseMass = 0.0f;
}

void Rigidbody::UpdateVelocity() {
    mVelocity += (mForce * mInverseMass) * (FRAME_TIME_SECOND / 2.0f);
    mAngularVelocity += mTorque * mInverseInertia * (FRAME_TIME_SECOND / 2.0f);
}

void Rigidbody::UpdateState() {
    if(Equal(mInverseMass, 0.0f)) return;

    UpdateVelocity();

    mPos += mVelocity * FRAME_TIME_SECOND;
    mOrient += mAngularVelocity * FRAME_TIME_SECOND;
    SetOrient(mOrient);
}

void Rigidbody::AddForce(const Vector2 &f) {
    if(Equal(mInverseMass, 0.0f)) return;

    Vector2 fm = f * mMass; //to appear more obvious

    mForce += fm;
}

void Rigidbody::AddForce2(const Vector2 &f) {
    mForce += f;
}

void Rigidbody::ClearForce() {
    mForce = Vector2(0,0);
    mTorque = 0;
}

void Rigidbody::SetColor(real r, real g, real b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

void Rigidbody::SetOrient(real o) {
    mOrient = o;
}

void Rigidbody::SetPos(Vector2 p) {
    mPos = p;
}

Vector2 Rigidbody::GetPos() const {
    return mPos;
}

void Rigidbody::SetUseGravity(bool isUse) {
    mUseGravity = isUse;
}

bool Rigidbody::GetUseGravity() const {
    return mUseGravity;
}

void Rigidbody::SetVelocity(Vector2 v) {
    mVelocity = v;
}

Vector2 Rigidbody::GetVelocity() const {
    return mVelocity;
}

void Rigidbody::SetInverseInertia(real ii) {
    mInverseInertia = ii;
}

real Rigidbody::GetinverseInertia() const {
    return mInverseInertia;
}

void Rigidbody::SetInverseMass(real im) {
    mInverseMass = im;
}

real Rigidbody::GetInverseMass() const {
    return mInverseMass;
}

real Rigidbody::GetMass() const {
    return mMass;
}

void Rigidbody::SetAngularVelocity(real av) {
    mAngularVelocity = av;
}

real Rigidbody::GetAngularVelocity() const {
    return mAngularVelocity;
}

void Rigidbody::SetStaticFriction(real sf) {
    mStaticFriction = sf;
}

real Rigidbody::GetStaticFriction() const {
    return mStaticFriction;
}

void Rigidbody::SetDynamicFriction(real df) {
    mDynamicFriction = df;
}

real Rigidbody::GetDynamicFriction() const {
    return mDynamicFriction;
}

void Rigidbody::SetRestitution(real r) {
    mRestitution = r;
}

real Rigidbody::GetRestitution() const {
    return mRestitution;
}

Vector2 Rigidbody::GetForce() const {
    return mForce;
}