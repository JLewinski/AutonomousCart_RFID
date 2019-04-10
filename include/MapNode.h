#pragma once
#include <PathNode.h>

class MapNode
{
public:
  MapNode() : id(0) {}
  MapNode(int id) : id(id) {}
  ~MapNode();
  void setNode(MapNode *node, int direction);
  MapNode *getNext(int direction);
  PathNode *getNextPath();
  int getCount();
  const int id;
  bool visited;

private:
  //0: North, 1: East, 2: South, 3: West 4: other
  MapNode *nodes[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
};