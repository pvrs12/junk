import fileinput

def get_closest(x, y, points):
    closest = 0
    min_dist = 10000
    i = 0
    for point in points:
        #manhattan distance
        dist = abs(point[0] - x) + abs(point[1] - y)
        if dist == min_dist:
            #equally close
            closest = -1
            break
        if dist < min_dist:
            closest = i
            min_dist = dist
        i += 1
    return closest, min_dist
            
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

infpoints = []
for y in range(miny, maxy):
    for x in range(minx, maxx):
        closest, min_dist = get_closest(x, y, points)
        # print(closest, min_dist)
        if x == minx or x == maxx or y == miny or y == maxy:
            if closest not in infpoints:
                infpoints.append(closest)
            points[closest][2] = 0
        if closest != -1 and closest not in infpoints:
            points[closest][2] += 1
        if points[closest][0] == x and points[closest][1] == y:
            print('!', end='')
        else:
            print(closest if closest != -1 else '.', end='')
    print()

print(infpoints)
for point in points:
    print(point)
