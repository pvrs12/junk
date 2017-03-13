"""Generate a simple IFS image"""

import numpy

RES = 10000
WIDTH = 1000
HEIGHT = 1000

class equation:
    equation(self):
        self.a = 0
        self.b = 0
        self.c = 0
        self.d = 0
        self.e = 0
        self.g = 0

    apply_e(self, x, y):
        return (a * x + b * y + e, c * y + d * y + f, )

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

        equations.append(e)
        weights.append(p)

    x = 1.0
    y = 1.0
    for i in range(RES):
        e = numpy.random.choice(equations, p = weights)
        points.append((x,y,))
        x, y = e.apply_y(x, y)

    image = [[None] * WIDTH] * HEIGHT

if __name__ == '__main__':
    main()
