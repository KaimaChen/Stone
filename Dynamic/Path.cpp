#include "Stone.h"

//TODO not correct
void Orbit(Rigidbody *a, Rigidbody *b) {
	real r = VectorMag(a->GetPos() - b->GetPos());
	real d = r * (a->GetMass() + b->GetMass());

	real valA = -sqrt( (-G * b->GetMass() * b->GetMass()) / d ) * sqrt(2.0f);
	real valB = sqrt( (-G * a->GetMass() * a->GetMass()) / d ) * sqrt(2.0f);

	real angleA = GetRadiansBetween(a->GetPos(), Vector2(1,0));
	real angleB = GetRadiansBetween(b->GetPos(), Vector2(1,0));

	Vector2 av = Vector2(sin(angleA) * valA, cos(angleA) * valA);
	Vector2 bv = Vector2(sin(angleB) * valB, cos(angleB) * valB);

	//Vector2 av = Vector2(0, -sqrt( (-G * b->GetMass() * b->GetMass()) / d ) * sqrt(2.0f));
	//Vector2 bv = Vector2(0, sqrt( (-G * a->GetMass() * a->GetMass()) / d ) * sqrt(2.0f));

	a->SetVelocity(av);
	b->SetVelocity(bv);
}

void DiagonalPath(Rigidbody *body) {
	Vector2 externalForce = body->GetForce();
	Vector2 v = body->GetVelocity();
	real sum = v.x + v.y;
	if(Equal(sum, 0.0f)) sum = 0.001;
	real lamda = (-externalForce.x + externalForce.y) / sum;	

	real constraintX = lamda * v.y;
	real constraintY = -lamda * v.x;
	Vector2 constraintForce = Vector2(constraintX, constraintY);

	body->AddForce2(constraintForce);
}