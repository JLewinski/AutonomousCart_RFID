#pragma once
#include <PathNode.h>

/**
 * This is a location on the map. It is
 * connected to one to 5 map nodes with
 * each connected node at one of the four
 * cardinal directions or connected to the
 * "OTHER" direction. Each node has an ID.
 */
class MapNode
{
public:
  //Creates a new map node with id of 0
  MapNode() : id(0) {}
  //Creates a new map node with a given id
  MapNode(int id) : id(id) {}
  //Attaches a node at a given direction
  void setNode(MapNode *node, Direction direction);
  //Gets the attached node at a given direction
  MapNode *getNext(Direction direction);
  //Gets all the possible next locations in the form of a path node.
  //These next locations are stored in arr.
  void getPaths(int previousId, PathNode **arr);
  //Id of the location
  const int id;
  //Prints the specific map location with the id of each node in it's direction
  void print();

private:
  //Attached nodes
  MapNode *nodes[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
};