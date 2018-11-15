# ros-rrt-quadtree

## Currently Finished Features
- The RRT is fully constructed and does not collide with obstacles.
The beginning is visuzlied with a pink circle, and the end with a green one.
![alt-text](https://github.com/lilinitsy/ros-rrt-quadtree/blob/messier/rrt.png)
- The quadtree is fully constructed. 
Shown are the valid bounding boxes through which the robot can traverse, outlined in green.
![alt-text]https://github.com/lilinitsy/ros-rrt-quadtree/blob/messier/quadtree.png)

## Unfinished Features
- Djikstra's was never finished. 
There is nonworking code for the RRT. 
Truly, this should not have been difficult to implement, since I've written Djikstra implementations many times before (https://github.com/lilinitsy/hackrender/blob/master/motion_planning/src/Djikstra.h). 
I don't know why it was.
- Not visualized in stage.
- Path smoothing.
Path smoothing would make a lot of sense when there is a faster path between nodes in a tree, and could be done as a post-processing step before Djikstra's / A* are run.

## Real World Considerations
- The paths for the RRT's try to be 3 to 4 pixels away from any obstacles (or more, depending on what the variable ``step_size`` is made to be in RRT.h).
- In both algorithms, termination occurs when the goal is finally connected to the tree; this means that a path-finding algorithm finds the *first* eligable path to the goal, but it would not be optimal (not that in continuous space, a path truly will be, but the optimal direction / general route could be searched for).

## Comparisons
- RRT:
  - rosrun stage_control rrtplanner -s 128 28 -e 476 476  0.15s user 0.05s system 44% cpu 0.447 total
  - This was with all print statements disabled. The pushed code has them because they are very useful for debugging.
  - I set the RRT to run for a maximum of 3000 iterations, but in most cases, it's running for 400-600.
  - The RRT is also not comprehensive across the map. Many regions are left unexplored, because I terminate when I find the first path.
  
- QuadTree:
  - rosrun stage_control qtplanner  0.32s user 0.25s system 7% cpu 7.994 total
  - This just decomposes to the minimum decomposition depth (the biggest bounding boxes possible), so it makes sense that it is a lot faster. It's also easier to check for obstacles, since I don't have to step 3-4 pixels at a time and do a sort-of pseudo raycast.
  - This created around 320 boxes / nodes. It should be remembered that parent nodes *contain* all their children, so there are fewer *look to be* drawn on the screen (cause lines overlap).
  - It makes sense that the quadtree, constructed like this, would  be faster.
  - Path planning should also be faster, as there tend to be fewer nodes. 
  - (Quad/Octrees are also *phenomenal* spatial data structures to handle collision detection, as making them, traversing them, and checking them is very fast).
  - Without path smoothing, this path should be less complex.
  - With path smoothing, I'd still lean towards the quadtree, but I'm not certain.
