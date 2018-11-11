#ifndef RRT_H
#define RRT_H


#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "RRTNode.h"

class RRT
{
    public:
        std::vector<RRTNode*> nodes;
        cv::Vec2i goal;

        RRT();
        /*
            So with extend:
                Don't need to raycast
                it should actually just return trapped, extended, or reached
                so just use valid_point to check if the point innit blocked
                COOL
        */
        void extend(cv::Vec2i point);
        void add_node(RRTNode *node);

        bool valid_point(cv::Vec2i point,)
};



#endif