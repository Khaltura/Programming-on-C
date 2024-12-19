
#pragma once
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"
#include "Color.h"

class Dust {
public:
    Dust(const Velocity velocity, const Point center,  double radius, const Color color, bool isCollidable, double lifetime_);

    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;

    void draw(Painter& painter) const;

    void setCenter(const Point& center);
    Point getCenter() const;

    double getRadius() const;

    double getMass() const;
    bool ifAlive() const;
    void dying(double time);

    
private:
    Velocity velocity_;
    Point center_;
    double radius_;
    Color color_;
    bool isCollidable_;
    double lifetime_;
    bool isAlive_ = true;
};
