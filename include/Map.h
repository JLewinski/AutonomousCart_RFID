#include <MapNode.h>

class Map
{
  public:
    Map();
    ~Map();
    int getSize();

  private:
    MapNode mapNodes[];
    int size;
}