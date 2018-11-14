


#include "Quadtree.h"


QuadTree::QuadTree()
{

}


QuadTree::QuadTree(AABB b, float mh)
{
	boundary = b;
	min_hd = mh;
}

void QuadTree::build_quadtree(ReadMapModule map, std::vector<QuadTree*> &clear_boxes)
{
	int bottom_x = boundary.center.val[0] - boundary.half_dimension_x;
	int bottom_y = boundary.center.val[1] - boundary.half_dimension_y;
	int top_x = boundary.center.val[0] + boundary.half_dimension_x;
	int top_y = boundary.center.val[1] + boundary.half_dimension_y;

	cv::Vec2i lower_bound = cv::Vec2i(bottom_x, bottom_y);
	cv::Vec2i upper_bound = cv::Vec2i(top_x, top_y);

	if(!cell_clear(map, lower_bound, upper_bound))
	{
		printf("FALSE\n");
		clear = false;

		// this node has children and is blocked..
		/*if(top_left != nullptr)
		{
			top_left->build_quadtree(map, clear_boxes);
			top_right->build_quadtree(map, clear_boxes);
			bottom_left->build_quadtree(map, clear_boxes);
			bottom_right->build_quadtree(map, clear_boxes);
		}*/

		// this node does not have children
		if(boundary.half_dimension_x > 2 * min_hd && boundary.half_dimension_y > 2 * min_hd && top_left == nullptr)
		{
			printf("In boundary check\n");
			subdivide();
			print_boxes();
			printf("SUBDIVIDING\n");
			top_left->build_quadtree(map, clear_boxes);
			top_right->build_quadtree(map, clear_boxes);
			bottom_left->build_quadtree(map, clear_boxes);
			bottom_right->build_quadtree(map, clear_boxes);
		}

		else if(boundary.half_dimension_x < 2 * min_hd && boundary.half_dimension_y < 2 * min_hd && top_left == nullptr)
		{
			return;
		}
	}

	else
	{
		printf("PUSHING BACK\n");
		clear_boxes.push_back(this); // IS THIS SAFE? I DON'T KNOW
		printf("CLEAR BOXES SIZE INTERNALL: %lu\n", clear_boxes.size());
	}

	return;
}


void QuadTree::subdivide()
{
	float child_hd_x = boundary.half_dimension_x / 2.0f;
	float child_hd_y = boundary.half_dimension_y / 2.0f;

	cv::Vec2i top_left_center = cv::Vec2i(
		boundary.center.val[0] - child_hd_x,
		boundary.center.val[1] + child_hd_y);
	cv::Vec2i top_right_center = cv::Vec2i(
		boundary.center.val[0] + child_hd_x,
		boundary.center.val[1] + child_hd_y);
	cv::Vec2i bottom_left_center = cv::Vec2i(
		boundary.center.val[0] - child_hd_x,
		boundary.center.val[1] - child_hd_y);
	cv::Vec2i bottom_right_center = cv::Vec2i(
		boundary.center.val[0] + child_hd_x,
		boundary.center.val[1] - child_hd_y);
	

	lerp(top_left_center + cv::Vec2i(child_hd_x, child_hd_y));
	lerp(top_right_center + cv::Vec2i(child_hd_x, child_hd_y));
	lerp(bottom_left_center + cv::Vec2i(child_hd_x, child_hd_y));
	lerp(bottom_right_center + cv::Vec2i(child_hd_x, child_hd_y));

	AABB top_left_boundary = AABB(top_left_center, child_hd_x, child_hd_y);
	AABB top_right_boundary = AABB(top_right_center, child_hd_x, child_hd_y);
	AABB bottom_left_boundary = AABB(bottom_left_center, child_hd_x, child_hd_y);
	AABB bottom_right_boundary = AABB(bottom_right_center, child_hd_x, child_hd_y);

	top_left = new QuadTree(top_left_boundary, min_hd);
	top_right = new QuadTree(top_right_boundary, min_hd);
	bottom_left = new QuadTree(bottom_left_boundary, min_hd);
	bottom_right = new QuadTree(bottom_right_boundary, min_hd);
}


bool QuadTree::cell_clear(ReadMapModule map, cv::Vec2i lower_bound, cv::Vec2i upper_bound)
{
	printf("MIN HD: %f\n", min_hd);
	printf("lower bound: %d %d\n", lower_bound.val[0], lower_bound.val[1]);
	printf("Upper bound: %d %d\n", upper_bound.val[0], upper_bound.val[1]);
	for(int i = lower_bound.val[0]; i < upper_bound.val[0]; i++)
	{
		for(int j = lower_bound.val[1]; j < upper_bound.val[1]; j++)
		{
			if(map.map[i][j].blocked)
			{
				printf("\n%d %d BLOCKED\n", i, j);
				return false;
			}
		}
	}

	printf("CELL IS CLEAR\n");
	return true;
}


void QuadTree::print_boxes()
{
	printf("Parent box center:\t %d %d half dims:\t%f, %f\n",
		boundary.center.val[0], boundary.center.val[1], 
		boundary.half_dimension_x, boundary.half_dimension_y);
	if(top_left != nullptr)
	{
		printf("\ttop_left box center:\t %d %d half dims:\t%f, %f\n",
			top_left->boundary.center.val[0], top_left->boundary.center.val[1], 
			top_left->boundary.half_dimension_x, top_left->boundary.half_dimension_y);
		printf("\ttop_right box center:\t %d %d half dims:\t%f, %f\n",
			top_right->boundary.center.val[0], top_right->boundary.center.val[1], 
			top_right->boundary.half_dimension_x, top_right->boundary.half_dimension_y);
		printf("\tbottom_left box center:\t %d %d half dims:\t%f, %f\n",
			bottom_left->boundary.center.val[0], bottom_left->boundary.center.val[1], 
			bottom_left->boundary.half_dimension_x, bottom_left->boundary.half_dimension_y);
		printf("\tbottom_right box center:\t %d %d half dims:\t%f, %f\n",
			bottom_right->boundary.center.val[0], bottom_right->boundary.center.val[1], 
			bottom_right->boundary.half_dimension_x, bottom_right->boundary.half_dimension_y);
	}
}

cv::Vec2f QuadTree::lerp(cv::Vec2i vec)
{
	cv::Vec2i new_vec = vec;
	if(vec.val[0] > 809)
	{
		new_vec.val[0] = 808;
	}

	else if(vec.val[0] < 0)
	{
		new_vec.val[0] = 0;
	}

	if(vec.val[1] > 689)
	{
		new_vec.val[1] = 688;
	}

	else if(vec.val[1] < 0)
	{
		new_vec.val[0] = 0;
	}

	return new_vec;
}