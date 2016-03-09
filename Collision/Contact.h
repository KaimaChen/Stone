/*
Desc: Contact response
Note: 参考 RandyGaul.net
*/
#ifndef CONTACT_H
#define CONTACT_H

#include "Define.h"

class Rigidbody;
class Vector2;

struct Contact {
    Contact( Rigidbody *a, Rigidbody *b );

	void Update();
    void Solve();     
    void ApplyImpulse();          
    void PositionalCorrection();  

    Rigidbody *A;
    Rigidbody *B;

    real penetration; 
    Vector2 normal;   
    Vector2 points[2];
    unsigned pointsCount; 
    real e;           
    real df;          
    real sf;     
};

#endif // CONTACT_H