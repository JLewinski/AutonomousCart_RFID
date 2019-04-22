#pragma once
#include <Direction.h>

class PathNode
{
public:
    PathNode(int id, Direction direction, PathNode *_next, PathNode *_previous);
    ~PathNode();
    PathNode *setNext(PathNode *_next);
    void setPrevious(PathNode *_previous);
    const int id;
    Direction direction;
    PathNode *getPrevious();
    PathNode *getNext();

private:
    PathNode *previous;
    PathNode *next;
};