import subwaystation
class line:

    def __init__(self, lname, color, is_loop):
        self.lname = lname
        self.color = color
        self.station_list_station_name = []
        self.is_loop = is_loop
        self.transfer_station_name = []

    def set_lname(self, lname):
        self.lname = lname

    def set_color(self, color):
        self.color = color

    def add_station(self, sname):
        self.station_list_station_name.append(sname)

    def add_transfer_station(self, sname):
        self.transfer_station_name.append(sname)

