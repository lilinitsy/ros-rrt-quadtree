#include "DjikstraRRT.h"


DjikstraRRT::DjikstraRRT()
{

}

// FOLLOWS WIKIPEDIA PSEUDOCODE, SO VARIABLE NAMES WILL LOOK... VARI ASS
void DjikstraRRT::find_path(std::vector<RRTNode*> graph, RRTNode *start, cv::Vec2i goal)
{
	std::vector<DjikstraRRTNode> Q;
	std::vector<DjikstraRRTNode> tmp_path;

	// lines 5 through 8
	for(unsigned int i = 0; i < graph.size(); i++)
	{
		DjikstraRRTNode v;
		v.node = graph[i];
		v.previous = nullptr; // this is safe, right? RIGHT????
		v.distance = INFINITY;
		Q.push_back(v);
	}

	// line 10
	for(unsigned int i = 0; i < Q.size(); i++)
	{
		if(Q[i].node == start)
		{
			Q[i].distance = 0;
			break;
		}
	}

	// line 12
	while(Q.size() > 0)
	{
		// line 13
		DjikstraRRTNode u = min_distance_in_Q(Q);

		// line 15
		unsigned int u_index = get_node_index(u, Q);
		if(Q[u_index].node->position == goal)
		{
			reconstruction(tmp_path, start, goal);
			return;
		}
		Q.erase(Q.begin() + u_index);

		// line 17
		for(unsigned int i = 0; i < u.node->children.size(); i++)
		{
			// line 18
			float alt = u.distance + distance(u.node, u.node->children[i]);
			unsigned int v_index = get_node_index(u.node->children[i], Q);

			// line 19
			if(alt < Q[v_index].distance)
			{
				// line 20
				Q[v_index].distance = alt;
				// line 21
				Q[v_index].previous = u.node;
				// basically line 23
				tmp_path.push_back(Q[v_index]);
			}
		}
	}

	reconstruction(tmp_path, start, goal);
}

void DjikstraRRT::reconstruction(std::vector<DjikstraRRTNode> tmp_path, RRTNode *start, cv::Vec2i goal)
{
	for(unsigned int i = tmp_path.size() - 1; i > 0; i--)
	{
		if(tmp_path[i].previous != nullptr)
		{
			path.push_back(tmp_path[i].previous);
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