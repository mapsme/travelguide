#include "../../storage/storage_builder.hpp"

#include "../../std/iostream.hpp"


int main(int argc, char const * argv[])
{
  if (argc == 4)
  {
    StorageBuilder builder;
    builder.ParseEntries(argv[1]);
    builder.ParseRedirects(argv[2]);
    builder.Save(argv[3]);
  }
  else
    cout << "Usage: <info file> <redirect file> <result file>" << endl;

  return 0;
}
