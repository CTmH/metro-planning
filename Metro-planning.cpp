#include "metro.h"

int main(int argc,char *argv[])
{
  if(argc < 6 || argc > 7)
    {
      cout << "Wrong arg!" << endl;
      return 0;
    }
  string arg[7];
  for(int i = 0; i < argc; i++)
    arg[i] = argv[i];
  if(arg[2] != "-c")
    {
      cout << "Please set cost!"<< endl;
      return 0;
    }
  int cost = atoi(argv[3]);
  string op = arg[4];
  cout << "loading..." << endl;
  SearchSys ss(arg[1], cost);
  Path ans;
  if(op == "-s")
    {
      if(argc == 7)
        {
          try
            {
              ans = ss.Find_the_shrt_path(argv[5],argv[6]);
            }
          catch(char const* err)
            {
              cout << err << endl;
              return 0;
            }
          ss.print_path(ans);
          ss.save_path(ans, "shortest_path.txt");
        }
      else return 0;
    }
  else if(op == "-t")
    {
      try
        {
          ans = ss.Trave_metro(argv[5]);
        }
      catch(char const* err)
        {
          cout << err << endl;
          return 0;
        }
      ss.print_path(ans);
      ss.save_path(ans, "travel.txt");
    }
  else if(op == "-l")
    {
      Path *pllist;
      int list_num = 0;
      try
        {
          list_num = ss.Find_line(argv[5], pllist);
        }
      catch(char const* err)
        {
          cout << err << endl;
          return 0;
        }
      ss.save_path(pllist, "line.txt", list_num);
      delete[] pllist;
    }
  else if(op == "-ln")
    {
      try
        {
          ans = ss.Find_line_with_name(argv[5]);
        }
      catch(char const* err)
        {
          cout << err << endl;
          return 0;
        }
      ss.print_path(ans);
      ss.save_path(ans, "line_with_num.txt");
    }
  cout << "Path has been saved" <<endl;
  return 0;
}
