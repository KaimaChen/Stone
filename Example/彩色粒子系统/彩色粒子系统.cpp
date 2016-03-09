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
}

void Update(int dt) {
    timer += dt;
    world->Update(dt);  

    if(timer < 200) return;
    timer = 0;

    //create the particles
    for(int i = 0; i < 5; i++) {
        Circle *ball = new Circle(Vector2(200, 200), Random(2,10));
        real x = Random(-100, 100);
        ball->SetColor(Random(0,1), Random(0,1), Random(0,1));
        ball->SetVelocity(Vector2(x, 130));
        gDrawList.push_back(ball);
        world->Add(ball);
    }

    //clear the particles
    for(auto it = gDrawList.begin(); it < gDrawList.begin() + gDrawList.size(); it++) {
        auto item = *it;
        if(item->GetPos().y < 0) {
            gDrawList.erase(it);
            world->Remove(item);
            return;
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