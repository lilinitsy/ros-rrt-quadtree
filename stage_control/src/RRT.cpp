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


void RRT::build_rrt(cv::Vec2i start, ReadMapModule map)
{
	srand(37);

	while(!goal_found())
	{

	}
}


RRTStatus RRT::extend(cv::Vec2i local_goal, RRTNode *current_node, ReadMapModule map)
{
	if(distance(current_node->position, goal) <= 3)
	{
		RRTNode *global_goal_node = new RRTNode(goal);
		current_node->children.push_back(global_goal_node);
		nodes.push_back(global_goal_node);
		return REACHED;
	}

	if(!valid_point(local_goal, map))
	{
		return TRAPPED;
	}

	cv::Vec2f unit_direction_vector = (cv::Vec2f) current_node->position - (cv::Vec2f) local_goal;
	
	if(distance(current_node->position, local_goal) <= 3)
	{
		RRTNode *local_goal_node = new RRTNode(local_goal);
		current_node->children.push_back(local_goal_node);
		nodes.push_back(local_goal_node);
		return REACHED;
	}

	cv::Vec2f float_next_position = (cv::Vec2f) current_node->position + step_size * unit_direction_vector;
	cv::Vec2i next_position = (cv::Vec2i) float_next_position;
	
	if(!valid_point(next_position, map))
	{
		return TRAPPED;
	}

	RRTNode *next_node = new RRTNode(next_position);
	current_node->children.push_back(next_node);
	nodes.push_back(next_node);
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
	return true;
}

float RRT::distance(cv::Vec2i a, cv::Vec2i b)
{
    return sqrtf(powf(a.val[0] - b.val[0], 2.0f) + powf(a.val[1] - b.val[1], 2.0f));
}

bool RRT::goal_found()
{
//	for(int i = 0; i < )
}