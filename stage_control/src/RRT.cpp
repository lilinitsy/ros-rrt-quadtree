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
	RRTNode *start_node = new RRTNode(start);
	nodes.push_back(start_node);
	leaf_nodes.push_back(start_node);
	int z = 0;
	cv::Vec2i begin_pos = start;

	// while(!goal_found()) // might be preferable
	while(z < 3000)
	{
		z++;
		int random_choice = rand() % 20;
		cv::Vec2i local_goal_position;
		RRTNode *begin_node;

		// 10% chance we choose the goal as the node we want to try to reach
		if(random_choice > 17 || distance(begin_pos, goal) < step_size * iterations)
		{
			local_goal_position = goal;
			unsigned int closest_node = get_closest_node_to_point(goal);
			begin_pos = nodes[closest_node]->position;
			begin_node = nodes[closest_node];
		//	printf("Picking goal: %d %d\n", local_goal_position.val[0], local_goal_position.val[1]);
		}

		else
		{
			local_goal_position = pick_local_goal_position(nodes[nodes.size() - 1]->position, iterations, map);
			unsigned int closest_node = get_closest_node_to_point(local_goal_position);
			begin_pos = nodes[closest_node]->position;
			begin_node = nodes[closest_node];
		}

		RRTStatus status = extend(local_goal_position, begin_node, map, iterations);
		if(status == GOAL_REACHED)
		{
			printf("GOAL REACHED\n");
			printf("Z: %d\n", z);
			break;
		}

		else if(status == TRAPPED)
		{
			printf("TRAPPED\n");
		}
	}

	if(z >= 3000)
	{
		printf("goal not reached in %d iterations\n", z);
	}
}


RRTStatus RRT::extend(const cv::Vec2i local_goal, RRTNode *current_node, ReadMapModule map, int iterations)
{
	// ITERATED AS MUCH AS WE WANT
	if(iterations == 0)
	{
		return EXTENDED;
	}

	// POINT IS IN A WALL
	if(cell_blocked(local_goal, map))
	{	
		printf("%d %d LOCAL GOAL IS NOT VALID", local_goal.val[0], local_goal.val[1]);
		return TRAPPED;
	}

	// WE CAN JUST GO TO THE GLOBAL GOAL, SINCE IT'S WITHIN A STEP_SIZE
	if(distance(current_node->position, goal) <= step_size)
	{		
		if(current_node->children.size() > 0)
		{
			remove_from_leaf_list(current_node);
		}

		RRTNode *global_goal_node = new RRTNode(goal);
		current_node->children.push_back(global_goal_node);
		nodes.push_back(global_goal_node);
		leaf_nodes.push_back(global_goal_node);
		return GOAL_REACHED;
	}

	// TYPICAL EXTENSION: Try taking another step
	cv::Vec2f unit_direction_vector = cv::normalize((cv::Vec2f) local_goal - (cv::Vec2f) current_node->position);
	cv::Vec2i next_position = (cv::Vec2f) current_node->position + step_size * unit_direction_vector;
	next_position = lerp(next_position);

	// EXTENSION NODE IS INVALID -> TRAPPED
	if(cell_blocked(next_position, map))
	{
		printf("%d %d was the next position BUT IT IS BLOCKED\n", next_position.val[0], next_position.val[1]);
		return TRAPPED;
	}

	// CAN TOUCH THE LOCAL GOAL -> REACHED
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

	// CAN'T TOUCH LOCAL GOAL, BUT NEXT STEP IS VALID
	// EXTEND
	if(current_node->children.size() > 0)
	{
		remove_from_leaf_list(current_node);
	}

	RRTNode *next_node = new RRTNode(next_position);
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


bool RRT::cell_blocked(const cv::Vec2i point, ReadMapModule map)
{
	int i = point.val[0];
	int j = point.val[1];
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