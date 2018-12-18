woods = []

def count_adjacent(x, y, c):
    count = 0
    if x > 0:
        if woods[x-1][y] == c:
            count += 1
        if y > 0:
            if woods[x-1][y-1] == c:
                count += 1
        if y < len(woods[x])-1:
            if woods[x-1][y+1] == c:
                count += 1
    if y > 0:
        if woods[x][y-1] == c:
            count += 1
    if x < len(woods)-1:
        if woods[x+1][y] == c:
            count += 1
        if y > 0:
            if woods[x+1][y-1] == c:
                count += 1
        if y < len(woods[x])-1:
            if woods[x+1][y+1] == c:
                count += 1
    if y < len(woods[x])-1:
        if woods[x][y+1] == c:
            count += 1
    return count

def sum_woods():
    trees = 0
    mills = 0
    for copse in woods:
        for acre in copse:
            if acre == '|':
                trees += 1
            if acre == '#':
                mills += 1
    return trees * mills

def print_woods():
    for copse in woods:
        print('.'.join(copse))

past_sums = []
with open("input.txt") as f:
    for line in f:
        woods.append([x for x in line.strip()])

    for min in range(1000000000 ):
        new_woods = []
        for x in range(len(woods)):
            new_copse = []
            for y in range(len(woods[x])):
                acre = woods[x][y]
                if acre == '.':
                    if count_adjacent(x, y, '|') >= 3:
                        #next pass make tree
                        new_copse.append('|')
                    else:
                        new_copse.append(acre)
                if acre == '|':
                    if count_adjacent(x, y, '#') >= 3:
                        #next pass make lumberyard
                        new_copse.append('#')
                    else:
                        new_copse.append(acre)
                if acre == '#':
                    if not (count_adjacent(x, y, '#') >= 1 and
                        count_adjacent(x, y, '|') >=1):
                        #next pass make open
                        new_copse.append('.')
                    else:
                        new_copse.append(acre)
            new_woods.append(new_copse)
        woods = new_woods
        s = sum_woods()
        if (s in past_sums):
            past_sums .append(sum_woods())
            print(min)
            print(past_sums)
            #part 2
            ### Ctrl+C after duplicate sums happen a lot
            ### copy the minute and the past_sums array
            ### using the last value in the past_sums, find the start of the loop
            ### the difference in position is the modulo of the cylcle
            ### subtract the minute from 1000000000 and subtract the size of the loop (less 1) then modulo the loop size
            
        else:
            past_sums .append(sum_woods())
        #print(sum_woods())
        #print_woods()