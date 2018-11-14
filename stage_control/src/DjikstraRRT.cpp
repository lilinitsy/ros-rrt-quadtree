#include <queue>


#include "DjikstraRRT.h"


DjikstraRRT::DjikstraRRT()
{

}

// FOLLOWS WIKIPEDIA PSEUDOCODE, SO VARIABLE NAMES WILL LOOK... VARI ASS
void DjikstraRRT::find_path(std::vector<RRTNode*> graph, RRTNode *start, cv::Vec2i goal)
{
	auto DjikstraCompare = [] (DjikstraRRTNode a, DjikstraRRTNode b)
	{
		return a.distance < b.distance;
	};
	std::priority_queue<DjikstraRRTNode, std::vector<DjikstraRRTNode>, decltype(DjikstraCompare)> Q(DjikstraCompare);

	for(unsigned int i = 0; i < graph.size(); i++)
	{
		DjikstraRRTNode v;
		v.node = graph[i]; // this may not matter?
		v.previous = nullptr;

		if(graph[i] != start)
		{
			v.distance = INFINITY;
		}

		else
		{
			v.distance = 0.0f;
		}

		Q.push(v);
	}

	// this is so I can pop shit off Q to get the nodes...

	while(!Q.empty())
	{
		std::priority_queue<DjikstraRRTNode, std::vector<DjikstraRRTNode>, decltype(DjikstraCompare)> hacked_Q = Q;
		DjikstraRRTNode u = hacked_Q.top();
		for(RRTNode *child : u.node->children)
		{
			printf("IN CHILDTHING\n");
			float alt = u.distance + distance(child, u.node);

			DjikstraRRTNode v;
			float v_dist = INFINITY;
			for(int i = 0; i < hacked_Q.size(); i++)
			{
				v = hacked_Q.top();
				Q.pop();
				if(v.node == child)
				{
					v_dist = v.distance;
					break;
				}
			}

			if(alt < v_dist)
			{
				v.distance = alt;
				v.previous = u.node;
				path.push_back(v);
				Q.pop();
			}
		}
	}
}

DjikstraRRTNode DjikstraRRT::min_distance_in_Q(std::vector<DjikstraRRTNode> Q)
{
	DjikstraRRTNode u = Q[0];

	for(unsigned int i = 0; i < Q.size(); i++)
	{
		if(Q[i].distance < u.distance)
		{
			u = Q[i];
		}
	}

	return u;
}


unsigned int DjikstraRRT::get_node_index(DjikstraRRTNode u, std::vector<DjikstraRRTNode> Q)
{
	for(unsigned int i = 0; i < Q.size(); i++)
	{
		if(u.node == Q[i].node)
		{
			return i;
		}
	}

	return 0; // SAFE? PROOOOBABLY NOT
}


unsigned int DjikstraRRT::get_node_index(RRTNode *u, std::vector<DjikstraRRTNode> Q)
{
	for(unsigned int i = 0; i < Q.size(); i++)
	{
		if(Q[i].node == u)
		{
			return i;
		}
	}

	return 0;
}	


float DjikstraRRT::distance(RRTNode *u, RRTNode *vchild)
{
	// opencv funcs
	cv::Vec2f a = u->position;
	cv::Vec2f b = vchild->position;
    return sqrtf(powf(a.val[0] - b.val[0], 2.0f) + powf(a.val[1] - b.val[1], 2.0f));
}