#pragma once
#include <MapNode.h>

class Map
{
public:
  Map(int _size);
  ~Map();
  void setNode(int id1, int id2, int direction);
  int getDirection(int foundId);
  void setDestination(int currentId, int destinationId, int direction);
  const int size;

private:
  MapNode **mapNodes;
  PathNode *currentPath;
  PathNode *finalDestination;
  PathNode *currentDestination;
};