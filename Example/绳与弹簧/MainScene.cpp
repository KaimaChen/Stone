#include "MainScene.h"
#include "sparrow.h"
#include <vector>
#include <iostream>
using namespace std;

int timer = 0;
World* world;

const Vector2 CENTER = Vector2(200, 200);

Circle *ball;
AABB *box1;
AABB *box2;
vector<Line*> lineList;

void OnCreate() {
    world = World::GetInstance();

    Circle *anchor = new Circle(CENTER, 3);
    anchor->SetUseGravity(false);
    anchor->SetColor(0.0, 1.0, 0.0);
    world->Add(anchor);

    ball = new Circle(Vector2(200, 300), 25);
    world->Add(ball);

    box1 = new AABB(Vector2(100, 100), Vector2(130, 130));
    world->Add(box1);

    box2 = new AABB(Vector2(300, 100), Vector2(320, 120));
    world->Add(box2);

    Line *anchorLine1 = new Line(CENTER, ball->GetPos());
    lineList.push_back(anchorLine1);
    world->Add(anchorLine1);

    Line *anchorLine2 = new Line(CENTER, box1->GetPos());
    lineList.push_back(anchorLine2);
    world->Add(anchorLine2);

    Line *anchorline3 = new Line(CENTER, box2->GetPos());
    lineList.push_back(anchorline3);
    world->Add(anchorline3);

    Line *line1 = new Line(ball->GetPos(), box1->GetPos());
    line1->SetColor(1.0f, 1.0f, 0.0f);
    lineList.push_back(line1);
    world->Add(line1);

    Line *line2 = new Line(ball->GetPos(), box2->GetPos());
    line2->SetColor(1.0f, 1.0f, 0.0f);
    lineList.push_back(line2);
    world->Add(line2);

    Line *line3 = new Line(box1->GetPos(), box2->GetPos());
    lineList.push_back(line3);
    world->Add(line3);
}

void Update(int dt) {
    timer += dt;
    world->Update(dt);  

    //Update anchor line 
    lineList[0]->SetEndPos(ball->GetPos());
    lineList[1]->SetEndPos(box1->GetPos());
    lineList[2]->SetEndPos(box2->GetPos());

    //Update line between item
    lineList[3]->SetStartPos(ball->GetPos());
    lineList[3]->SetEndPos(box1->GetPos());
    lineList[4]->SetStartPos(ball->GetPos());
    lineList[4]->SetEndPos(box2->GetPos());
    lineList[5]->SetStartPos(box1->GetPos());
    lineList[5]->SetEndPos(box2->GetPos());

    //create force between item and anchor
    AnchoredSpring *asBall = new AnchoredSpring(ball, Vector2(200,200), 0.8, 80);
    ball->AddForce(asBall->CreateForce(), dt);

    AnchoredSpring *asBox1 = new AnchoredSpring(box1, CENTER, 0.8, 80);
    box1->AddForce(asBox1->CreateForce(), dt);

    AnchoredSpring *asBox2 = new AnchoredSpring(box2, CENTER, 0.8, 80);
    box2->AddForce(asBox2->CreateForce(), dt);

    //create force between itemA and itemB 
    Bungee *bBall1 = new Bungee(ball, box1, 0.8, 100);
    ball->AddForce(bBall1->CreateForce1(), dt);
    box1->AddForce(bBall1->CreateForce2(), dt);

    Bungee *bBall2 = new Bungee(ball, box2, 0.8, 100);
    ball->AddForce(bBall2->CreateForce1(), dt);
    box2->AddForce(bBall2->CreateForce2(), dt);

    Spring *sBox12 = new Spring(box1, box2, 0.8, 100);
    box1->AddForce(sBox12->CreateForce1(), dt);
    box2->AddForce(sBox12->CreateForce2(), dt);
    
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