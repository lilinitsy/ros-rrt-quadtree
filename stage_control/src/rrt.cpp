#include "rrt.h"

#include "map_read.h"

RRT::RRT()
{

}


void RRT::extend(geometry_msgs::Point point)
{
    
}


void RRT::add_node(RRTNode *node)
{
    nodes.push_back(node);

    if(nodes.size() > 0)
    {
        // Potential issue that the closest node will be between a wall...?
        int closest_node = get_closest_node_to_point(node->position);
        nodes[closest_node]->children.push_back(node);
    }
}


cv::Vec2i RRT::get_pixel_coordinates(float world_x, float world_y, float world_max_x, float world_max_y, int pixel_rows, int pixel_cols, float pixels_to_meters)
{
	float real_world_max_x = world_max_x / 2.0f;
	float real_world_max_y = world_max_y / 2.0f;
	float real_world_min_x = -1.0f * real_world_max_x;
	float real_world_min_y = -1.0f * real_world_max_y;

	int pixel_x = (0.5f * pixel_cols * (world_x - real_world_min_x)) / pixels_to_meters;
	int pixel_y = (0.5f * pixel_rows * (world_y - real_world_min_y)) / pixels_to_meters;
	return cv::Vec2i(pixel_x, pixel_y);
}


bool RRT::valid_point(geometry_msgs::Point point, geometry_msgs::Point robot_position, ReadMapModule map)
{
    float dist = distance(point, robot_position);
    cv::Vec2i pixel_point = get_pixel_coordinates((float) point.x, (float) point.y, 29.0f, 28.0f, 689, 809, 28.806f);
    cv::Vec2i pixel_robot = get_pixel_coordinates((float) robot_position.x, (float) robot_position.y, 29.0f, 28.0f, 689, 809, 28.806f);
   
    float slope;
    if((float) pixel_point.val[0] != (float) pixel_robot.val[0])
    {
        slope = ((float) pixel_point.val[1] - (float) pixel_robot.val[1]) / 
            ((float) pixel_point.val[0] - (float) pixel_robot.val[0]);
    }

    else
    {
        slope = 1; // TODO: Figure out a way to get a line traversal?
    }

    cv::Vec2f unit_direction_vector = cv::normalize(pixel_robot - pixel_point);
    float distance_traversed = 0;

    while(distance_traversed < dist)
    {
        distance_traversed += slope;
        cv::Vec2i next_pixels = pixel_robot + (cv::Vec2i) (distance_traversed * unit_direction_vector);
      //  std::cout << "Distance traversed: " << distance_traversed << std::endl;
    //    std::cout << "Next pixels: " << next_pixels;
      //  std::cout << "slope: " << slope << std::endl;
        if(map.map[next_pixels.val[0]][next_pixels.val[1]].blocked)
        {
            std::cout << "Point isn't  valid: " << point;
            return false;
        }
    }
    std::cout << "Valid point " << point;
    return true;
}



unsigned int RRT::get_closest_node_to_point(geometry_msgs::Point point)
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


float RRT::distance(geometry_msgs::Point a, geometry_msgs::Point b)
{
    return sqrtf(powf(a.x - b.x, 2.0f) + powf(a.y - b.y, 2.0f));
}