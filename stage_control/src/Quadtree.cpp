


#include "Quadtree.h"


QuadTree::QuadTree()
{

}


QuadTree::QuadTree(AABB b, float mh)
{
	boundary = b;
	min_hd = mh;
}

void QuadTree::build_quadtree(ReadMapModule map)
{
	int bottom_x = boundary.center.val[0] - boundary.half_dimension;
	int bottom_y = boundary.center.val[1] - boundary.half_dimension;
	int top_x = boundary.center.val[0] + boundary.half_dimension;
	int top_y = boundary.center.val[1] + boundary.half_dimension;

	cv::Vec2i lower_bound = cv::Vec2i(bottom_x, bottom_y);
	cv::Vec2i upper_bound = cv::Vec2i(top_x, top_y);

	if(!cell_clear(map, lower_bound, upper_bound))
	{
		clear = false;
		if(boundary.half_dimension > min_hd && top_left == nullptr)
		{
			subdivide();
		}
	}
}


void QuadTree::subdivide()
{
	float child_hd = boundary.half_dimension / 2.0f;
	float child_hd = boundary.half_dimension / 2.0f;
	cv::Vec2i top_left_center = cv::Vec2i(
		boundary.center.val[0] - child_hd,
		boundary.center.val[1] + child_hd);
	cv::Vec2i top_right_center = cv::Vec2i(
		boundary.center.val[0] + child_hd,
		boundary.center.val[1] + child_hd);
	cv::Vec2i bottom_left_center = cv::Vec2i(
		boundary.center.val[0] - child_hd,
		boundary.center.val[1] - child_hd);
	cv::Vec2i bottom_right_center = cv::Vec2i(
		boundary.center.val[0] + child_hd,
		boundary.center.val[1] - child_hd);

	AABB top_left_boundary = AABB(top_left_center, child_hd);
	AABB top_right_boundary = AABB(top_right_center, child_hd);
	AABB bottom_left_boundary = AABB(bottom_left_center, child_hd);
	AABB bottom_right_boundary = AABB(bottom_right_center, child_hd);

	top_left = new QuadTree(top_left_boundary, min_hd);
	top_right = new QuadTree(top_right_boundary, min_hd);
	bottom_left = new QuadTree(bottom_left_boundary, min_hd);
	bottom_right = new QuadTree(bottom_right_boundary, min_hd);
}


bool QuadTree::cell_clear(ReadMapModule map, cv::Vec2i lower_bound, cv::Vec2i upper_bound)
{
	for(int i = lower_bound.val[0]; i < upper_bound.val[0]; i++)
	{
		for(int j = lower_bound.val[1]; j < upper_bound.val[1]; j++)
		{
			if(map.map[i][j].blocked)
			{
				return false;
			}
		}
	}

	printf("CELL IS CLEAR\n");
	return true;
}