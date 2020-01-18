class station:
    def __init__(self, staid, name, sname, mapx, mapy, pos, lat, lng, lname, station_cnt):
        self.staid = staid
        self.name = name
        self.sname = sname
        self.mapx = mapx
        self.mapy = mapy
        self.pos = pos
        self.lat = lat
        self.lng = lng

        # 不知道是否是transfer_station
        self.is_transfer = False
        self.next = []
        self.previous = []
        self.lname_list = []
        self.lname_list.append(lname)
        self.station_cnt = station_cnt

    def set_transfer(self, is_transfer):
        self.is_transfer = is_transfer

    def add_lname(self, lname):
        self.lname_list.append(lname)