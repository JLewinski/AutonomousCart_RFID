class MapNode
{
public:
  MapNode(int id);
  ~MapNode();
  void AddNode(MapNode *node, int direction);
  const int id;

private:
  //0: North, 1: East, 2: South, 3: West 4: other
  MapNode *nodes[5];
  bool visited;
}