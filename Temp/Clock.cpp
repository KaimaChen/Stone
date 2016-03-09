#include "Stone.h"

PhysicsWorld *world;

vector<Circle*> g_circles;
vector<Circle*> g_anchors;

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

void OnCreate() {
	world = new PhysicsWorld();

    CreateGround();

    for(int i = 0; i < 5; i++) {
        Circle *c = new Circle(35, Vector2(150+i*70, 200));
        g_circles.push_back(c);
        world->Add(c);
    }
    g_circles[0]->SetPos(Vector2(75, 400));
    g_circles[0]->SetVelocity(Vector2(100,0));

    for(int i = 0; i < 5; i++) {
        Circle *c = new Circle(1, Vector2(150+i*70, 400));
        g_anchors.push_back(c);
        c->SetStatic();
        world->Add(c);
    }

}

void Update() {
    world->Update();

    for(int i = 0; i < g_circles.size(); i++) {
        Rod rod = Rod(g_circles[i], g_anchors[i]);
        rod.SetLength(200);
        rod.Update();
    }
}

void OnSpecialKey(int key, int x, int y) {

}

void OnMouse(int button, int state, int x, int y) {
	y = HEIGHT - y;

    if(state == GLUT_DOWN) {
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
}

void OnPassiveMouse(int x, int y) {
}

void OnDraw() {
	world->Render();

}