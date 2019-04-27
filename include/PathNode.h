#pragma once
#include <Direction.h>

/**
 * This represents a location in a path. It is
 * used to allow the Map to build and store paths
 * from one destination and direction to another
 * destination. The path nodes are connected as a
 * double linked list where each node has a pointer
 * to both the previous and next node.
 */
class PathNode
{
public:
    //Initializes the node with these attributes
    PathNode(int id, Direction direction, PathNode *_next, PathNode *_previous);
    //Deletes this node and all connected nodes
    ~PathNode();
    //Returns a copy of this path with the next node attached to it
    PathNode *setNext(PathNode *_next);
    //Sets the previous node in the pat
    void setPrevious(PathNode *_previous);
    //ID of the location
    const int id;
    //Direction to go on the path or direction that needs
    //to be taken
    Direction direction;
    //Gets the previous node in the path
    PathNode *getPrevious();
    //Gets the next node in the path
    PathNode *getNext();
    //Prints the whole path to Serial
    void print(bool fromElseWhere = true);

private:
    //Sets the next node for the path without
    //modifying anything else
    void setNextFromPrevious(PathNode *_next);
    //Previous node in the path
    PathNode *previous;
    //Next node in the path
    PathNode *next;
};