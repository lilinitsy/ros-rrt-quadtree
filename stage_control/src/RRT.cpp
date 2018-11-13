#include "RRT.h"

RRT::RRT()
{
	goal = cv::Vec2i(0, 0);
}


RRT::RRT(cv::Vec2i g, int sz)
{
	goal = g;
	step_size = sz;
}


void RRT::build_rrt(cv::Vec2i start, ReadMapModule map, int iterations)
{
	srand(37);
	RRTNode *begin_node = new RRTNode(start);
	nodes.push_back(begin_node);
	leaf_nodes.push_back(begin_node);
	int z = 0;
	//while(!goal_found())
	while(z < 1000)
	{
		z++;
		// sample within a range of centre + stepsize * iterations
		// and then have a 10% chance the goal node is picked.
		int random_choice = rand() % 20;
		//printf("Random choice: %d\n", random_choice);

		cv::Vec2i local_goal_position;
		// 10% chance we choose the goal as the node we want to try to reach
		if(random_choice > 17 || distance(start, goal) < step_size * iterations)
		{
			local_goal_position = goal;
		}

		else
		{
			local_goal_position = pick_local_goal_position(start, iterations, map);
		}

		printf("LOCAL GOAL POSITION: %d %d\n", local_goal_position.val[0], local_goal_position.val[1]);

		for(int i = 0; i < iterations; i++)
		{
			unsigned int closest_node = get_closest_node_to_point(local_goal_position);
			RRTStatus status = extend(local_goal_position, nodes[closest_node], map);
			printf("z: %d i: %d: Status: %d\n", z, i, status);
			if(status == TRAPPED || status == REACHED || status == GOAL_REACHED)
			{
				break;
			}
		}
	}
}


RRTStatus RRT::extend(const cv::Vec2i local_goal, RRTNode *current_node, ReadMapModule map)
{
	/*
		First check if we've reached the global goal
			- if so, return GOAL_REACHED to stop building
				(TODO: Add a parameter to keep building 'for a while" after)
	*/
	if(distance(current_node->position, goal) <= step_size)
	{
		RRTNode *global_goal_node = new RRTNode(goal);
		
		if(current_node->children.size() > 0)
		{
			remove_from_leaf_list(current_node);
		}

		current_node->children.push_back(global_goal_node);
		nodes.push_back(global_goal_node);
		leaf_nodes.push_back(global_goal_node);
		printf("GOAL REACHED\n");
		return GOAL_REACHED;
	}

	// POINT IS IN A WALL
	if(!valid_point(local_goal, map))
	{
		printf("TRAPPED\n");
		printf("Point trapped local_goal: %d, %d\n", local_goal.val[0], local_goal.val[1]);
		return TRAPPED;
	}

	cv::Vec2f unit_direction_vector = make_non_negative(cv::normalize((cv::Vec2f) current_node->position - (cv::Vec2f) local_goal));
	
	// LOCAL GOAL REACHED
	if(distance(current_node->position, local_goal) <= step_size)
	{
		RRTNode *local_goal_node = new RRTNode(local_goal);

		if(current_node->children.size() > 0)
		{
			remove_from_leaf_list(current_node);
		}
		
		current_node->children.push_back(local_goal_node);
		nodes.push_back(local_goal_node);
		leaf_nodes.push_back(local_goal_node);
		printf("LOCAL GOAL REACHED\n");
		return REACHED;
	}

	cv::Vec2i next_position = (cv::Vec2f) current_node->position + step_size * unit_direction_vector;
	printf("next position: %d, %d\n", next_position.val[0], next_position.val[1]);

	/*
	printf("current node position: %d %d\n", current_node->position.val[0], current_node->position.val[1]);
	printf("Step size: %d\n", step_size);
	printf("unit direction vector: %f %f\n", unit_direction_vector.val[0], unit_direction_vector.val[1]);
	printf("next position: %d %d\n\n", next_position.val[0], next_position.val[1]);
	*/
	// EXTENSION NODE IS INVALID -> TRAPPED
	if(!valid_point(next_position, map))
	{
		printf("TRAPPEDDDD\n");
		return TRAPPED;
	}

	// CAN EXTEND

	RRTNode *next_node = new RRTNode(next_position);
	
	if(current_node->children.size() > 0)
	{
		remove_from_leaf_list(current_node);
	}

	current_node->children.push_back(next_node);
	nodes.push_back(next_node);
	leaf_nodes.push_back(next_node);
	printf("EXTENDED\n");
	return EXTENDED;
}


unsigned int RRT::get_closest_node_to_point(cv::Vec2i point)
{
	if(nodes.size() > 0)
    {
        int closest_index = 0; 
        RRTNode *closest_node = nodes[0]; // default
        float min_distance = distance(closest_node->position, point);

        // just do a linear search, it should be fine... right????
        for(unsigned int i = 1; i < nodes.size(); i++)
        {
            float dist = distance(nodes[i]->position, point);
            if(dist < min_distance)
            {
                min_distance = dist;
                closest_node = nodes[i];
                closest_index = i;
            }
        }

        return closest_index;
    }

    return 0; // unsafe? yeah!!
}

bool RRT::valid_point(const cv::Vec2i point, ReadMapModule map)
{
	int i = point.val[0];
	int j = point.val[1];
	printf("i, j: %d, %d\n", i, j);
	return map.map[i][j].blocked;
}

float RRT::distance(cv::Vec2i a, cv::Vec2i b)
{
    return sqrtf(powf(a.val[0] - b.val[0], 2.0f) + powf(a.val[1] - b.val[1], 2.0f));
}

bool RRT::goal_found()
{
	if(nodes.size() > 0)
	{
		for(unsigned int i = 0; i < nodes.size(); i++)
		{
			if(nodes[i]->position == goal)
			{
				return true;
			}
		}
	}

	return false;
}

void RRT::remove_from_leaf_list(RRTNode *node)
{
	for(unsigned int i = 0; i < leaf_nodes.size(); i++)
	{
		if(leaf_nodes[i] == node)
		{
			leaf_nodes.erase(leaf_nodes.begin() + i);
		}
	}
}


cv::Vec2i RRT::pick_local_goal_position(cv::Vec2i start, int iterations, ReadMapModule map)
{
	int xmin = start.val[0] - step_size * iterations;
	int xmax = start.val[0] + step_size * iterations;
	int ymin = start.val[1] - step_size * iterations;
	int ymax = start.val[1] + step_size * iterations;
	printf("start.val[0]: %d\t step_size: %d\t iterations: %d\n", start.val[0], step_size, iterations);
	if(xmin < 0)
	{
		xmin = 0;
	}
	if(ymin < 0)
	{
		ymin = 0;
	}
	if(xmax >= map.max_pixels.val[0])
	{
		xmax = map.max_pixels.val[0] - 1;
	}
	if(ymax >= map.max_pixels.val[0])
	{
		ymax = map.max_pixels.val[0] - 1;
	}

	printf("xmin, xmax: %d %d\n", xmin, xmax);
	printf("ymin, ymax: %d %d\n", ymin, ymax);


	int x_pos = rand() % (xmax - xmin + 1) + xmin;
	int y_pos = rand() % (ymax - ymin + 1) + ymin;
	return cv::Vec2i(x_pos, y_pos);
}


float RRT::make_non_negative(cv::Vec2f vect)
{
	if(vect.val[0] < 0.0f)
	{
		vect.val[0] = 0.0f;
	}

	if(vect.val[1] < 0.0f)
	{
		vect.val[1] = 0.0f;
	}
}