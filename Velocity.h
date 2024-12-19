#pragma once
#include "Point.h"
#include <cmath>

class Velocity {
  public:
    inline Velocity() = default;

    // TODO: комментарии
    inline Velocity(double abs, double angle) {
        const double x = std::cos(angle);
        const double y = std::sin(angle);
        vec = Point{x, y} * abs;
    }

    inline Velocity(const Point& vector) {
        setVector(vector);
    }

    inline void setVector(const Point& vector) {
        vec = vector;
    }

    inline Point vector() const {
        return vec;
    }

    // void rotate(Point& vector, double angle)
    // {
    //     Point rotatedVector;
    //     rotatedVector.x = vector.x * std::cos(angle) - vector.y * std::sin(angle);
    //     rotatedVector.y = vector.x * std::sin(angle) + vector.y * std::cos(angle);
    //     vector.x = rotatedVector.x;
    //     vector.y = rotatedVector.y;
    // }

  private:
    Point vec;
};