#pragma once
#include <MapNode.h>
#include <Direction.h>

/**
 * This module is used to store location data and enable
 * the cart to navigate correctly from point A to point B
 * by making correct turns at nodes. The map holds all the
 * map nodes and currently sets up the map to a specific
 * building layout. In order for a user to input a custom
 * map they would either have to change the hard-coded map
 * for their specific floor-plan and RFID ids or would have
 * to create a UI and way to save the map.
 * 
 * This map is able to create a path to a destination if given
 * a starting location and direction. It does this using a
 * breadth first search (BFS) algorithm to find the shortest path.
 * There have been times when the map is unable to create a path
 * and the cause of this bug is still unknown.
 * 
 * The map stores the MapNodes as an array of points to pointers
 * to allow for immediate access to each node given that the id
 * of each node corresponds to the index in the array. This is
 * unnecessary, however it does allow for the possibility of
 * having multiple maps and faster time to immediately access
 * each node instead of having to search through all the nodes
 * to find a specific one.
 */
class Map
{
public:
  //Initialize a map of a specific size which cannot be changed
  Map(int _size);
  //Deletes all the map nodes
  ~Map();
  //Attaches one node to the other at a specific direction 1 direction -> 2
  void setNode(int id1, int id2, Direction direction);
  //Get the direction of the location as presented on the path
  Direction getDirection(int foundId);
  //Find a path from one location to another
  void setDestination(int currentId, int destinationId, Direction direction);
  //Size of the map
  const int size;
  //Has a current path (route)
  bool hasDestination();
  //Deletes the path
  void endRoute();
  //Sets up the map with hard-coded data
  void setup();
  //Prints each map node
  void print();

private:
  //All locations on the map
  MapNode **mapNodes;
  //The current path
  PathNode *currentPath;
  //The final destination of the path (unused)
  PathNode *finalDestination;
  //The current destination on the path (unused)
  PathNode *currentDestination;
};