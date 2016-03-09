#include "Stone.h"

Circle::Circle(real r, Vector2 pos) {
	mRadius = r;
    mPos = pos;
    Initialize();
    CalculateMass(1.0f);
}

Circle::Circle(const Circle &c1,const Circle &c2) {
    mRadius = 0.0f;
    mPos = Vector2(0,0);
    Initialize();
    CalculateMass(1.0f);
    BoundCircles(&c1, &c2);
}

void Circle::SetRadius(real r) {
    assert(r > 0.0f);
    
	mRadius = r;
    CalculateMass(1.0f);
}

real Circle::GetRadius() const {
	return mRadius;
}

Vector2 Circle::ClosestPointTo(const Vector2 &p1) const {
    if (abs(p1.x - GetPos().x) < PRECITION) {
        Vector2 up = Vector2(GetPos().x, GetPos().y+mRadius);
        float dis2Up = Distance(up, p1);
        Vector2 down = Vector2(GetPos().x, GetPos().y-mRadius);
        float dis2Down = Distance(down, p1);
        return dis2Up > dis2Down ? down : up;
    }

    Vector2 p2 = GetPos();
    //直线  y = kx + m
    float k = (p1.y - p2.y) / (p1.x - p2.x);
    float m = p1.y - k * p1.x;
    //圆 (x - a)^2 + (y - b)^2 = r^2
    float a = p2.x;
    float b = p2.y;
    float r = mRadius;
    //用求根公式求x = ( -f +- sqrt(f^2 - 4eg) ) / 2e
    float e = 1 + k * k;
    float f = 2 * ( k * m - k * b - a );
    float g = a * a + (m - b) * (m - b) - r * r;

    float rx1 = (-f + sqrt(f * f - 4 * e * g) ) / (2 * e);
    float rx2 = (-f - sqrt(f * f - 4 * e * g) ) / (2 * e);
    float ry1 = k * rx1 + m;
    float ry2 = k * rx2 + m;

    return Vector2(rx1, ry1);
}

bool Circle::IntersectWithLine(const Vector2 &start, const Vector2 &end) const {
    vector<Vector2> points;

    // **平行于Y轴的线段（处理浮点数记住用上精度，否则ooxx）
    if(abs(start.x - end.x) < PRECITION) {
        real xModel = start.x;
        //判断是否与直线相交
        if ( (mPos.x - mRadius) <= (xModel + PRECITION) && (mPos.x + mRadius) >= (xModel - PRECITION) ) {
            real y1 = mPos.y - sqrt( mRadius * mRadius - (xModel - mPos.x) * (xModel - mPos.x) );
            real y2 = mPos.y + sqrt( mRadius * mRadius - (xModel - mPos.x) * (xModel - mPos.x) );
            //判断是否与线段相交
            bool hasDown = IsBetween(y1, start.y, end.y);
            bool hasUp = IsBetween(y2, start.y, end.y);

            if( hasDown ) points.push_back(Vector2(xModel, y1));
            if( hasUp ) points.push_back(Vector2(xModel, y2));
            if( points.size() == 2 && points[0] == points[1] ) points.erase(points.end()-1);

            if( hasDown || hasUp ) return true;
        }

        return false;
    }

    // **平行于X轴的线段
    if(abs(start.y - end.y) < PRECITION) {
    real yModel = start.y;
    //判断是否与直线相交
    if ( (mPos.y - mRadius) <= (yModel + PRECITION) && (mPos.y + mRadius) >= (yModel - PRECITION) ) {
        real x1 = mPos.x - sqrt( mRadius * mRadius - (yModel - mPos.y) * (yModel - mPos.y) );
        real x2 = mPos.x + sqrt( mRadius * mRadius - (yModel - mPos.y) * (yModel - mPos.y) );
        //判断是否与线段相交
        bool hasLeft = IsBetween(x1, start.x, end.x);
        bool hasRight = IsBetween(x2, start.x, end.x);

        if( hasLeft ) points.push_back(Vector2(x1, yModel));
        if( hasRight ) points.push_back(Vector2(x2, yModel));
        if( points.size() == 2 && points[0] == points[1] ) points.erase(points.end()-1);

        if( hasLeft || hasRight ) return true;
    }

        return false;
    }

    // **一般线段
    //直线：y = kx + b
    real k = (end.y - start.y) / (end.x - start.x);
    real b = start.y - k * start.x;

    //圆：(x - m)^2 + (y - n)^2 = r^2
    real m = mPos.x;
    real n = mPos.y;
    real r = mRadius;

    //二次方程：wx^2 + ex + t = 0;
    real w = 1 + k*k;
    real e = 2 * (b*k - n*k - m);
    real t = m*m + (b-n) * (b-n) - r*r;

    //根据求根公式求交点
    real inner = e*e - 4*w*t;

    if (inner < 0) return false;

    real x1 = (-e + sqrt(inner)) / (2*w);
    real y1 = k * x1 + b;
    real x2 = (-e - sqrt(inner)) / (2*w);
    real y2 = k * x2 + b;
    if( IsBetween(x1, start.x, end.x) )
        points.push_back(Vector2(x1, y1));
    if( IsBetween(x2, start.x, end.x) )
        points.push_back(Vector2(x2, y2));

    if( points.size() == 2 && points[0] == points[1] )
        points.erase(points.end()-1);

    return points.size() != 0;
}

