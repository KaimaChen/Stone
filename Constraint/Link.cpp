#include "Stone.h"

real Link::CurrentLength() const {
    Vector2 relativePos = mBodies[0]->GetPos() - mBodies[1]->GetPos();
    return VectorMag(relativePos);
}

void Link::Update() {
    if(!mBodies[0] || !mBodies[1]) return;

    Contact *contact = new Contact(mBodies[0], mBodies[1]);
    if(AddContact(contact)) {
        contact->Update();
    }
    delete contact;
}

Rigidbody* Link::GetBodyOne() const {
    return mBodies[0];
}

void Link::SetBodyOne(Rigidbody *b) {
    mBodies[0] = b;
}

Rigidbody* Link::GetBodyTwo() const {
    return mBodies[1];
}

void Link::SetBodyTwo(Rigidbody *b) {
    mBodies[1] = b;
}

Cable::Cable(Rigidbody *b1, Rigidbody *b2) {
    mRestitution = 1.0f;
    mMaxLength = 0.0f;
    mBodies[0] = b1;
    mBodies[1] = b2;
    if(b1 && b2) {
        mMaxLength = VectorMag(b1->GetPos() - b2->GetPos());
        mRestitution = min(b1->GetRestitution(), b2->GetRestitution());
    }
}

bool Cable::AddContact(Contact *contact) const {
    real length = CurrentLength();

    //小于绳长则不产生力
    if(length < mMaxLength) {
        return false;
    }

    contact->A = mBodies[0];
    contact->B = mBodies[1];

    Vector2 normal = mBodies[0]->GetPos() - mBodies[1]->GetPos();
    normal.Normalize();

    contact->normal = normal;
    contact->penetration = length - mMaxLength;
    contact->e = mRestitution;

    return true;
}

void Cable::SetRestitution(real r) {
    mRestitution = r;
}

real Cable::GetRestitution() const {
    return mRestitution;
}

void Cable::SetMaxLength(real ml) {
    mMaxLength = ml;
}

real Cable::GetMaxLength() const {
    return mMaxLength;
}

Rod::Rod(Rigidbody *body1, Rigidbody *body2) {
    mBodies[0] = body1;
    mBodies[1] = body2;
    if(body1 && body2)
        mLength = VectorMag(body1->GetPos() - body2->GetPos());
}

void Rod::SetLength(real len) {
    assert(len >= 0);
    mLength = len;
}

bool Rod::AddContact(Contact *contact) const {
    real currentLen = CurrentLength();

    if(currentLen == mLength) {
        return false;
    }

    contact->A = mBodies[0];
    contact->B = mBodies[1];

    Vector2 normal = mBodies[1]->GetPos() - mBodies[0]->GetPos();
    normal.Normalize();

    if(currentLen > mLength) {
        contact->normal = -normal;
        contact->penetration = currentLen - mLength;
    } else {
        contact->normal = normal;
        contact->penetration = mLength - currentLen;
    }

    contact->e = 1;

    return true;
}

void Rod::CorrectPos() {
    real currentLen = CurrentLength();

    if(currentLen == mLength) {
        return;
    }

    const real k_slop = 0.05f; // Penetration allowance
    const real percent = 1.0f; // Penetration percentage to correct
    Rigidbody *A = mBodies[0];
    Rigidbody *B = mBodies[1];

    Vector2 normal = B->GetPos() - A->GetPos();
    normal.Normalize();

    Vector2 correction;
    real penetration = real_abs(currentLen - mLength);

    if(currentLen < mLength) {
        correction = (std::max( penetration - k_slop, 0.0f ) / (A->GetInverseMass() + B->GetInverseMass())) * normal * percent;
    }

    if(currentLen > mLength) {
        correction = (std::max( penetration - k_slop, 0.0f ) / (A->GetInverseMass() + B->GetInverseMass())) * normal * percent;
        correction *= -1;
    }

    Vector2 ap = A->GetPos();
    ap -= correction * A->GetInverseMass();
    A->SetPos(ap);
    Vector2 bp = B->GetPos();
    bp += correction * B->GetInverseMass();
    B->SetPos(bp);
}

void Rod::Update() {
    if(!mBodies[0] || !mBodies[1]) return; 

    Contact *contact = new Contact(mBodies[0], mBodies[1]);
    const int ITERATION = 10;
    for(int i = 0; i < ITERATION; i++) {
        if(AddContact(contact)) {
            contact->Update();
        }
    }
    
    delete contact;
}
