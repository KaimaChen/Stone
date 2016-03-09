#include "MainScene.h"
#include "sparrow.h"
#include <vector>
#include <iostream>
using namespace std;

int timer = 0;
World* world;

vector<Circle*> gAnchorList;
vector<Circle*> gPivotList;
vector<Line*> gBungeeList;
vector<Line*> gRodList;

real len1;
real len2;

void OnCreate() {
    world = World::GetInstance();

    for(int i = 0; i < 8; i++) {
        //anchor
        Circle *anchor = new Circle(Vector2(70+i*40, 300), 5);
        anchor->SetUseGravity(false);
        world->Add(anchor);
        gAnchorList.push_back(anchor);

        //pivot
        real y = (i % 2 == 1) ? 150 : 180;
        Circle *pivot = new Circle(Vector2(70+i*40, y), 7);
        pivot->SetColor(0, 1, 1);
        world->Add(pivot);
        gPivotList.push_back(pivot);

        //Bungee line
        Line *bungeeLine = new Line(anchor->GetPos(), pivot->GetPos());
        world->Add(bungeeLine);
        gBungeeList.push_back(bungeeLine);
    }

    //Rod
    for(int i = 0; i < gPivotList.size()-1; i++) {
        Line *rodLine = new Line(gPivotList[i]->GetPos(), gPivotList[i+1]->GetPos());
        world->Add(rodLine);
        gRodList.push_back(rodLine);

        Rod *rod = new Rod(gPivotList[i], gPivotList[i+1]);
        world->AddLink(rod);
    }

    len1 = VectorMag(gAnchorList[0]->GetPos() - gPivotList[0]->GetPos());
    len2 = VectorMag(gAnchorList[1]->GetPos() - gPivotList[1]->GetPos());

    gPivotList[0]->SetVelocity(Vector2(30, 0));
    gPivotList[5]->SetVelocity(Vector2(-50, 0));
}

void Update(int dt) {
    timer += dt;
    world->Update(dt);  

    //anchor bungee
    for(int i = 0; i < gAnchorList.size(); i++) {
        real len = (i % 2 == 0) ? len1 : len2;
        AnchoredBungee *ab = new AnchoredBungee(gPivotList[i], gAnchorList[i]->GetPos(), 0.8, len);
        gPivotList[i]->AddForce(ab->CreateForce(), dt);

        gBungeeList[i]->SetEndPos(gPivotList[i]->GetPos());
    }

    //Rod
    for(int i = 0; i < gPivotList.size()-1; i++) {
        gRodList[i]->SetStartPos(gPivotList[i]->GetPos());
        gRodList[i]->SetEndPos(gPivotList[i+1]->GetPos());
    }
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
}