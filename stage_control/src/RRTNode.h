#ifndef RRTNODE_H
#define RRTNODE_H

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


struct RRTNode
{
    cv::Vec2i position;
    std::vector<RRTNode*> children;

    RRTNode();
    RRTNode(cv::Vec2i p);
};


#endif