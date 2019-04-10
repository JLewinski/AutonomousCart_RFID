#pragma once
#include <MapNode.h>

class Map
{
public:
  Map(int _size);
  ~Map();
  void setNode(int id1, int id2, int direction);
  PathNode *getPath(int currentId, int destinationId, int direction);
  const int size;

private:
  MapNode **mapNodes;
};