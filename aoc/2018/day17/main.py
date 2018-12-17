import sys

starty = 0
endy = 1800
startx = 0
endx = 650
field = [[y for y in range(starty, endy)] for x in range(startx, endx)]

#list of flowing waters
waters = [

]

def print_field(field, maxy):
    for y in range(starty, maxy):
        for x in range(startx, endx):
            print(field[x][y], end='')
        print()

def water_sum(field, maxy):
    sum = 0
    for y in range(starty, maxy):
        for x in range(startx, endx):
            if field[x][y] in ('|', '~', '+'):
                sum += 1
    return sum

with open("input.txt") as f:
    maxy = 0
    for x in range (startx, endx):
        for y in range(starty, endy):
            field[x][y] = '.'
    field[500][0] = '+'
    for line in f:
        lines = line.split(",")
        if 'x' in lines[0]:
            x = int(lines[0].split('=')[1])
            back = lines[1].split('=')[1]
            start= int(back.split('..')[0])
            end = int(back.split('..')[1])
            if end > maxy:
                maxx = end
            for y in (range(start, end+1)):
                field[x][y] = '#'
        else:
            y = int(lines[0].split('=')[1])
            if y > maxy:
                maxy = y
            back = lines[1].split('=')[1]
            start= int(back.split('..')[0])
            end = int(back.split('..')[1])
            for x in (range(start, end+1)):
                field[x][y] = '#'

step = 0
waters.append((500, 0))
stopped = False

water_sums = [1, 2, 3]

while not stopped:
    step += 1

    print(step)
    removed = 0
    for water in waters:
        nextx = water[0]
        nexty = water[1] + 1
        if nexty > maxy:
            print("removing ", (water[0], water[1]))
            waters.remove((water[0], water[1]))
            removed += 1
            continue
        if field[nextx][nexty] == '.':
            #water can spread to sand
            field[nextx][nexty] = '|'
            waters.append((nextx,nexty))

        #water hit a wall or still water
        elif field[nextx][nexty] in ('#', '~'):
            #ignore other flowing water
            #water can't flow
            #convert this and ones to the left/right to still water
            waters.remove(water)
            nexty-=1
            field[nextx][nexty] = '~'

            sides = 0
            stop_left = False
            stop_right = False

            flipped_left = []
            flipped_right = []

            while True:
                sides += 1
                #check if can spread left or right simultaneously
                #if either flows down, stop both
                nextx_left = nextx - sides
                nextx_right = nextx + sides

                if stop_left and stop_right:
                    break
                if not stop_left:
                    if field[nextx_left][nexty] in ('.', '|'):
                        if field[nextx_left][nexty] == '|':
                            if (nextx_left, nexty) in waters:
                                waters.remove((nextx_left, nexty))
                            else:
                                print("error?", (nextx_left, nexty))
                        if field[nextx_left][nexty+1] == '.':
                            #need to make this flowing because there is empty space beneath it
                            field[nextx_left][nexty] = '|'
                            waters.append((nextx_left, nexty))
                            #also leave this loop because flowing water won't continue to spread
                            #if it starts to spread, then turn this whole set into '|'
                            field[nextx][nexty] = '|'
                            #waters.append((nextx, nexty))
                            for f in flipped_left:
                                field[f][nexty] = '|'
                                #waters.append((f, nexty))
                            for f in flipped_right:
                                field[f][nexty] = '|'
                                #waters.append((f, nexty))
                            stop_left = True
                        else:
                            field[nextx_left][nexty] = '~'
                            flipped_left.append(nextx_left)
                    #unless there is another step down available
                    elif field[nextx_left][nexty] in('#' , '~'):
                        stop_left = True

                if not stop_right:
                    if field[nextx_right][nexty] in ('.', '|'):
                        if field[nextx_right][nexty] == '|':
                            if (nextx_right, nexty) in waters:
                                waters.remove((nextx_right, nexty))
                            else:
                                print("error?", (nextx_left, nexty))
                        if field[nextx_right][nexty+1] == '.':
                            #need to make this flowing because there is empty space beneath it
                            field[nextx_right][nexty] = '|'
                            waters.append((nextx_right, nexty))
                            #also leave this loop because flowing water won't continue to spread
                            field[nextx][nexty] = '|'
                            #waters.append((nextx, nexty))
                            for f in flipped_left:
                                field[f][nexty] = '|'
                                #waters.append((f, nexty))
                            for f in flipped_right:
                                field[f][nexty] = '|'
                                #waters.append((f, nexty))
                            stop_right = True
                        else:
                            field[nextx_right][nexty] = '~'
                            flipped_right.append(nextx_right)
                    #unless there is another step down available
                    elif field[nextx_right][nexty] in('#' , '~'):
                        stop_right = True

    #print_field(field, maxy + 1)
    stopped = True
    sum = water_sum(field, maxy) + removed
    for s in water_sums[-3:]:
        if s != sum:
            stopped = False
            break
    water_sums.append(sum)


print("Done!")
#max is because i'm removing some which shouldn't be.
#it's lazy
print(max(water_sums))