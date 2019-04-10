#include <Map.h>

Map::Map(int _size) : size(_size)
{
    mapNodes = new MapNode *[size];
    for (int i = 0; i < size; i++)
    {
        mapNodes[i] = new MapNode(i);
    }
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

void Map::setNode(int id1, int id2, int direction)
{
    if (id1 != id2 && direction >= 0 && direction <= 4)
    {
        mapNodes[id1]->setNode(mapNodes[id2], direction);
        switch (direction)
        {
        case 0:
            mapNodes[id2]->setNode(mapNodes[id1], 2);
            break;
        case 1:
            mapNodes[id2]->setNode(mapNodes[id1], 3);
            break;
        case 2:
            mapNodes[id2]->setNode(mapNodes[id1], 0);
            break;
        case 3:
            mapNodes[id2]->setNode(mapNodes[id1], 1);
            break;
        default:
            mapNodes[id2]->setNode(mapNodes[id1], 4);
            break;
        }
    }
}

PathNode *Map::getPath(int currentId, int destinationId, int direction)
{
    if (destinationId >= size)
    {
        //destination does not exist
        return nullptr;
    }
    else if (currentId == destinationId)
    {
        return new PathNode(currentId, -1, nullptr, nullptr);
    }

    for (int i = 0; i < size; i++)
    {
        mapNodes[i]->visited = false;
    }

    //Setup array to hold all possibly paths
    //Hopefully the size won't be that big
    PathNode **possibilities = new PathNode *[size];

    //Set first posibility to the current node
    possibilities[0] = new PathNode(mapNodes[currentId]->id, -1, nullptr, nullptr);
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
        int count = mapNodes[possibilities[currentIndex]->id]->getCount();

        while (count)
        {
            //Get the first node to search from the current node
            PathNode *next = mapNodes[possibilities[currentIndex]->id]->getNextPath();

            //This should never be null
            if (next == nullptr)
            {
                //ERROR!!!!
                return nullptr;
            }

            //If there is already something in possibilities for this index delete it because we should be done with it by now
            if (possibilities[nextIndex++] != nullptr)
            {
                delete possibilities[nextIndex];
            }

            //Create a copy of the path, add on the next node, and put it in the next index
            possibilities[nextIndex] = possibilities[currentIndex]->setNext(next);

            //We found the destination
            if (possibilities[nextIndex - 1]->id == destinationId)
            {
                destinationIndex = nextIndex - 1;
                count = 0;
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
    PathNode *path = possibilities[destinationIndex];
    //delete the buffer
    delete possibilities;
    //return the solution
    return path;
}