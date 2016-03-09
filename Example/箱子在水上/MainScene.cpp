#include "MainScene.h"
#include "sparrow.h"
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

int timer = 0;
World* world;

AABB *box;

//sine function (y = Asin(Bx + C) + h)
real A = 1.5;
real B = 1.0f / 2;
real C = 0;
real h = 200;

void OnCreate() {
    world = World::GetInstance();

    box = new AABB(Vector2(180, 180), Vector2(220, 220));
    box->SetColor(1.0, 1.0, 0);
    world->Add(box);
}

void Update(int dt) {
    timer += dt;
    world->Update(dt);  

    Buoyancy *b = new Buoyancy(box, box->YSize(), box->XSize()*box->YSize(), 200);
    box->AddForce(b->CreateForce(), dt);

    C += dt / 100.0;
}

void OnSpecialKey(int key, int x, int y) {
    vector<Rigidbody*> list = world->GetList();
    for(auto item : list) {
        if (key == GLUT_KEY_DOWN) {
            item->Move(Vector2(0, -MOVE_STEP));
        }

        if (key == GLUT_KEY_UP) {
            item->Move(Vector2(0, MOVE_STEP));
        }

        if (key == GLUT_KEY_LEFT) {
            item->Move(Vector2(-MOVE_STEP, 0));
        }

        if (key == GLUT_KEY_RIGHT) {
            item->Move(Vector2(MOVE_STEP, 0));
        }
    }
}

void OnMouse(int button, int state, int x, int y) {
}

void OnDraw() {
    world->Draw();

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POINTS);
        for(real x = 0; x < 400; x += 0.1) {
            real y = A * sin(B * x + C) + h;
            glVertex2f(x, y);
        }
    glEnd();
}