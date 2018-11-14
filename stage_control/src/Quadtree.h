#ifndef QUADTREE_H
#define QUADTREE_H


#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


#include "Map.h"


struct AABB
{
	cv::Vec2i center;
	float half_dimension;

	AABB()
	{

	}


	AABB(cv::Vec2i c, float hd)
	{
		center = c;
		half_dimension = hd;
	}


	bool contains_point(cv::Vec2i point)
	{
		bool left_edge = point.val[0] >= center.val[0] - half_dimension;
		bool right_edge = point.val[0] <= center.val[0] + half_dimension;
		bool bottom_edge = point.val[1] >= center.val[1] - half_dimension;
		bool top_edge = point.val[1] <= center.val[1] + half_dimension;
	
		return (left_edge && right_edge && bottom_edge && top_edge);
	}


	bool intersectsAABB(AABB other)
	{
		// does this matter? TODO for now
	}
};


class QuadTree
{
	public:
		AABB boundary;
		QuadTree *top_left = nullptr;
		QuadTree *top_right = nullptr;
		QuadTree *bottom_left = nullptr;
		QuadTree *bottom_right = nullptr;
		float min_hd = 10;;
		bool clear = true;

		QuadTree();
		QuadTree(AABB boundary, float mh);
		void build_quadtree(ReadMapModule map);
		void subdivide();

	private:
		bool cell_clear(ReadMapModule map, cv::Vec2i lower_bound, cv::Vec2i upper_bound);
};











#endif