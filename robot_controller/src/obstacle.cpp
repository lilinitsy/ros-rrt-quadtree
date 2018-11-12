#include "obstacle.h"

Obstacle::Obstacle()
{

}


Obstacle::Obstacle(cv::Vec3f p, cv::Vec3f s)
{
    position = p;
    size = s;
}
