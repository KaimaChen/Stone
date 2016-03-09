#include "Stone.h"

Contact::Contact(Rigidbody *a, Rigidbody *b)
    : A(a), B(b) {
    pointsCount = 0;
    e = min(A->GetRestitution(), B->GetRestitution());
    sf = sqrt(A->GetStaticFriction() * B->GetStaticFriction());
    df = sqrt(A->GetDynamicFriction() * B->GetDynamicFriction());
}
    
void Contact::Solve()
{
  	BodyType AType = A->GetType();
  	BodyType BType = B->GetType();

  	if(AType == T_Circle && BType == T_Circle)
  	  	CircleVsCircle(this, (Circle*)A, (Circle*)B);

  	if(AType == T_Circle && BType == T_Polygon) 
        CircleVsPolygon(this, (Circle*)A, (Polygon*)B);

  	if(AType == T_Polygon && BType == T_Circle)
        PolygonVsCircle(this, (Polygon*)A, (Circle*)B);
    
  	if(AType == T_Polygon && BType == T_Polygon)
  	  	PolygonVsPolygon(this, (Polygon*)A, (Polygon*)B);
}

//TODO
void Contact::ApplyImpulse()
{
  	for(uint i = 0; i < pointsCount; ++i)
  	{
    	Vector2 ra = points[i] - A->GetPos();
    	Vector2 rb = points[i] - B->GetPos();
    	Vector2 rv = B->GetVelocity() + Cross(B->GetAngularVelocity(), rb) -
    	          A->GetVelocity() - Cross(A->GetAngularVelocity(), ra);

    	real contactVel = Dot(rv, normal);
    	if(contactVel > 0)
    	  return;

    	real raCrossN = Cross(ra, normal);
    	real rbCrossN = Cross(rb, normal);
    	real invMassSum = A->GetInverseMass() + B->GetInverseMass() + ( raCrossN*raCrossN ) * A->GetinverseInertia() + ( rbCrossN*rbCrossN ) * B->GetinverseInertia();

    	real j = -(1.0f + e) * contactVel;
    	j /= invMassSum;
    	j /= (real)pointsCount;

    	Vector2 impulse = normal * j;

    	A->ApplyImpulse(-impulse, ra);
    	B->ApplyImpulse(impulse, rb);

    	// Friction impulse
    	rv = B->GetVelocity() + Cross(B->GetAngularVelocity(), rb) -
    	     A->GetVelocity() - Cross(A->GetAngularVelocity(), ra);

    	Vector2 t = rv - (normal * Dot(rv, normal));
    	t.Normalize();

    	// j tangent magnitude
    	real jt = -Dot(rv, t);
    	jt /= invMassSum;
    	jt /= (real)pointsCount;

    	// Don't apply tiny friction impulses
    	if(Equal(jt, 0.0f))
    	  return;

    	// Coulumb's law
    	Vector2 tangentImpulse;
    	if(std::abs(jt) < j * sf)
    	  tangentImpulse = t * jt;
    	else
    	  tangentImpulse = t * -j * df;

    	// Apply friction impulse
    	A->ApplyImpulse(-tangentImpulse, ra);
    	B->ApplyImpulse(tangentImpulse, rb);
  	}
}

void Contact::PositionalCorrection()
{
  	const real precition = 0.01f;
  	const real percent = 0.07f; //should adjust according to the ITERATION
  	Vector2 correction = (max( penetration - precition, 0.0f ) / (A->GetInverseMass() + B->GetInverseMass())) * normal * percent;
  	
  	Vector2 ap = A->GetPos();
  	ap -= correction * A->GetInverseMass();
  	A->SetPos(ap);

  	Vector2 bp = B->GetPos();
  	bp += correction * B->GetInverseMass();
  	B->SetPos(bp);
}

void Contact::Update() {
    ApplyImpulse();
    PositionalCorrection();
}
