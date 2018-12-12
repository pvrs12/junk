# from itertools import range

def calc_power(x, y, serial):
    rack = x + 10
    power = rack * y
    power += serial
    power *= rack
    if power < 100:
        power = 0
    else:
        power = int(str(power)[-3])
    power -= 5
    return power

max_v = 0
max_x=0
max_y=0
max_r=0

n=301
grid = [[[0 for k in range(n)] for j in range(n)] for i in range(n)]

#base
for x in range(1,301):
    for y in range(1,301):
        grid[x][y][0] = calc_power(x, y, 5235)

for r in range(1, 301):
    # print(r)
    for x in range(1,300-r+1):
        for y in range(1,300-r+1):
            a = (
                   grid[x + 1][y    ][r - 1]  #top right
                 + grid[x    ][y + 1][r - 1] #top left
                 - grid[x + 1][y + 1][r - 2] #less center
                 + grid[x    ][y    ][0    ] #top left point
                 + grid[x + r - 1][y + r - 1][0]) #bot right point
            grid[x][y][r] = a
            # print(a)
            if a > max_v:
                max_v = a
                max_x = x
                max_y = y
                max_r = r

                print("{},{},{}\t{}".format(max_x,max_y,max_r, a))
