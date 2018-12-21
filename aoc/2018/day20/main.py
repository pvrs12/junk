import sys
sys.setrecursionlimit(10000)

minx = 1000000
maxx = 0
miny = 1000000
maxy = 0

def dist(xa, ya, xb, yb):
    return abs(xa - xb) + abs(ya - yb)

###recursion is the right idea,
### but i misunderstood what longest distance
def process(s, start, start_list, posx, posy, maxest=0, depth=0):
    if(start == -1):
        start = 0
    if start in start_list:
        print("repeat")
        return
    start_list.append(start)
    # if depth % 100 == 0:
    #     print(depth, posx, posy)
    #     print(start, end)
    #     print(s[start:start+10])

    new_poss = []
    for i in range(start, len(s)):
        if s[i] == '^':
            continue
        elif s[i]  == '(':
            #go deeper
            while True:
                r = process(s, i + 1, start_list, posx, posy, maxest, depth+1)
                i = r[3] + 1
                new_poss.append((posx, posy))
                if not r[0]:
                    break
            #pick the furthest distance away
            max_dist = 0
            for pos in new_poss:
                d = dist(pos[0], pos[1], posx, posy)
                if d > max_dist:
                    max_dist = d
                    posx = pos[0]
                    posy = pos[1]
        if s[i]  == ')':
            #go up
            ###### go back down, x, y, skip
            return False, posx, posy, i, "paren"
        if s[i]  == '|':
            #go up then come back
            return True, posx, posy, i, "pipe"
        if s[i]  == '$':
            #end
            print(depth)
            print(maxest)
            return False, "done", "done", maxest
        #add a direction
        if s[i]  == 'N':
            posy -= 1
        if s[i]  == 'E':
            posx += 1
        if s[i] == 'S':
            posy += 1
        if s[i] == 'W':
            posx -= 1

        d = dist(0,0, posx, posy)
        if d > maxest:
            maxest = d
    return d

with open("input.txt") as f:
    for line in f:
        print(process(line.strip(), -1, [], 0, 0))