#include <PathNode.h>

PathNode::PathNode(int id, int direction, PathNode *_next, PathNode *_previous) : id(id), direction(direction), next(_next), previous(_previous) {}

PathNode::~PathNode()
{
    if (previous != nullptr)
    {
        previous->setNext(nullptr);
        PathNode *p = previous;

        while (p->getPrevious() != nullptr)
        {
            p = p->getPrevious();
        }
        delete p;
    }
    if (next != nullptr)
    {
        next->setPrevious(nullptr);
        delete next;
    }
}

PathNode *PathNode::setNext(PathNode *_next)
{
    if (_next == nullptr)
    {
        next = _next;
        return this;
    }

    PathNode *copy = next;
    PathNode *previous = this;
    while (previous != nullptr)
    {
        copy->setPrevious(new PathNode(previous->id, previous->direction, _next, nullptr));
        previous = previous->getPrevious();
        copy = copy->getPrevious();
    }
    return _next;
}

void PathNode::setPrevious(PathNode *_previous)
{
    if (previous != nullptr)
    {
        delete previous;
    }
    previous = _previous;
}

PathNode *PathNode::getNext()
{
    return next;
}

PathNode *PathNode::getPrevious()
{
    return previous;
}