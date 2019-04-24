#pragma once
#include <MapNode.h>
#include <Direction.h>

class Map
{
public:
  Map(int _size);
  ~Map();
  void setNode(int id1, int id2, Direction direction);
  Direction getDirection(int foundId);
  void setDestination(int currentId, int destinationId, Direction direction);
  const int size;
  bool hasDestination();
  void endRoute();
  void setup();
  void print();

private:
  MapNode **mapNodes;
  PathNode *currentPath;
  PathNode *finalDestination;
  PathNode *currentDestination;
};