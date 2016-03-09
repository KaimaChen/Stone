#ifndef FORCEGENERATOR_H_INCLUDED
#define FORCEGENERATOR_H_INCLUDED

#include "Constant.h"
#include "Define.h"

class Vector2;
class Rigidbody;

    //作用力生成器基类(Anchord)
    class AnchoredForceGenerator {
    public:
        virtual Vector2 CreateForce() = 0;
    };

    //作用力生成器基类(not Anchord)
    class ForceGenerator {
    public:
        virtual Vector2 CreateForce1() = 0;
        virtual Vector2 CreateForce2() = 0;
    };

    //重力发生器
    class GravityGenerator : public AnchoredForceGenerator {
    private:
        Vector2 mGravity;
        real mMass;
    public:
        GravityGenerator(real mass, const Vector2 &gravity = Vector2(0, G)) 
        : mGravity(gravity), mMass(mass) {}
        virtual Vector2 CreateForce();
    };

    //摩擦力发生器
    class DragGenerator : public AnchoredForceGenerator {
    private:
        real k1;
        real k2;
        Rigidbody *mBody;
    public:
        DragGenerator(Rigidbody *body, real a=0, real b=0) : k1(a), k2(b), mBody(body) {}
        virtual Vector2 CreateForce();
    };

    //浮力发生器 (要和阻力一起才正常一点, the effect is not good) TODO
    //Example: Buoyancy(box, box->YSize(), box->XSize()*box->YSize(), 200);
    class Buoyancy : public AnchoredForceGenerator {
    private:
        real mMaxDepth; //the height of body
        real mVolume; 
        real mWaterHeight; 
        real mLiquidDensity;
        Rigidbody *mBody;
    public:
        Buoyancy(Rigidbody *body, real maxDepth, real v, real h, real density = 0.1f)
            : mBody(body), mMaxDepth(maxDepth), mVolume(v), mWaterHeight(h), mLiquidDensity(density) {}
        virtual Vector2 CreateForce();
    };

    //弹力发生器（一端固定，类似弹簧）
    class AnchoredSpring : public AnchoredForceGenerator {
    public:
        Rigidbody *mBody;
        Vector2 mAnchor;
        real mSpringConstant; //弹性系数
        real mRestLength; //静止长度
    public:
        AnchoredSpring(Rigidbody *body, Vector2 anchor, real constant, real len)
            : mBody(body), mAnchor(anchor), mSpringConstant(constant), mRestLength(len) {}
        virtual Vector2 CreateForce();
    };

    //弹力发生器（一端固定，类似弹力绳）
    class AnchoredBungee : public AnchoredSpring {
    public:
        AnchoredBungee(Rigidbody *body, Vector2 a, real c, real l)
            : AnchoredSpring(body, a, c, l) {}
        virtual Vector2 CreateForce();
    };

    //弹力发生器（弹簧）
    class Spring : public ForceGenerator {
    protected:
        Rigidbody *mFirst; //绑定的第一个物体
        Rigidbody *mSecond; //绑定的第二个物体
        real mSpringConstant; //弹性系数
        real mRestLength; //静止长度
    public:
        Spring(Rigidbody* first, Rigidbody *second, real constant, real len)
            : mFirst(first), mSecond(second), mSpringConstant(constant), mRestLength(len) {}
        virtual Vector2 CreateForce1();
        virtual Vector2 CreateForce2();
    };

    //弹力发生器（弹力绳）
    class Bungee : public Spring {
    public:
        Bungee(Rigidbody *first, Rigidbody *second, real constant, real len)
            : Spring(first, second, constant, len) {}
        virtual Vector2 CreateForce1();
        virtual Vector2 CreateForce2();
    };

#endif // FORCEGENERATOR_H_INCLUDED
