//#define DEBUG
#include <MapNode.h>
#ifdef DEBUG
#include <Arduino.h>
#endif

MapNode::~MapNode()
{
    delete[] nodes;
}

void MapNode::print()
{
#ifdef DEBUG
    Serial.print(id);
    Serial.print(": NORTH: ");
    int tempDir = (int)North;
    if (nodes[tempDir] == nullptr)
    {
        Serial.print("NA");
    }
    else
    {
        Serial.print(nodes[tempDir]->id);
    }
    Serial.print(" SOUTH: ");
    tempDir = (int)South;
    if (nodes[tempDir] == nullptr)
    {
        Serial.print("NA");
    }
    else
    {
        Serial.print(nodes[tempDir]->id);
    }
    Serial.print(" EAST: ");
    tempDir = (int)East;
    if (nodes[tempDir] == nullptr)
    {
        Serial.print("NA");
    }
    else
    {
        Serial.print(nodes[tempDir]->id);
    }
    Serial.print(" WEST: ");
    tempDir = (int)West;
    if (nodes[tempDir] == nullptr)
    {
        Serial.print("NA");
    }
    else
    {
        Serial.print(nodes[tempDir]->id);
    }
    Serial.println();
#endif
}

void MapNode::setNode(MapNode *node, Direction direction)
{
    if (direction >= 0 && direction <= 4 && node->id != id)
    {
#ifdef DEBUG
        Serial.print("Setting ");
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
        default:
            break;
        }
        Serial.println(node->id);
        if (nodes[direction] != nullptr)
        {
            Serial.println("Overwritting a pointer to a node. Could be causing some unreachable data");
        }
#endif
        nodes[direction] = node;
        switch (direction)
        {
        case North:
            if (node->getNext(South) == nullptr)
            {
                node->setNode(this, South);
            }
            break;
        case South:
            if (node->getNext(North) == nullptr)
            {
                node->setNode(this, North);
            }
            break;
        case East:
            if (node->getNext(West) == nullptr)
            {
                node->setNode(this, West);
            }
            break;
        case West:
            if (node->getNext(East) == nullptr)
            {
                node->setNode(this, East);
            }
            break;
        default:
            if (node->getNext(direction) == nullptr)
            {
                node->setNode(this, direction);
            }
            break;
        }
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
        if (nodes[i] != nullptr)
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
        if (nodes[i] != nullptr)
        {
            return new PathNode(nodes[i]->id, (Direction)i, nullptr, nullptr);
        }
    }
    return nullptr;
}

void MapNode::getPaths(int previousId, PathNode **arr)
{
    // Serial.print("Count: ");
    // Serial.println(count);
    int count = 1;
    for (int i = 0; i < 5; i++)
    {
        if (nodes[i] != nullptr && nodes[i]->id != previousId)
        {
#ifdef DEBUG
            PathNode *temp = new PathNode(nodes[i]->id, (Direction)i, nullptr, nullptr);
            Serial.print(count);
            count++;
            Serial.print(" Found: ");
            temp->print();
            arr[i] = temp;
            arr[i]->print();
#else
            arr[i] = new PathNode(nodes[i]->id, (Direction)i, nullptr, nullptr);
#endif
        }
        else
        {
            arr[i] = nullptr;
        }
    }
    for (int i = 0; i < 5; i++)
    {
        if (arr[i] != nullptr)
        {
            arr[i]->print();
        }
    }
}