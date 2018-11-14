#include <ros/ros.h>

#include "Map.h"
#include "Robot.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "robot_controller");
	Robot robot = Robot();

	int start_x;
	int start_y;
	int end_x;
	int end_y;
	for(int i = 0; i < argc; i++)
	{
		if(strcmp(argv[i], "-s") == 0)
		{
			if(i + 1 < argc)
			{
				start_x = atoi(argv[i + 1]);
			}

			if(i + 2 < argc)
			{
				start_y = atoi(argv[i + 2]);
			}
		}

		if(strcmp(argv[i], "-e") == 0)
		{
			if(i + 1 < argc)
			{
				end_x = atoi(argv[i + 1]);
			}

			if(i + 2 < argc)
			{
				end_y = atoi(argv[i + 2]);
			}
		}
	}

	robot.run_rrt(start_x, start_y, end_x, end_y);
	return 0;
}