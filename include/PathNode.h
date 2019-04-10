#pragma once
class PathNode
{
public:
    PathNode(int id, int direction, PathNode *_next, PathNode *_previous);
    ~PathNode();
    PathNode *setNext(PathNode *_next);
    void setPrevious(PathNode *_previous);
    const int id;
    int direction;
    PathNode *getPrevious();
    PathNode *getNext();

private:
    PathNode *previous;
    PathNode *next;
};