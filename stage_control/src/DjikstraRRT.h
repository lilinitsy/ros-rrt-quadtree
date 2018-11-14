#ifndef DJIKSTRARRT_H
#define DJIKSTRARRT_H


#include <vector>

#include "RRTNode.h"
#include "RRT.h"


struct DjikstraRRTNode
{
	RRTNode *node;
	RRTNode *previous;
	float distance;
};


class DjikstraRRT
{
	public:
		std::vector<RRTNode*> path;

		DjikstraRRT();
		void find_path(std::vector<RRTNode*> graph, RRTNode *start, cv::Vec2i goal);
	
	private:
		DjikstraRRTNode min_distance_in_Q(std::vector<DjikstraRRTNode> Q);
		unsigned int get_node_index(DjikstraRRTNode u, std::vector<DjikstraRRTNode> Q);
		unsigned int get_node_index(RRTNode *u, std::vector<DjikstraRRTNode> Q);
		float distance(RRTNode *u, RRTNode *vchild);
		void reconstruction(std::vector<DjikstraRRTNode> tmp_path, RRTNode *start, cv::Vec2i goal);
};


#endif