import sys
from PyQt5.QtWidgets import QMainWindow, QApplication, QMouseEventTransition
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPainter
import math
import draw


class Point:
    def __init__(self, tx, ty):
        self.x = tx
        self.y = ty
        self.enter = False

    def dist(self, a):
        dx = a.x - self.x
        dy = a.y - self.y
        return math.sqrt(dx * dx + dy * dy)


class Intersection:
    def __init__(self, tx, ty, tenter, tk, ttype, npolygon, nline):
        self.x = tx
        self.y = ty
        self.enter = tenter
        self.k = tk
        self.type = ttype
        self.num_polygon = npolygon
        self.num_line = nline


class WA(QMainWindow):
    def __init__(self):
        super().__init__()
        # 变量
        self.source_points = []  # [[Point1, Point2, ...], [Point1, Point2, ...], ...]
        self.window_points = []
        self.buffer_points = []

        self.mode = 0  # 0 画目标多边形， 1 画裁剪多边形
        self.out_source = -1
        self.out_window = -1

        self.list_source = []  # 目标多边形交点列表
        self.list_window = []  # 裁剪多边形交点列表
        self.visited = []
        self.result = []  # 结果多边形

        # ui 操作
        self.ui = draw.Ui_MainWindow()
        self.ui.setupUi(self)
        self.show()

        self.source_button = self.ui.pushButton_source
        self.source_button.setCheckable(True)
        self.source_button.setChecked(True)
        self.source_button.clicked.connect(self.switchToSource)

        self.scissor_button = self.ui.pushButton_scissor
        self.scissor_button.setCheckable(True)
        self.scissor_button.clicked.connect(self.switchToWindow)

        self.save_button = self.ui.pushButton_save
        self.save_button.clicked.connect(self.weilerAtherton)

        self.clean_button = self.ui.pushButton_clean
        self.clean_button.clicked.connect(self.clean)

    def mousePressEvent(self, event):
        pos = event.pos()
        x = pos.x()
        y = pos.y()
        p = Point(x, y)

        if event.button() == Qt.LeftButton:
            print(event.button(), event.pos())
            self.buffer_points.append(p)
            self.update()

        elif event.button() == Qt.RightButton:
            print(event.button(), event.pos())
            if len(self.buffer_points) >= 3:
                self.closeLink()
                self.update()

    def getIntersection(self, p_start: Point, p_end: Point, q_start: Point, q_end: Point):
        # 求直线方程
        # A = Y2 - Y1
        # B = X1 - X2
        # C = X2 * Y1 - X1 * Y2
        a_p = p_end.y - p_start.y
        b_p = p_start.x - p_end.x
        c_p = p_start.x * p_end.y - p_end.x * p_start.y
        # c_p = a_p * p_start.x + b_p * p_start.y

        a_q = q_end.y - q_start.y
        b_q = q_start.x - q_end.x
        c_q = q_start.x * q_end.y - q_end.x * q_start.y
        # c_q = a_q * q_start.x + b_q * q_start.y

        # 平行
        if a_p * b_q == a_q * b_p:
            return Point(-1, -1)

        tmp = a_p * b_q - a_q * b_p
        x = (b_q * c_p - b_p * c_q) / tmp
        y = (a_p * c_q - a_q * c_p) / tmp

        if x > max(p_end.x, p_start.x) or x < min(p_end.x, p_start.x) or x > max(q_end.x, q_start.x) or x < min(q_end.x,
                                                                                                                q_start.x):
            return Point(-1, -1)
        if y > max(p_end.y, p_start.y) or y < min(p_end.y, p_start.y) or y > max(q_end.y, q_start.y) or y < min(q_end.y,
                                                                                                                q_start.y):
            return Point(-1, -1)

        p = Point(x, y)
        if tmp > 0:
            p.enter = 1

        return p

    def getK(self, start: Point, end: Point, middle: Point):
        return start.dist(middle) / start.dist(end)

    def getIntersections(self, start, end, n_polygon, n_line, window):
        for i in range(len(window)):
            window_list = window[i]
            n = len(window_list)
            for j in range(n):
                start_1 = window_list[j]
                end_1 = window_list[(j + 1) % n]
                intersection = self.getIntersection(start, end, start_1, end_1)
                if intersection.x >= 0 and intersection.y >= 0:
                    k1 = self.getK(start, end, intersection)
                    k2 = self.getK(start_1, end_1, intersection)
                    inter_1 = Intersection(intersection.x, intersection.y, intersection.enter, k1, 0, i, j)
                    inter_2 = Intersection(intersection.x, intersection.y, intersection.enter, k2, 1, n_polygon, n_line)
                    self.list_source[n_polygon][n_line].append(inter_1)
                    self.list_window[i][j].append(inter_2)

    def sortIntersections(self, t_list: [Intersection]):
        if len(t_list) <= 1:
            return
        for i in range(len(t_list) - 1):
            for j in range(i + 1, len(t_list)):
                if t_list[i].k > t_list[j].k:
                    tmp = t_list[i]
                    t_list[i] = t_list[j]
                    t_list[j] = tmp
                    # t_list[i], t_list[j] = t_list[j], t_list[i]

    def findRelatedPlace(self, type, p: Intersection):
        nline = p.num_line
        npolygon = p.num_polygon
        if type == 0:
            for i in range(len(self.list_window[npolygon][nline])):
                if self.list_window[npolygon][nline][i].x == p.x and self.list_window[npolygon][nline][i].y == p.y:
                    npoint = i
                    return npolygon, nline, npoint
        else:
            for i in range(len(self.list_source[npolygon][nline])):
                if self.list_source[npolygon][nline][i].x == p.x and self.list_source[npolygon][nline][i].y == p.y:
                    npoint = i
                    return npolygon, nline, npoint

    def findNextPlace(self, pmode, cpolygon, cline, cpoint, cmode, current):
        if cpolygon == -1 or cline == -1 or cpoint == -1:
            for i in range(len(self.visited)):
                for j in range(len(self.visited[i])):
                    for k in range(len(self.visited[i][j])):
                        if self.visited[i][j][k] == 0:
                            npolygon = i
                            nline = j
                            npoint = k
                            pmode = 0
                            p = self.list_source[i][j][k]
                            if not p.enter:
                                pmode = 1
                                npolygon, nline, npoint = self.findRelatedPlace(0, p)
                                return npolygon, nline, npoint, pmode
                            return npolygon, nline, npoint, pmode

            npolygon = nline = npoint = -1
            return npolygon, nline, npoint, pmode
        else:
            if cmode == 0:
                n = len(self.list_source[cpolygon][cline])
                while cpoint >= n - 1:
                    cline += 1
                    if cline >= len(self.source_points[cpolygon]):
                        cline = 0
                    cpoint = -1
                    n = len(self.list_source[cpolygon][cline])

                    self.result[current].append(self.source_points[cpolygon][cline])
                cpoint += 1
                p = self.list_source[cpolygon][cline][cpoint]
                if not p.enter:
                    pmode = 1
                    npolygon, nline, npoint = self.findRelatedPlace(0, p)
                    return npolygon, nline, npoint, pmode
                else:
                    pmode = 0
                    npolygon = cpolygon
                    nline = cline
                    npoint = cpoint
                    return npolygon, nline, npoint, pmode
            else:
                n = len(self.list_window[cpolygon][cline])
                while cpoint >= n - 1:
                    cline += 1
                    if cline >= len(self.window_points[cpolygon]):
                        cline = 0
                    cpoint = -1
                    n = len(self.list_window[cpolygon][cline])

                    self.result[current].append(self.window_points[cpolygon][cline])
                cpoint += 1
                p = self.list_window[cpolygon][cline][cpoint]
                if p.enter:
                    pmode = 0
                    npolygon, nline, npoint = self.findRelatedPlace(1, p)
                    return npolygon, nline, npoint, pmode
                else:
                    pmode = 1
                    npolygon = cpolygon
                    nline = cline
                    npoint = cpoint
                    return npolygon, nline, npoint, pmode

    def closeLink(self):
        if self.mode == 0:
            out = -1  # 是否是外环
            for i in range(len(self.source_points)):
                if self.isInside(self.buffer_points[0], self.source_points[i]):
                    out = i
                    break
            self.source_points.append(self.buffer_points.copy())
            if out == -1:
                out = len(self.source_points) - 1
            self.out_source = out
            # 遍历各环
            for i in range(len(self.source_points)):
                if i == out:
                    if self.isClockwise(self.source_points[i]):
                        self.source_points[i].reverse()
                else:
                    if not self.isClockwise(self.source_points[i]):
                        self.source_points[i].reverse()
        else:
            out = -1
            for i in range(len(self.window_points)):
                if self.isInside(self.buffer_points[0], self.window_points[i]):
                    out = i
                    break
            self.window_points.append(self.buffer_points.copy())
            if out == -1:
                out = len(self.window_points) - 1
            self.out_window = out
            for i in range(len(self.window_points)):
                if i == out:
                    if self.isClockwise(self.window_points[i]):
                        self.window_points[i].reverse()
                else:
                    if not self.isClockwise(self.window_points[i]):
                        self.window_points[i].reverse()
        self.buffer_points.clear()

    def isClockwise(self, polygon: [Point]):
        n = len(polygon)
        sum = 0
        for i in range(n):
            sum += (polygon[(i + 1) % n].x - polygon[i].x) * (polygon[(i + 1) % n].y + polygon[i].y)
        return sum < 0

    def isInside(self, p: Point, polygon: [Point]):
        sum = 0
        # 遍历外环多边形
        n = len(polygon)
        for i in range(n):
            q = polygon[(i + 1) % n]
            r = polygon[i]
            a = p.dist(q)
            b = p.dist(r)
            c = q.dist(r)
            sum += math.acos((a * a + b * b - c * c) / (2 * a * b))

        if sum - 2 * 3.1415926535 < -0.01:
            return False
        return True

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)

        painter.setPen(Qt.red)
        for polygon in self.source_points:
            current = polygon[0]
            for point in polygon[1:]:
                painter.drawLine(current.x, current.y, point.x, point.y)
                current = point
            painter.drawLine(current.x, current.y, polygon[0].x, polygon[0].y)

        painter.setPen(Qt.blue)
        for polygon in self.window_points:
            current = polygon[0]
            for point in polygon[1:]:
                painter.drawLine(current.x, current.y, point.x, point.y)
                current = point
            painter.drawLine(current.x, current.y, polygon[0].x, polygon[0].y)

        painter.setPen(Qt.black)
        for polygon in self.result:
            if len(polygon) > 2:
                current = polygon[0]
                for point in polygon[1:]:
                    painter.drawLine(current.x, current.y, point.x, point.y)
                    current = point
                painter.drawLine(current.x, current.y, polygon[0].x, polygon[0].y)

    def checkSpecial(self):
        # 没交点, 只有嵌套
        # 判断source
        for i in range(len(self.source_points)):
            if not self.hasIntersection(0, i) and self.isEmbedded(0, i):
                self.result.append([])
                for point in self.source_points[i]:
                    self.result[len(self.result) - 1].append(point)

        # 判断window
        for i in range(len(self.window_points)):
            if not self.hasIntersection(1, i) and self.isEmbedded(1, i):
                self.result.append([])
                for point in self.window_points[i]:
                    self.result[len(self.result) - 1].append(point)

    def hasIntersection(self, mode, num):
        if mode == 0:
            for i in range(len(self.list_source[num])):
                if len(self.list_source[num][i]):
                    return True
        else:
            for i in range(len(self.list_window[num])):
                if len(self.list_window[num][i]):
                    return True
        return False

    def isEmbedded(self, mode, num):
        if mode == 0:
            # 遍历每个窗口多边形
            for i in range(len(self.window_points)):
                for point in self.source_points[num]:
                    if i != self.out_window:
                        if self.isInside(point, self.window_points[i]):
                            return False
                    else:
                        if not self.isInside(point, self.window_points[i]):
                            return False
        else:
            for i in range(len(self.source_points)):
                for point in self.window_points[num]:
                    if i != self.out_source:
                        if self.isInside(point, self.source_points[i]):
                            return False
                    else:
                        if not self.isInside(point, self.source_points[i]):
                            return False
        return True

    def switchToSource(self):
        self.mode = 0
        self.buffer_points.clear()
        self.source_button.setChecked(True)
        self.scissor_button.setChecked(False)

    def switchToWindow(self):
        self.mode = 1
        self.buffer_points.clear()
        self.scissor_button.setChecked(True)
        self.source_button.setChecked(False)

    def weilerAtherton(self):
        # 初始化链表
        self.list_source.clear()
        self.list_window.clear()
        self.visited.clear()
        self.result.clear()

        # [ [ [], [], ...], [], ... ]
        self.list_source = [[[] for j in range(len(self.source_points[i]))] for i in range(len(self.source_points))]
        self.list_window = [[[] for j in range(len(self.window_points[i]))] for i in range(len(self.window_points))]
        self.visited = [[[] for j in range(len(self.source_points[i]))] for i in range(len(self.source_points))]

        for i in range(len(self.source_points)):
            source_list = self.source_points[i]  # source_list每个元素是一个点
            window_lists = self.window_points.copy()

            n = len(source_list)
            for j in range(n):
                self.getIntersections(source_list[j], source_list[(j + 1) % n], i, j, window_lists)

        for i in range(len(self.list_source)):
            for j in range(len(self.list_source[i])):
                self.sortIntersections(self.list_source[i][j])
                for k in range(len(self.list_source[i][j])):
                    self.visited[i][j].append(0)

        for i in range(len(self.list_window)):
            for j in range(len(self.list_window[i])):
                self.sortIntersections(self.list_window[i][j])

        # WA算法
        self.result.append([])
        current = 0
        p_polygon = p_line = p_point = -1
        p_mode = 0

        while True:
            # print("loop")
            n_polygon = -1
            n_line = -1
            n_point = -1
            n_mode = 1
            v_polygon = -1
            v_line = -1
            v_point = -1
            n_polygon, n_line, n_point, n_mode = self.findNextPlace(n_mode, p_polygon, p_line, p_point, p_mode, current)
            if n_polygon == -1 or n_line == -1 or n_point == -1:
                break
            if n_mode == 1:
                v_polygon, v_line, v_point = self.findRelatedPlace(1, self.list_window[n_polygon][n_line][n_point])
            else:
                v_polygon = n_polygon
                v_line = n_line
                v_point = n_point
            if self.visited[v_polygon][v_line][v_point] == 1:
                self.result.append([])
                current += 1
                p_polygon = -1
                p_line = -1
                p_point = -1
                p_mode = 0
            else:
                new = Point(self.list_source[v_polygon][v_line][v_point].x,
                            self.list_source[v_polygon][v_line][v_point].y)
                self.result[current].append(new)
                self.visited[v_polygon][v_line][v_point] = 1
                p_polygon = n_polygon
                p_line = n_line
                p_point = n_point
                p_mode = n_mode
        self.checkSpecial()

        # ui 操作
        self.update()
        self.source_button.setEnabled(False)
        self.scissor_button.setEnabled(False)
        self.source_button.setChecked(False)
        self.scissor_button.setChecked(False)

    def clean(self):
        self.source_points.clear()
        self.window_points.clear()
        self.buffer_points.clear()
        self.mode = 0
        self.out_source = -1
        self.out_window = -1
        self.list_source.clear()
        self.list_window.clear()
        self.visited.clear()
        self.result.clear()
        self.update()
        self.source_button.setChecked(True)
        self.scissor_button.setChecked(False)
        self.source_button.setEnabled(True)
        self.scissor_button.setEnabled(True)
