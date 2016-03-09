#include "Stone.h"

const Vector2 gravity(0, -10.0f * 10);

void PhysicsWorld::Update( void ) {
    UpdateGravity();

    //with iteration, the bad penetration will reduce
    const int ITERATION = 10;
    for(int i = 0; i < ITERATION; i++) {
        CreateContacts();
        UpdateContacts();
    }

    Test();

    UpdateState();

    DetectSleep();

    ClearForce();
}

void PhysicsWorld::Render( void ) {
    //Draw body
    for(uint i = 0; i < bodies.size( ); ++i)
    {
        Rigidbody *b = bodies[i];
        b->Draw( );
    }

    //Draw contact point
    glPointSize( 4.0f );
    glBegin( GL_POINTS );
        glColor3f( 1.0f, 0.0f, 0.0f );
        for(uint i = 0; i < contacts.size( ); ++i) {
            Contact& m = contacts[i];
            for(uint j = 0; j < m.pointsCount; ++j) {
                Vector2 c = m.points[j];
                glVertex2f( c.x, c.y );
            }
        }
    glEnd( );
    glPointSize( 1.0f );

    //Draw contact normal
    glBegin( GL_LINES );
        glColor3f( 0.0f, 1.0f, 0.0f );
        for(uint i = 0; i < contacts.size( ); ++i) {
            Contact& m = contacts[i];
            Vector2 n = m.normal;
            for(uint j = 0; j < m.pointsCount; ++j) {
                Vector2 c = m.points[j];
                glVertex2f( c.x, c.y );
                c += n * 5;
                glVertex2f( c.x, c.y );
            }
        }
    glEnd( );
}

void PhysicsWorld::Add(Rigidbody *body) {
    assert(body);
    bodies.push_back(body);
}

void PhysicsWorld::Clear() {
    for(int i = 0; i < bodies.size(); i++) {
        delete bodies[i];
    }
    bodies.clear();
}

void PhysicsWorld::CreateContacts() {
    contacts.clear( );

    for(int i = 0; i < bodies.size( ); ++i) {
        Rigidbody *A = bodies[i];
        for(int j = i + 1; j < bodies.size( ); ++j) {
            Rigidbody *B = bodies[j];
            if(A->GetInverseMass() == 0 && B->GetInverseMass() == 0) continue;

            Contact m(A, B);
            m.Solve();
            if(m.pointsCount) contacts.emplace_back( m );
        }
    }
}

void PhysicsWorld::UpdateGravity() {
    for(uint i = 0; i < bodies.size( ); ++i) {
        Rigidbody *b = bodies[i];
        if(Equal(b->GetInverseMass(), 0.0f)) continue;
        if(!b->GetUseGravity()) continue;

        b->AddForce(gravity);
    }
}

void PhysicsWorld::UpdateState() {
    for(int i = 0; i < bodies.size(); i++) {
        bodies[i]->UpdateState();
    }
}

void PhysicsWorld::UpdateContacts() {
    for(int i = 0; i < contacts.size(); i++) {
        contacts[i].Update();
    }
}

void PhysicsWorld::ClearForce() {
    for(int i = 0; i < bodies.size(); i++) {
        bodies[i]->ClearForce();
    }
}

void PhysicsWorld::DetectSleep() {
    const real gravityMag = VectorMag(gravity);
    for(int i = 0; i < bodies.size(); i++) {
        real accMag = VectorMag(bodies[i]->GetForce()) * bodies[i]->GetInverseMass();
        real velocityMag = VectorMag(bodies[i]->GetVelocity());

        //if(accMag <= gravityMag + PRECITION && velocityMag < 1.5f)
            //bodies[i]->SetColor(0,1,0);
    }
}


void PhysicsWorld::Test() {
    for(int i = 0; i < bodies.size(); i++) {
        DiagonalPath(bodies[i]);
    }
}