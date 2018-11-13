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
		printf("Z: %d\n\n", z);

		// and then have a 10% chance the goal node is picked.
		int random_choice = rand() % 20;
		cv::Vec2i local_goal_position;

		// 40% chance we choose the goal as the node we want to try to reach
		if(random_choice > 12 || distance(start, goal) < step_size * iterations)
		{
			local_goal_position = goal;
		}

		else
		{
			local_goal_position = pick_local_goal_position_global_version(map);
		}

		unsigned int closest_node = get_closest_node_to_point(local_goal_position);
		start = nodes[closest_node]->position;
		//printf("local_goal_position: %d %d\n", local_goal_position.val[0], local_goal_position.val[1]);
		//printf("closest node position: %d %d\n", start.val[0], start.val[1]);
		RRTStatus status = extend(local_goal_position, nodes[closest_node], map, iterations);
	}
}


RRTStatus RRT::extend(const cv::Vec2i local_goal, RRTNode *current_node, ReadMapModule map, int iterations)
{
// MIGHT HAVE TO DO RECURSIVELY WITH iterations == 0?	
	/*
		First check if we've reached the global goal
			- if so, return GOAL_REACHED to stop building
				(TODO: Add a parameter to keep building 'for a while" after)
	*/

	printf("ITERATIONS: %d\n", iterations);

	if(iterations == 0)
	{
		return EXTENDED;
	}

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
		return GOAL_REACHED;
	}

	// POINT IS IN A WALL
	if(!valid_point(local_goal, map))
	{	
		return TRAPPED;
	}

	cv::Vec2f unit_direction_vector = cv::normalize((cv::Vec2f) current_node->position - (cv::Vec2f) local_goal);

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
		return REACHED;
	}


	//cv::Vec2i next_position = lerp((cv::Vec2f) current_node->position + step_size * unit_direction_vector);
	cv::Vec2i next_position = (cv::Vec2f) current_node->position + step_size * unit_direction_vector;
	next_position = lerp(next_position);

	// EXTENSION NODE IS INVALID -> TRAPPED
	if(!valid_point(next_position, map))
	{
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
	return extend(local_goal, next_node, map, iterations - 1);
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
	//printf("i, j: %d, %d\n", i, j);
	return !map.map[i][j].blocked;
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
	//printf("start.val[0]: %d\t step_size: %d\t iterations: %d\n", start.val[0], step_size, iterations);
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

	int x_pos = rand() % (xmax - xmin + 1) + xmin;
	int y_pos = rand() % (ymax - ymin + 1) + ymin;
	return cv::Vec2i(x_pos, y_pos);
}


cv::Vec2i RRT::pick_local_goal_position_global_version(ReadMapModule map) // search from leaf nodes
{
	int x_pos = rand() % 808;
	int y_pos = rand() & 688;
	printf("x_pos: %d, y_pos: %d\n", x_pos, y_pos);
	return cv::Vec2i(x_pos, y_pos);
}


// THIS FUNCTION MAY HAVE BEEN A STUPID IDEA
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


cv::Vec2f RRT::lerp(cv::Vec2i vec)
{
	cv::Vec2i new_vec = vec;
	if(vec.val[0] > 809)
	{
		new_vec.val[0] = 808;
	}

	else if(vec.val[0] < 0)
	{
		new_vec.val[0] = 0;
	}

	if(vec.val[1] > 689)
	{
		new_vec.val[1] = 688;
	}

	else if(vec.val[1] < 0)
	{
		new_vec.val[0] = 0;
	}

	return new_vec;
}