import xml.etree.cElementTree as ET
import subwaystation
import tkinter as tk
import codecs
from tkinter import messagebox
import time
import subprocess
# 全局变量
# 线路树，储存线路是否是环线、车站名字、换乘车站名字、颜色
line_dic = {}
# 站点链表，其中每个结点储存站点的所有信息：包括坐标、名字、所属的路线等
station_list = []
# 换乘车站链表，其中每个结点储存换乘车站的所有信息：包括坐标、名字、所属的路线
transfer_station_list = []
# 车站和UI中的标签对应的链表，其中每个元素组成为（车站，标签）
station_label_list = []


def decode_xml(path):
    tree = ET.parse(path)
    map = tree.getroot()
    # 长宽记录
    w = int(map.find('w').text)
    h = int(map.find('h').text)
    linecnt = 0  # 记录线路个数
    # 遍历路线
    loop_line = ['Line 2', 'Line 4', "Line 10"]
    special_cnt = 0
    for line in map.findall('line'):  # 查找所有线路
        # 标记北京线路的个数
        linecnt += 1
        # 遍历车站
        lname = line.find('lname').text
        s = str(lname)
        s = s.split()  # 分割合并，去除所有空格和换行
        lname = ""
        for t in s:
            lname += t

        color = line.find('color').text
        # print(color)
        line_dic[lname] = {}
        station_cnt = 0
        is_loop = False
        if lname == 'Line2' or lname == 'Line4' or lname == 'Line10':  # 特判环形线路
            is_loop = True
        # line_dic[lname]['is_loop'] = is_loop
        line_dic[lname]['transfer_station_name_list'] = []  #存储换乘站点的信息
        line_dic[lname]['station_name_list'] = []  # 存储该线路的站点信息
        color = str(color)
        color = '#' + color.upper()
        line_dic[lname]['color'] = color

        for station in line:
            # 标记为是否为环线
            if station.tag != 'station':
                continue
            # 具体的结果
            station_cnt += 1
            staid = station.find('staid').text
            name = station.find('name').text
            sname = station.find('sname').text
            sname = str(sname)
            sname = sname.strip().split()
            sname = sname[len(sname) - 1]
            # 等比例缩小坐标
            mapx = float(station.find('mapx').text) / 2
            mapy = float(station.find('mapy').text) / 2

            is_transfer = False
            for tmp_station in station_list:
                if tmp_station.sname == sname:
                    is_transfer = True
                    tmp_station.add_lname(lname)
                    transfer_station_list.append(tmp_station)
                    break
            if not is_transfer:
                tmp_station = subwaystation.station(staid, name, sname, mapx, mapy, lname, station_cnt)
                station_list.append(tmp_station)
            line_dic[lname]['station_name_list'].append(sname)

    # for key in line_dic:
    #     print(key)
    #     print(line_dic[key])

    print()
    for tmp_station in station_list:
        if len(tmp_station.lname_list) > 1:
            transfer_station_list.append(tmp_station)

    print()

    for tmp_station in transfer_station_list:
        tmp_lname_list = tmp_station.lname_list
        sname = tmp_station.sname
        for tmp_lname in tmp_lname_list:
            if sname not in line_dic[tmp_lname]['transfer_station_name_list']:
                line_dic[tmp_lname]['transfer_station_name_list'].append(sname)
            else:
                continue
    return
xml_path = 'stations_v5.xml'
decode_xml(xml_path)

# UI部分，包括UI部分的所有插件
# 缺少Tkinter下MVC的配置资料，故解析和UI有些杂糅
window = tk.Tk()
window.title("地铁路线图")
window.geometry('1920x1080')

# 寻找最短路线的排版
e1 = tk.Entry(window)
e2 = tk.Entry(window)
l1 = tk.Label(window, text="出发地", font=('Aerial', 15))
l1.place(x=1000, y=180)
l2 = tk.Label(window, text="目的地",font=('Aerial', 15))
l2.place(x=1000, y = 210)
e1.place(x=1080,y = 180)
e2.place(x=1080,y = 210)
# 根据题目要求查找从某个起点车站到某个终点车站的路线
v1 = tk.IntVar()
def find_a_way_by_cost():
    # 按站寻路为0
    sign = 0
    init_color()
    if (v1.get()==1):
        cost = "3"
    else:
        cost = "1"
    start_name = str(e1.get())
    end_name = str(e2.get())

    #########检查输入正确性
    flag = False
    for tmp_station in station_list:
        if tmp_station.sname == start_name or tmp_station.sname == end_name:
            flag = True
            break
    if flag == False:
        messagebox.showwarning(title="错误！", message="站点输入有误！")
        return
    ##########

    child = subprocess.Popen(['./Metroplan', 'BEIJING','-c', cost, '-s', start_name, end_name])  # 创建一个子进 程，进程名为child，执行操作ping -c 4 www.baidu.com
    child.wait()  # 子进程等待
    ##########
    # python对文件进行处理
    tmp_sname_list = []
    input_path = 'shortest_path.txt'
    input_file = codecs.open(input_path, 'r')
    i = 0
    output = ''
    tmp_sname_color_dic = {}
    for line in input_file.readlines():
        output += line
        info = line.strip().split()
        tmp_sname_list.append(info[0])

    # 动态显示线路
    show_line(tmp_sname_list)
    # 更改右侧的结果
    T.delete(0.0,tk.END)
    T.insert('insert', output)
