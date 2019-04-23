//#define DEBUG
#include <PathNode.h>
#ifdef DEBUG
#include <Arduino.h>
#endif

PathNode::PathNode(int id, Direction direction, PathNode *_next, PathNode *_previous) : id(id), direction(direction), next(_next), previous(_previous)
{
    if (next != nullptr)
    {
        next->setPrevious(this);
    }
    if (previous != nullptr)
    {
        previous->setNextFromPrevious(this);
    }
}

void PathNode::print(bool fromElseWhere = true)
{
#ifdef DEBUG
    if (fromElseWhere && previous != nullptr)
    {
        previous->print();
        return;
    }

    Serial.print(id);
    switch (direction)
    {
    case North:
        Serial.print(" North -> ");
        break;
    case South:
        Serial.print(" South -> ");
        break;
    case East:
        Serial.print(" East -> ");
        break;
    case West:
        Serial.print(" West -> ");
        break;
    case Stopped:
        Serial.println(" Stopped");
        return;
    default:
        Serial.print(" Other -> ");
        break;
    }
    if (next != nullptr)
    {
        next->print(false);
    }
    else
    {
        Serial.println("NA");
    }
#endif
}

PathNode::~PathNode()
{

    if (previous != nullptr)
    {
        previous->setNextFromPrevious(nullptr);
        delete previous;
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
        return nullptr;
    }
    else
    {
        _next->print();
    }

#ifdef DEBUG
    Serial.print("Before Copy: ");
    print();
#endif

    PathNode *copy = new PathNode(id, _next->direction, _next, nullptr);
    PathNode *previous = this->getPrevious();
    while (previous != nullptr)
    {
#ifdef DEBUG
        Serial.print("Copy: ");
        copy->print();
#endif

        copy = new PathNode(previous->id, previous->direction, copy, nullptr);
        // copy->setPrevious(temp);
        previous = previous->getPrevious();
        // copy = copy->getPrevious();
    }
#ifdef DEBUG
    Serial.print("Result: ");
    _next->print();
#endif
    return _next;
}

void PathNode::setPrevious(PathNode *_previous)
{
    previous = _previous;
    if (previous != nullptr)
        previous->setNextFromPrevious(this);
}

PathNode *PathNode::getNext()
{
    return next;
}

PathNode *PathNode::getPrevious()
{
    return previous;
}

void PathNode::setNextFromPrevious(PathNode *_next)
{
    next = _next;
}