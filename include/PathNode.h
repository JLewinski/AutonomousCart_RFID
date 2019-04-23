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
    void print(bool fromElseWhere = true);

private:
    void setNextFromPrevious(PathNode *_next);
    PathNode *previous;
    PathNode *next;
};