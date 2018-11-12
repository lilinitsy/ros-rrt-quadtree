#include "robot_controller.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "robot_controller");

    RobotController robot = RobotController();
    robot.run();

    return 0;
}
