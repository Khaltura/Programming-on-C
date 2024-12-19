#pragma once
#include "Painter.h"
#include "Point.h"
#include "Velocity.h"
#include "Color.h"

class Ball {
public:
    Ball(const Velocity velocity, const Point center,  double radius, const Color color, bool isCollidable);

    void setVelocity(const Velocity& velocity);
    Velocity getVelocity() const;

    void draw(Painter& painter) const;

    void setCenter(const Point& center); 
    Point getCenter() const;

    double getRadius() const;
    
    double getMass() const;
    Color getColor() const;
    bool ifCollidable() const;
    
private:

    Velocity velocity_;
    Point center_;
    double radius_;
    Color color_;
    bool isCollidable_;
    
};
