#include "Stone.h"

enum State {
    Create, 
    Ray,    
    Choose,  
    MakeRod,
    MakeCable,
} g_state;

PhysicsWorld *world;
Rigidbody *g_body;
Edge *g_ray;
Edge *g_forceEdge;
Rod *g_rod;
Cable *g_cable;
int g_curX, g_curY; //the mouse current pos

void CreateGround() {
    Vector2 vertices[4];
    vertices[0] = Vector2(-WIDTH/2, 20);
    vertices[1] = Vector2(-WIDTH/2, -20);
    vertices[2] = Vector2(WIDTH/2, -20);
    vertices[3] = Vector2(WIDTH/2, 20);
    Polygon *p = new Polygon(vertices, 4, Vector2(WIDTH/2, -10));
    p->SetStatic();
    p->SetRestitution(1.0);
    world->Add(p);
}

void ChoseBody(int x, int y) {
    if(!g_body) return;

    AnchoredBungee *ab = new AnchoredBungee(g_body, Vector2(x,y), 0.8f, 1);
    g_body->AddForce(ab->CreateForce() * 7);
    delete ab;

    if(!g_forceEdge)
        g_forceEdge = new Edge(Vector2(x,y), g_body->GetPos());
    else {
        g_forceEdge->v1 = Vector2(x,y);
        g_forceEdge->v2 = g_body->GetPos();
    }
}

void OnCreate() {
    world = new PhysicsWorld();
    g_state = State::Choose;

    CreateGround();

    g_ray = new Edge(Vector2(0,20), Vector2(WIDTH, HEIGHT));

    g_rod = new Rod(nullptr, nullptr);
    g_cable = new Cable(nullptr, nullptr);
}

void Update() {
    world->Update();
    
    g_rod->Update();
    g_cable->Update();

    //Ray casting
    if(g_state == State::Ray) {
        for(int i = 0; i < world->bodies.size(); i++) {
            world->bodies[i]->SetColor(1,1,0.7);

            if(world->bodies[i]->GetType() == T_Circle) {
                if(CircleVsEdge((Circle*)world->bodies[i], g_ray)) 
                    world->bodies[i]->SetColor(0,0,1);
            } else if(world->bodies[i]->GetType() == T_Polygon) {
                if(PolygonVsEdge((Polygon*)world->bodies[i], g_ray))
                    world->bodies[i]->SetColor(0,0,1);
            }
        }
    }
    
    if(g_body && g_state == State::Choose) {
        g_body->SetColor(1,0,0);
        ChoseBody(g_curX, g_curY);
    }
}

void OnSpecialKey(int key, int x, int y) {

}

