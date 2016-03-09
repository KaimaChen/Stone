#include "MainScene.h"
#include "sparrow.h"
#include "Joint.h"
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

int timer = 0;
World* world;

vector<Circle*> ballList;
vector<Line*> lineList;
Vector2 force;

void OnCreate() {
    glPointSize(30);
    world = World::GetInstance();

    world->SetDragParam(0.01, 0.015);

    int count = 100;
    real len = 5;
    real r = 0.5;
    for(int i = 0; i < count; i++) {
        real x = WIDTH / 2;
        real y = HEIGHT / 2 - i * len;
        Circle *ball = new Circle(Vector2(x, y), r);
        ball->SetColor(1, 1, 0);
        ball->SetUseGravity(false);
        ballList.push_back(ball);
        world->Add(ball);
    }
    
    for(int i = 0; i < ballList.size() - 1; i++) {
        Circle *ball1 = ballList[i];
        Circle *ball2 = ballList[i+1];
        Line *line = new Line(ball1->GetPos(), ball2->GetPos());
        line->SetColor(1, 1, 0);
        lineList.push_back(line);
        world->Add(line);

        Rod *rod = new Rod(ball1, ball2);
        world->AddLink(rod);
    }

    force = Vector2(10, 0);
}

int count = 0;

void Update(int dt) {
    timer += dt;
    world->Update(dt); 

    for(int i = 0; i < ballList.size() - 1; i++) {
        Line *line = lineList[i];
        Circle *ball1 = ballList[i];
        Circle *ball2 = ballList[i+1];
        line->SetStartPos(ball1->GetPos());
        line->SetEndPos(ball2->GetPos());
    }

    ballList[0]->AddForce(force, dt);
}

void OnSpecialKey(int key, int x, int y) {
    if (key == GLUT_KEY_DOWN) {
        force = Vector2(0, -50);
    }

    if (key == GLUT_KEY_UP) {
        force = Vector2(0, 50);
    }

    if (key == GLUT_KEY_LEFT) {
        force = Vector2(-50, 0);
    }

    if (key == GLUT_KEY_RIGHT) {
        force = Vector2(50, 0);
    }
}

void OnMouse(int button, int state, int x, int y) {

}

void OnDraw() {
    world->Draw();
}