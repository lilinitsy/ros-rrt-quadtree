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

	while(!goal_found())
	{
		// sample within a range of centre + stepsize * iterations
		// and then have a 10% chance the goal node is picked.
		int random_choice = rand() % 10;

		for(int i = 0; i < iterations; i++)
		{

		}
	}
}


RRTStatus RRT::extend(cv::Vec2i local_goal, RRTNode *current_node, ReadMapModule map)
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
		return GOAL_REACHED;
	}

	// POINT IS IN A WALL
	if(!valid_point(local_goal, map))
	{
		return TRAPPED;
	}

	cv::Vec2f unit_direction_vector = (cv::Vec2f) current_node->position - (cv::Vec2f) local_goal;
	
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
		return REACHED;
	}

	cv::Vec2f float_next_position = (cv::Vec2f) current_node->position + step_size * unit_direction_vector;
	cv::Vec2i next_position = (cv::Vec2i) float_next_position;
	
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

bool RRT::valid_point(cv::Vec2i point, ReadMapModule map)
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