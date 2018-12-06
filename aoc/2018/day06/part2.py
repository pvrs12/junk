import sys
import fileinput

def get_closest(x, y, points):
    closest = []
    min_dist = 10000
    i = 0
    for point in points:
        #manhattan distance
        dist = abs(point[0] - x) + abs(point[1] - y)
        closest.append(dist)
    return sum(closest) < 10000
            
points = []
minx = 100
miny=100
maxx = 0
maxy=0
for line in fileinput.input():
    x, y = line.strip().split(",")
    x = int(x)
    y = int(y)
    points.append([x,y,0])
    if x < minx:
        minx = x
    if y < miny:
        miny = y
    if x > maxx:
        maxx = x
    if y > maxy:
        maxy = y

minx -=1
maxx +=2
miny -=1
maxy +=2

area = 0
for y in range(miny, maxy + 1):
    for x in range(minx, maxx + 1):
        if get_closest(x,y,points):
            area += 1

# print(infpoints)
# for point in points:
    # print(point[2])
print(area)
