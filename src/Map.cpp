#include <Map.h>

Map::Map(int _size) : size(_size)
{
    mapNodes = new MapNode *[size];
    for (int i = 0; i < size; i++)
    {
        mapNodes[i] = new MapNode(i);
    }
    //HARD-CODE THE MAP HERE
}

Map::~Map()
{
    for (int i = 0; i < size; i++)
    {
        if (mapNodes[i] != nullptr)
        {
            delete mapNodes[i];
        }
    }
    delete mapNodes;
}

void Map::setNode(int id1, int id2, Direction direction)
{
    if (id1 != id2 && direction >= 0 && direction <= 4)
    {
        mapNodes[id1]->setNode(mapNodes[id2], direction);
        switch (direction)
        {
        case North:
            mapNodes[id2]->setNode(mapNodes[id1], South);
            break;
        case East:
            mapNodes[id2]->setNode(mapNodes[id1], West);
            break;
        case South:
            mapNodes[id2]->setNode(mapNodes[id1], North);
            break;
        case West:
            mapNodes[id2]->setNode(mapNodes[id1], East);
            break;
        default:
            mapNodes[id2]->setNode(mapNodes[id1], Other);
            break;
        }
    }
}

Direction Map::getDirection(int foundId)
{
    if (foundId == finalDestination->id)
    {
        return Stopped;
    }

    if (foundId == currentPath->id)
    {
        Direction direction = currentPath->direction;
        currentPath = currentPath->getNext();
        return direction;
    }
}

void Map::setDestination(int currentId, int destinationId, Direction direction)
{
    if (currentPath != nullptr)
    {
        delete currentPath;
    }
    if (destinationId >= size)
    {
        //destination does not exist
        currentPath = nullptr;
        return;
    }
    else if (currentId == destinationId)
    {
        currentPath = new PathNode(currentId, Other, nullptr, nullptr);
        return;
    }

    for (int i = 0; i < size; i++)
    {
        mapNodes[i]->visited = false;
    }

    //Setup array to hold all possibly paths
    //Hopefully the size won't be that big
    PathNode **possibilities = new PathNode *[size];

    if (0)
    {
        //This is just used to fix the comments not showing up as comments.
        //I'm not sure why, but if this if statement is taken out then vscode
        //won't display comments as comments until it's in the while loop
    }

    //Set first posibility to the current node
    possibilities[0] = new PathNode(mapNodes[currentId]->id, Other, nullptr, nullptr);
    
    //Init variables
    int destinationIndex = -1;
    int currentIndex = 0;
    int nextIndex = 1;

    //While the destination isn't found
    while (destinationIndex == -1)
    {
        //Set the current node to search from to the first if the current is pointing to null
        if (currentIndex == size || possibilities[currentIndex] == nullptr)
        {
            currentIndex = 0;
        }

        //Get the number of available nodes to search
        int nodeCount = mapNodes[possibilities[currentIndex]->id]->getCount();

        while (nodeCount)
        {
            //Get the first node to search from the current node
            PathNode *next = mapNodes[possibilities[currentIndex]->id]->getNextPath();

            //This should never be null
            if (next == nullptr)
            {
                //ERROR!!!!
                currentPath = nullptr;
                return;
            }

            //If there is already something in possibilities for this index delete it because we should be done with it by now
            if (possibilities[nextIndex] != nullptr)
            {
                delete possibilities[nextIndex];
            }

            //Create a copy of the path, add on the next node, and put it in the next index
            possibilities[nextIndex] = possibilities[currentIndex]->setNext(next);

            //We found the destination
            if (possibilities[nextIndex - 1]->id == destinationId)
            {
                destinationIndex = nextIndex - 1;
                nodeCount = 0;
            }

            //Circulare buffer (it shoud be finished)
            if (nextIndex == size)
            {
                nextIndex = 0;
            }
        }
        //Go to next possibility
        currentIndex++;
    }

    //delete everything in possibilities except the solution
    for (int i = 0; i < size; i++)
    {
        if (i != destinationIndex && possibilities[i] != nullptr)
        {
            delete possibilities[i];
        }
    }

    //save the solution
    currentPath = possibilities[destinationIndex];
    //delete the buffer
    delete possibilities;
    //return the solution
}