#include "../../storage/storage_builder.hpp"

#include "../../std/iostream.hpp"


int main(int argc, char const * argv[])
{
  if (argc == 5)
  {
    StorageBuilder builder;
    builder.ParseEntries(argv[1]);
    builder.ParseRedirects(argv[2]);
    builder.ParseGeocodes(argv[3]);
    builder.Save(argv[4]);
  }
  else
    cout << "Usage: <info file> <redirect file> <geocodes file> <result file>" << endl;

  return 0;
}
