#pragma once

#include "robot/Robot.hpp"
#include <vector>

namespace sim {

class PhysicsEngine {
public:
    PhysicsEngine();

    void addRobot(Robot* robot);
    void update(double dt);

private:
    std::vector<Robot*> robots;
};

}
