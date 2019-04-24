#pragma once
#include <PathNode.h>

class MapNode
{
public:
  MapNode() : id(0) {}
  MapNode(int id) : id(id) {}
  ~MapNode();
  void setNode(MapNode *node, Direction direction);
  MapNode *getNext(Direction direction);
  PathNode *getNextPath();
  void getPaths(int previousId, PathNode **arr);
  int getCount();
  const int id;
  void print();

private:
  MapNode *nodes[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
};