#include <Map.h>
//#define DEBUG
#ifdef DEBUG
#include <Arduino.h>
#endif

Map::Map(int _size) : size(_size)
{
    mapNodes = new MapNode *[size];
    for (int i = 0; i < size; i++)
    {
        mapNodes[i] = new MapNode(i);
    }
}

void Map::print()
{
    for (int i = 0; i < size; i++)
    {
        mapNodes[i]->print();
    }
}

void Map::setup()
{
    //HARD-CODE THE MAP HERE
    mapNodes[0]->setNode(mapNodes[1], North);
    mapNodes[0]->setNode(mapNodes[2], South);
    mapNodes[1]->setNode(mapNodes[3], East);
    mapNodes[3]->setNode(mapNodes[7], South);
    mapNodes[7]->setNode(mapNodes[0], West);
    mapNodes[2]->setNode(mapNodes[4], West);
    mapNodes[2]->setNode(mapNodes[5], South);
    mapNodes[5]->setNode(mapNodes[6], West);
    mapNodes[6]->setNode(mapNodes[4], North);
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

    PathNode *cp = currentPath;
    while (cp != nullptr)
    {
        if (cp->id == foundId)
        {
            Direction direction = cp->direction;
            currentPath = cp->getNext();
            return direction;
        }
    }
    return Other;
}

void Map::setDestination(int currentId, int destinationId, Direction direction)
{
#ifdef DEBUG
    Serial.println("Finding Path");
#endif

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
    else if (direction == Stopped || direction == Other || mapNodes[currentId] == nullptr || mapNodes[currentId]->getNext(direction) == nullptr)
    {
        return;
    }

#ifdef DEBUG
    Serial.println("Setting first two locations");
#endif

    //Set first posibility to the current node
    currentPath = new PathNode(mapNodes[currentId]->id, direction, nullptr, nullptr);
    currentPath = new PathNode(mapNodes[currentId]->getNext(direction)->id, Other, nullptr, currentPath);
#ifdef DEBUG
    Serial.println("Set second location");
#endif
    if (mapNodes[currentId]->getNext(direction)->id == destinationId)
    {
        return;
    }

#ifdef DEBUG
    Serial.println("Creating Array");
#endif

    //Setup array to hold all possibly paths
    //Hopefully the size won't be that big
    PathNode **possibilities = new PathNode *[size * 3];
    possibilities[0] = currentPath;
    for (int i = 1; i < size * 3; i++)
    {
        possibilities[i] = nullptr;
    }

    if (0)
    {
        //This is just used to fix the comments not showing up as comments.
        //I'm not sure why, but if this if statement is taken out then vscode
        //won't display comments as comments until it's in the while loop
    }

    //Init variables
    int destinationIndex = -1;
    int currentIndex = 0;
    int nextIndex = 0;

#ifdef DEBUG
    Serial.println("Starting loop");
    print();
    possibilities[0]->print();
#endif

    //While the destination isn't found
    while (destinationIndex == -1)
    {
#ifdef DEBUG
        Serial.print("SEARCHING: ");
#endif
        //Set the current node to search from to the first if the current is pointing to null
        if (currentIndex == size || possibilities[currentIndex] == nullptr)
        {
            currentIndex = 0;
        }
#ifdef DEBUG
        Serial.print(currentIndex);
        Serial.print(" (");
        Serial.print(possibilities[currentIndex]->id);
        Serial.println(")");
#endif
        //Get the number of available nodes to search
        PathNode *next[5];
        mapNodes[possibilities[currentIndex]->id]->getPaths(possibilities[currentIndex]->getPrevious()->id, next);
#ifdef DEBUG
        int c = 0;
        Serial.println("Paths: ");
        for (int i = 0; i < 5; i++)
        {
            if (next[i] != nullptr)
            {
                Serial.print(c);
                Serial.print(" [");
                Serial.print(i);
                Serial.print("]: ");
                next[i]->print();
                c++;
            }
        }
        Serial.print("Found ");
        Serial.print(c);
        Serial.print(" path(s) from ");
        mapNodes[possibilities[currentIndex]->id]->print();
#endif
        // Serial.println(next[0]->id);
        for (int i = 0; i < 5; i++)
        {
            if (next[i] != nullptr)
            {
#ifdef DEBUG
                Serial.print("Adding: ");
                next[i]->print();
#endif
                nextIndex++;
                if (nextIndex == size * 3)
                {
                    nextIndex = 0;
                }
#ifdef DEBUG
                if (nextIndex == currentIndex)
                {
                    Serial.println("Override currentIndex");
                }
#endif
                //Create a copy of the path, add on the next node, and put it in the next index
                possibilities[nextIndex] = possibilities[currentIndex]->setNext(next[i]);

                //We found the destination
                if (possibilities[nextIndex]->id == destinationId)
                {
                    destinationIndex = nextIndex;
                    possibilities[nextIndex]->direction = Stopped;
                    break;
                }

                //Circulare buffer (it shoud be finished)
            }
        }

        // delete next;

        //Go to next possibility
        currentIndex++;
    }
#ifdef DEBUG
    Serial.println("Done searching");
#endif

    currentPath = possibilities[destinationIndex];
#ifdef DEBUG
    currentPath->print();

    delay(10);
    Serial.println("Deleting Array");
    int count = 0;
#endif

    //delete everything in possibilities except the solution
    for (int i = 0; i < size * 3; i++)
    {
        if (i != destinationIndex && possibilities[i] != nullptr)
        {
            delete possibilities[i];
#ifdef DEBUG
            count++;
#endif
        }
    }

    //save the solution
    //delete the buffer
    delete possibilities;
    //return the solution

#ifdef DEBUG
    Serial.print("Deleted ");
    Serial.println(count);

    currentPath->print();
    while (currentPath->getPrevious() != nullptr)
    {
        currentPath = currentPath->getPrevious();
    }
    currentPath->print();
#else
    while (currentPath->getPrevious() != nullptr)
    {
        currentPath = currentPath->getPrevious();
    }
#endif
}

bool Map::hasDestination()
{
    return finalDestination != nullptr;
}

void Map::endRoute()
{
    if (finalDestination != nullptr)
        delete finalDestination;
    if (currentDestination != nullptr)
        delete currentDestination;
    if (currentPath != nullptr)
        delete currentPath;
}