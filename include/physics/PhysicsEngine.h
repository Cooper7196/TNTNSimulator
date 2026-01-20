#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "robot/Robot.h"
#include <vector>

class PhysicsEngine {
public:
    PhysicsEngine();

    void addRobot(Robot* robot);
    void update(double dt);

private:
    std::vector<Robot*> robots;
};

#endif // PHYSICSENGINE_H