void Circle::Draw() const {
    // draw circle
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    for(int i = 0; i < LINE_NUMS_IN_CIRCLE; i++)
        glVertex2f(
            mRadius * cos(2*PI/LINE_NUMS_IN_CIRCLE*i) + mPos.x,
            mRadius * sin(2*PI/LINE_NUMS_IN_CIRCLE*i) + mPos.y
        );
    glEnd();

    // draw line to show orient
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
        Vector2 r(0, 1.0f);
        real c = cos(mOrient);
        real s = sin(mOrient);
        r.Set(r.x * c - r.y * s, r.x * s + r.y * c);
        r *= mRadius;
        r = r + mPos;
        glVertex2f(mPos.x, mPos.y);
        glVertex2f(r.x, r.y);
    glEnd();

    //Draw point at pos
    glColor3f(0,0,1);
    glPointSize(5);
    glBegin(GL_POINTS);
        glVertex2f(mPos.x, mPos.y);
    glEnd();
}

void Circle::CalculateMass(real density) {
    real area = PI * mRadius * mRadius;
    mMass = area * density;
    mInverseMass = (mMass > 0.0f) ? 1.0f/mMass : 0.0f;
    //I = m * r^2
    mInertia = mMass * mRadius * mRadius;
    mInverseInertia = (mInertia > 0.0f) ? 1.0f/mInertia : 0.0f;
}

BodyType Circle::GetType() const {
    return T_Circle;
}

bool Circle::IsContainPoint(const Vector2 &p) const {
    float dis = Distance(mPos, p);
    return dis <= mRadius;
}

void Circle::Add(const Vector2& p) {
    float dis = Distance(GetPos(), p);
    if (dis > mRadius)
        mRadius = dis;
}

void Circle::BoundAABB(const Vector2 &min, const Vector2 &max) {
    float centerX = (min.x + max.x) / 2;
    float centerY = (min.y + max.y) / 2;
    mPos = Vector2(centerX, centerY);
    float halfXSize = (max.x - min.x) / 2;
    float halfYSize = (max.y - min.y) / 2;
    mRadius = sqrt(halfXSize*halfXSize + halfYSize*halfYSize);
}

void Circle::BoundCircles(const Circle *c1,const Circle *c2) {
    Vector2 centerOffset = c2->GetPos() - c1->GetPos();
    real distance = DistanceSquared(c1->GetPos(), c2->GetPos());
    real radiusDiff = c2->GetRadius() - c1->GetRadius();

    //如果大的圆包含小的圆
    if(radiusDiff * radiusDiff >= distance) {
        if(c1->GetRadius() > c2->GetRadius()) {
            mPos = c1->GetPos();
            mRadius = c1->GetRadius();
        } else {
            mPos = c2->GetPos();
            mRadius = c2->GetRadius();
        }
    } else {
        distance = real_sqrt(distance);
        mRadius = (distance + c1->GetRadius() + c2->GetRadius()) * (0.5); //直径 = 距离 + r1 + r2
        mPos = c1->GetPos();
        if(distance > 0) {
            mPos += centerOffset * ((mRadius - c1->GetRadius()) / distance); //偏移量 = 半径 - r1
        }
    }

    CalculateMass(1.0f);
}
