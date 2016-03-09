#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

class PhysicsWorld {
public:
    void Update();
    void Render();
    void Add(Rigidbody *body);
    void Clear();
public:
    vector<Rigidbody*> bodies;
    vector<Contact> contacts;
private:
    void CreateContacts(); //O(n^2)
    void UpdateGravity();
    void UpdateState();
    void UpdateContacts();
    void ClearForce();
    void DetectSleep();

    void Test();
};

#endif // PHYSICS_WORLD_H