void OnMouse(int button, int state, int x, int y) {
    y = HEIGHT - y;

    if(state != GLUT_DOWN) return;

    if(g_state == State::Create) {
        if(button == GLUT_LEFT_BUTTON) {
            Vector2 vertices[4];
            vertices[0] = Vector2(-20, 20);
            vertices[1] = Vector2(-20, -20);
            vertices[2] = Vector2(20, -20);
            vertices[3] = Vector2(20, 20);
            Polygon *p = new Polygon(vertices, 4, Vector2(x, y));
            //p->SetUseGravity(false);
            p->SetOrient(PI / 3);
            world->Add(p);
        } else if(button == GLUT_RIGHT_BUTTON) {
            Circle *c = new Circle(Random(20, 40), Vector2(x, y));
            //c->SetUseGravity(false);
            world->Add(c);
        }
    }
    //Pick Body
    if(g_state == State::Choose) {
        for(int i = 0; i < world->bodies.size(); i++) {
            if(world->bodies[i]->GetType() == T_Circle) {
                Circle *c = (Circle*)world->bodies[i];
                if(c->IsContainPoint(Vector2(x,y))) {
                    c->SetColor(1,0,0);
                    g_body = c;
                }
            }
        }
    }
    //Make Rod
    if(g_state == State::MakeRod) {
        for(int i = 0; i < world->bodies.size(); i++) {
            if(world->bodies[i]->GetType() == T_Circle) {
                Circle *c = (Circle*)world->bodies[i];
                if(c->IsContainPoint(Vector2(x,y))) {
                    if(!g_rod->GetBodyOne())
                        g_rod->SetBodyOne(c);
                    else if(!g_rod->GetBodyTwo()) {
                        g_rod->SetBodyTwo(c);
                        real dis = VectorMag(g_rod->GetBodyOne()->GetPos() - c->GetPos());
                        g_rod->SetLength(dis);
                    }
                }
            }
        }
    }
    //Make Cable
    if(g_state == State::MakeCable) {
        for(int i = 0; i < world->bodies.size(); i++) {
            if(world->bodies[i]->GetType() == T_Circle) {
                Circle *c = (Circle*)world->bodies[i];
                if(c->IsContainPoint(Vector2(x,y))) {
                    if(!g_cable->GetBodyOne())
                        g_cable->SetBodyOne(c);
                    else if(!g_cable->GetBodyTwo()) {
                        g_cable->SetBodyTwo(c);
                        real dis = VectorMag(g_cable->GetBodyOne()->GetPos() - c->GetPos());
                        g_cable->SetMaxLength(dis);
                        g_cable->SetRestitution(0.8f);
                    }
                }
            }
        }
    }
}

void OnPassiveMouse(int x, int y) {
    y = HEIGHT - y;
    g_curX = x;
    g_curY = y;

    if(g_state == State::Ray) 
        g_ray->v1 = Vector2(x, y);

    if(g_state == State::Choose) {
        ChoseBody(x, y);
    }
}

void OnMenu(int value) {
    switch(value) {
        case CLEAR:
            world->Clear();
            delete g_cable;
            g_cable = new Cable(nullptr, nullptr);
            delete g_rod;
            g_rod = new Rod(nullptr, nullptr);
            CreateGround();
            g_state = State::Create;
            break;

        case CREATE:
            g_state = State::Create;
            break;

        case RAY_CAST:
            g_state = State::Ray;
            break;

        case PICK:
            g_state = State::Choose;
            break;

        case ROD:
            g_state = State::MakeRod;
            break;

        case CABLE:
            g_state = State::MakeCable;
            break;
    }
}

void OnDraw() {
    world->Render();

    if(g_state == State::Ray) {
        glColor3f(0.7f, 0.7f, 0.7f);
        glBegin(GL_LINE_STRIP);
            Vector2 v1 = g_ray->v1;
            Vector2 v2 = g_ray->v2;
            glVertex2f(v1.x, v1.y);
            glVertex2f(v2.x, v2.y);
        glEnd();
    }

    if(g_forceEdge && g_state == State::Choose) 
        DrawEdge(g_forceEdge);

    //draw rod
    Rigidbody *b1 = g_rod->GetBodyOne();
    Rigidbody *b2 = g_rod->GetBodyTwo();
    if(b1 && b2) {
        glColor3f(0, 0.3f, 0.7f);
        glBegin(GL_LINE_STRIP);
            Vector2 v1 = b1->GetPos();
            Vector2 v2 = b2->GetPos();
            glVertex2f(v1.x, v1.y);
            glVertex2f(v2.x, v2.y);
        glEnd();
    }

    //draw cable
    Rigidbody *cb1 = g_cable->GetBodyOne();
    Rigidbody *cb2 = g_cable->GetBodyTwo();
    if(cb1 && cb2) {
        glColor3f(0, 0.3f, 0.7f);
        glBegin(GL_LINE_STRIP);
            Vector2 v1 = cb1->GetPos();
            Vector2 v2 = cb2->GetPos();
            glVertex2f(v1.x, v1.y);
            glVertex2f(v2.x, v2.y);
        glEnd();
    }
}