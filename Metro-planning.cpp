#include "metro.h"

int main(int argc,char *argv[])
{
    string op;
    int cost;
    Path ans;
    //无输入参数 命令行式交互
    if(argc == 1)
    {
        cout << "Input city name:" << endl;
        string city_name;
        cin >> city_name;
        cout << "Input transfer cost:" << endl;
        cin >> cost;
        cout << "Waiting..." << endl;
        SearchSys metro(city_name, cost);
        string src,trg;
        op = "";
        while(op != "q")
        {
            cout << "Input your command:" << endl << "f: Find shortest path" << endl
                 << "t: Travel around Metro" << endl << "s: Station in line" << endl
                 << "q: Quit" << endl;
            cin >> op;
            if(op == "f") //查询最短路径
            {
                cout << "From which station:" << endl;
                cin >> src;
                cout << "To witch station:" << endl;
                cin >> trg;
                try
                {
                    ans = metro.Find_the_shrt_path(src,trg);
                }
                catch(char const* err)
                {
                    cout << err << endl;
                    continue;
                }
                metro.print_path(ans);
            }
            else if(op == "t") //遍历站点
            {
                cout << "From which station:" << endl;
                cin >> src;
                try
                {
                    ans = metro.Trave_metro(src);
                }
                catch(char const* err)
                {
                    cout << err << endl;
                    continue;
                }
                metro.print_path(ans);
            }
            else if(op == "s") //查找指定线路
            {
                cout << "Line name:" << endl;
                cin >> src;
                try
                {
                    ans = metro.Find_line_with_name(src);
                }
                catch(char const* err)
                {
                    cout << err << endl;
                    continue;
                }
                metro.print_path(ans);
            }
            else
            {
                cout<<"op Error!"<<endl;
            }
        }
        return 0;
    }
    //有输入参数 调用查询 结果写入文件
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
    cost = atoi(argv[3]);
    op = arg[4];
    cout << "loading..." << endl;
    SearchSys ss(arg[1], cost);
    if(op == "-s") //查询最短路
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
    else if(op == "-t") //遍历
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
    else if(op == "-l") //根据站点名查线路信息
    {
        Path *pllist = NULL;
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
    else if(op == "-ln") //根据线路名查线路站点
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
    else
    {
        cout<<"op Error!"<<endl;
    }
    cout << "Path has been saved" <<endl;
    return 0;
}