button = tk.Button(window,
                   text="查询最短路",
                   width=15,
                   height=2,
                   command=find_a_way_by_cost,
                   activebackground = '#C4C4C4',
                   background="#D5D5D5")
button.place(x=1000, y=250)

C1 = tk.Checkbutton(window, text = "考虑换乘代价",
                font=('Aerial', 15),
                fg="blue",
                relief = "raised",
                variable=v1,
                onvalue = 1,
                offvalue = 0,
                )
C1.place(x=1000,y=110)

# 右侧文本框的排版
T = tk.Text(window,
            height=60,
            width=40)
T.place(x=1370, y=5)
T.insert('insert',"欢迎使用地铁线路查询系统")

# 按照线路名寻找的排版
e3 = tk.Entry(window)
e3.place(x=1000, y=370)
l3 = tk.Label(window, text="输入线路名", font=('Aerial', 15))
l3.place(x=1000, y=330)

# 根据后端返回的信息，按顺序在地图上显示出来，并使用红色表示经历过的，蓝色表示正在经历的
def show_line(tmp_sname_list):
    for tmp_sname in tmp_sname_list:
        last_one = -1
        for (tmp_station, l) in station_label_list:
            if tmp_sname == tmp_station.sname:
                l['bg'] = 'blue'
                # last_one = i
                l.update()
                time.sleep(.200)
                l['bg'] = 'red'
                # ls.update()
                break

def print_line():
    # 颜色复原
    init_color()
    if (v1.get() == 1):
        cost = "3"
    else:
        cost = "1"
    print(cost)
    line_name = e3.get()
    if line_name not in line_dic.keys():
        messagebox.showwarning(title="错误！",
                            message="输入内容应该在以下范围： Line1\Line2\Line4/Daxing\Line5\Line8\Line9\Line10\Line13\Line15\BatongLine\ChangpingLine\YizhuangLine\FangshanLine\AirportExpress")
    ##########
    child = subprocess.Popen(['./Metroplan', 'BEIJING', '-c', cost, '-ln', line_name,
                              ])  # 创建一个子进 程，进程名为child，执行操作ping -c 4 www.baidu.com
    child.wait()  # 子进程等待
    # python对文件进行处理
    tmp_sname_list = []
    input_path = 'line_with_num.txt'
    input_file = codecs.open(input_path, 'r')
    i = 0
    output = ''
    for line in input_file.readlines():
        output += line
        if i == 0:
            i += 1
            continue
        info = line.strip().split()
        tmp_sname_list.append(info[0])
    # 动态显示线路
    show_line(tmp_sname_list)
    # 更改右侧的结果
    T.delete(0.0, tk.END)
    T.insert('insert', output)
    return
button2 = tk.Button(window,
                    text="查询线路站点",
                    background= "#D5D5D5",
                    width=15,
                    height=2,
                    command=print_line,
                    activebackground = '#C4C4C4')
button2.place(x=1000, y=410)

# 按照初始站的名字来遍历整个地铁站
l3 = tk.Label(window, text="输入站点", font=('Aerial', 15))
l3.place(x=1000, y=500)
e4 = tk.Entry(window)
e4.place(x=1090, y=500)
# 寻找一条从当前站点遍历并回到自身的路径
def search_a_way():
    # 按站寻路为0
    # 恢复原来的颜色
    init_color()
    if (v1.get() == 1):
        cost = "3"
    else:
        cost = "1"
    print(cost)
    start_name = str(e4.get())

    flag = False
    for tmp_station in station_list:
        if tmp_station.sname == start_name:
            flag = True
            break
    if flag == False:
        messagebox.showwarning(title="错误！", message="站点名称有误！")
        return

    ##########
    child = subprocess.Popen(['./Metroplan', 'BEIJING', '-c', cost, '-t', start_name,
                              ])
    child.wait()  # 子进程等待
    ##########
    # python对文件进行处理
    tmp_sname_list = []
    input_path = 'travel.txt'
    input_file = codecs.open(input_path, 'r'  )
    # for line in input_file.readlines():
    #     print(line)
    i = 0
    output = ''
    tmp_sname_color_dic = {}
    for line in input_file.readlines():
        output += line
        if i == 0:
            i += 1
            continue
        info = line.strip().split()
        tmp_sname_list.append(info[0])

    # 动态显示线路
    show_line(tmp_sname_list)
    # 更改右侧的结果
    T.delete(0.0, tk.END)
    T.insert('insert', output)
    pass
button4 = tk.Button(window, text="查询遍历站点",
                    background= "#D5D5D5",
                    width=15,
                    height=2,
                    command=search_a_way,
                    activebackground = '#C4C4C4')
button4.place(x=1000, y=550)

def init_color():
    for (station, l) in station_label_list:
        lname_list = station.lname_list
        lname = station.lname_list[0]
        color = 'white'
        l['bg'] = color

# 创建标签，其中每一个标签都是地铁上的一个站
for tmp_station in station_list:
    text = tmp_station.sname
    mapx = tmp_station.mapx
    mapy = tmp_station.mapy
    lname_list = tmp_station.lname_list
    lname = lname_list[0]
    color = "white"
    l = tk.Label(window, text=text, bg= color, font=('Aerial', 8))
    l.place(x=mapx, y=mapy)
    station_label_list.append((tmp_station, l))
# 窗口的循环
window.mainloop()