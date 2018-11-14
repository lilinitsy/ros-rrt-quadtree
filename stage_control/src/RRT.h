#ifndef RRT_H
#define RRT_H


#include <vector>


#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


#include "Map.h"
#include "RRTNode.h"



enum RRTStatus
{
    TRAPPED,
    EXTENDED,
    REACHED,
    GOAL_REACHED
};


class RRT
{
    public:
        std::vector<RRTNode*> nodes;
        std::vector<RRTNode*> leaf_nodes;
        cv::Vec2i goal;
        int step_size;


        RRT();
        RRT(cv::Vec2i g, int sz);
        void build_rrt(cv::Vec2i start, ReadMapModule map, int iterations);

    private:
        RRTStatus extend(cv::Vec2i local_goal, RRTNode *current_node, ReadMapModule map, int iterations);
        void add_node(RRTNode *node); // useless?
        void remove_from_leaf_list(RRTNode *node);
        bool cell_blocked(cv::Vec2i point, ReadMapModule map);
        
        bool goal_found();    
        unsigned int get_closest_node_to_point(cv::Vec2i point);
        float distance(cv::Vec2i a, cv::Vec2i b);
        cv::Vec2i pick_local_goal_position(cv::Vec2i start, int iterations, ReadMapModule map); // search from leaf nodes
        cv::Vec2f lerp(cv::Vec2i vec);


};



#endif