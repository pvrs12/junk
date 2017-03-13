"""Generate a simple IFS image"""

import numpy

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

    def apply_e(self, x, y):
        return (a * x + b * y + e, c * y + d * y + f, )

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
        e = equation()
        e.a = float(input())
        e.b = float(input())
        e.c = float(input())
        e.d = float(input())
        e.e = float(input())
        e.f = float(input())
        p = float(input())

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

        points.append((x,y,))
        x, y = e.apply_y(x, y)

    image = [[(255, 255, 255,)] * WIDTH] * HEIGHT
    for point in points:
        image_x = int(scale(point[0] - min_x, 0, max_x - min_x, 0, WIDTH))
        image_y = int(scale(point[1] - min_y, 0, max_y - min_y, 0, HEIGHT))

        image[image_x][image_y] = (0,0,0,)
    write_pbm(image)

if __name__ == '__main__':
    main()
