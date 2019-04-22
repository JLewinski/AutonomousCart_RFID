#include <MapNode.h>

MapNode::~MapNode()
{
    delete[] nodes;
}

void MapNode::setNode(MapNode *node, Direction direction)
{
    if (direction >= 0 && direction <= 4 && node->id != id)
    {
        if (nodes[direction] != nullptr)
        {
            delete nodes[direction];
        }
        nodes[direction] = node;
    }
}

MapNode *MapNode::getNext(Direction direction)
{
    if (direction >= 0 && direction <= 4)
    {
        return nodes[direction];
    }
    return nullptr;
}

int MapNode::getCount()
{
    int count = 0;
    for (int i = 0; i < 5; i++)
    {
        if (nodes[i] != nullptr && !nodes[i]->visited)
        {
            count++;
        }
    }
    return count;
}

PathNode *MapNode::getNextPath()
{
    for (int i = 0; i < 5; i++)
    {
        if (nodes[i] != nullptr && !nodes[i]->visited)
        {
            nodes[i]->visited = true;
            return new PathNode(nodes[i]->id, (Direction)i, nullptr, nullptr);
        }
    }
    return nullptr;
}