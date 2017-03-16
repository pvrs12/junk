"""Generate a simple IFS image"""

import numpy
from operator import itemgetter

RES = 10000
WIDTH = 1000
HEIGHT = 1000

class equation:
    def equation(self):
        self.a = 0
        self.b = 0
        self.c = 0
        self.d = 0
        self.e = 0
        self.g = 0

    def apply_x(self, x, y):
        return self.a * x + self.b * y + self.e
    def apply_y(self, x, y):
        return self.c * y + self.d * y + self.f

def scale(x, in_min, in_max, out_min, out_max):
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

def write_pbm(image):
    print("P3")
    print("{} {}".format(len(image), len(image[0])))
    print("255")
    for image_row in image:
        s = ""
        for c in image_row:
            s += "{} {} {}\t".format(c[0],c[1],c[2])
        print(s)

def main():
    equations = []
    weights = []
    points = []
    while True:
        line = input().strip()
        lineS = line.split()

        e = equation()
        e.a = float(lineS[0])
        e.b = float(lineS[1])
        e.c = float(lineS[2])
        e.d = float(lineS[3])
        e.e = float(lineS[4])
        e.f = float(lineS[5])
        p = float(lineS[6])

        if p == 0:
            break

        equations.append(e)
        weights.append(p)

    x = 1.0
    y = 1.0

    min_x = 100
    max_x = -100
    min_y = 100
    max_y = -100
    for i in range(RES):
        e = numpy.random.choice(equations, p = weights)
        min_x = min(x, min_x)
        max_x = max(x, max_x)
        min_y = min(y, min_y)
        max_y = max(y, max_y)

        #points.append((int(x*100)/100,int(y*100)/100,))
        points.append((x,y,))
        tx = x
        x = float(e.apply_x(x, y))
        y = float(e.apply_y(tx, y))

    # ps = sorted(points, key=itemgetter(1,0))
    # curX = ps[0][0]
    # s = ""
    # for p in ps:
        # if(p[1] != curX):
            # print(s)
            # s=""
            # curX = p[1]
        # s+="({},{})".format(p[0],p[1])
    
    image = [[(255, 255, 255,)] * (WIDTH+1)] * (HEIGHT+1)
    for point in points:
        image_x = int(scale(point[0] - min_x, 0, max_x - min_x, 0, WIDTH))
        image_y = int(scale(point[1] - min_y, 0, max_y - min_y, 0, HEIGHT))
        #print("({},{})\t({},{})".format(point[0],point[1],image_x,image_y))

        image[image_x][image_y] = (0,0,0,)
    write_pbm(image)

if __name__ == '__main__':
    main()
