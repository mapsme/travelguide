#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std;

struct GeoCoord
{
  int pageId;
  double lat;
  double lon;
  bool operator<(GeoCoord const & other) const
  {
    return pageId < other.pageId;
  }
};

typedef set<GeoCoord> GeoContainerT;

void FileToContainer(ifstream & file, GeoContainerT & outVec)
{
  string line;
  while (file.good())
  {
    getline(file, line);
    istringstream stream(line);
    GeoCoord c;
    stream >> c.pageId >> c.lat >> c.lon;
    if (c.lat < 90. && c.lat > -90. && c.lon < 180. && c.lon > -180.)
      outVec.insert(c);
  }
}


int main(int argc, char ** argv)
{
  if (argc < 3)
  {
    cout << "  Usage: " << argv[0] << " input_files..." << endl;
    cout << "    Merges geocodes from files in tab delimeted format into stdout:" << endl;
    cout << "    <page_id> <lat> <lon>" << endl;
    cout << "    Note that first file has priority over others." << endl;
    return 0;
  }

  GeoContainerT merged;
  for (int i = 1; i < argc; ++i)
  {
    ifstream file(argv[i]);
    FileToContainer(file, merged);
  }

  for (GeoContainerT::iterator it = merged.begin(); it != merged.end(); ++it)
    cout << it->pageId << '\t' << it->lat << '\t' << it->lon << endl;
  return 0;
}
