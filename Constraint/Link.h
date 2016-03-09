#ifndef LINK_H_INCLUDED
#define LINK_H_INCLUDED

#include "Define.h"

class Contact;
class Rigidbody;

//@class: 硬约束的基类
class Link {
public:
    Rigidbody *mBodies[2];
protected:
    real CurrentLength() const;
    virtual bool AddContact(Contact *) const {};
public:
	Rigidbody *GetBodyOne() const;
	void SetBodyOne(Rigidbody *b);
	Rigidbody *GetBodyTwo() const;
	void SetBodyTwo(Rigidbody *b);
    virtual void Update();
};

class Cable : public Link {
private:
	real mMaxLength;
    real mRestitution;
    virtual bool AddContact(Contact *contact) const;
public:
    Cable(Rigidbody *b1, Rigidbody *b2);
    void SetMaxLength(real ml);
    real GetMaxLength() const;
    void SetRestitution(real r);
    real GetRestitution() const;
};

class Rod : public Link {
private:
	real mLength;
	virtual bool AddContact(Contact *contact) const;
public:
	Rod(Rigidbody *b1, Rigidbody *b2);
	void SetLength(real len);
	void CorrectPos();
	virtual void Update();
};

#endif // LINK_H_INCLUDED