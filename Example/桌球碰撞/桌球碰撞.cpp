#include "MainScene.h"
#include "sparrow.h"
#include <vector>
#include <cstdlib>
#include <iostream>
using namespace std;

int timer = 0;
vector<Rigidbody*> gDrawList;
World* world;

void OnCreate() {
    world = World::GetInstance();

    real r = 10.0f;

    //create the triangle shape balls
    for(int i = 0; i < 5; i++) {
        for(int k = 0; k <= i; k++) {
            real x = 200 - i*r + k*r*2;
            real y = 200 + (r*2) * i;
            Vector2 pos = Vector2(x, y);
            Circle *cir = new Circle(pos, r);
            cir->SetUseGravity(false);
            cir->SetColor(Random(0,1), Random(0,1), Random(0,1));
            gDrawList.push_back(cir);
            world->Add(cir);
        }
    }

    Circle *ball = new Circle(Vector2(200, 100), r);
    ball->SetUseGravity(false);
    ball->SetVelocity(Vector2(0, 150));
    gDrawList.push_back(ball);
    world->Add(ball);
}

void Update(int dt) {
    timer += dt;
    world->Update(dt);  

    for(int i = 0; i < gDrawList.size(); i++) {
        Circle *b1 = (Circle*)gDrawList[i];
        for(int k = i+1; k < gDrawList.size(); k++) {
            Circle *b2 = (Circle*)gDrawList[k];
            if(b1->IsIntersectWithCircle(b2)) {
                Contacts *contact = new Contacts(b1, b2);
                contact->Resolve(dt);
            }
        }
    }

}

void OnSpecialKey(int key, int x, int y) {
    for(auto item : gDrawList) {
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
    for(auto item : gDrawList) {
        item->Draw();
    }
}