#include "DjikstraRRT.h"

DjikstraRRT::DjikstraRRT()
{

}

// FOLLOWS WIKIPEDIA PSEUDOCODE, SO VARIABLE NAMES WILL LOOK... VARI ASS
void find_path(std::vector<RRTNode*> graph, RRTNode *start)
{
	std::vector<DjikstraRRTNode> Q;

	// lines 5 through 8
	for(unsigned int i = 1; i < graph.size(); i++)
	{
		DjikstraRRTNode v;
		v.node = graph[i];
		v.previous = nullptr; // this is safe, right? RIGHT????
		v.distance = INFINITY;
	}

	// line 10
	for(unsigned int i = 0; i < Q.size(); i++)
	{
		if(Q[i].node == start)
		{
			Q[i].distance = 0;
		}
	}

	// line 12
	while(Q.size() > 0)
	{
		// line 13
		DjikstraRRTNode u = min_distance_in_Q(Q);

		// line 15
		unsigned int u_index = get_node_index(u, Q);
		Q.erase(Q.begin() + u_index);

		// line 17
		for(unsigned int i = 0; i < u.node->children.size(); i++)
		{
			float alt = u.distance + distance(u, u.node->children[i]);
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


float DjikstraRRT::distance(RRTNode *u, RRTNode *vchild)
{
	// opencv funcs
}