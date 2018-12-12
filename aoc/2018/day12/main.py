import fileinput

first = True
current = [".","."]
rules = {}
for line in fileinput.input():
    if first:
        first = False
        current.extend(line.strip())
        current.extend("..")
        continue
    lines = line.strip().split("=>")
    rules[lines[0].strip()] = lines[1].strip()

prepends = 2

for generation in range(50000000000):
    new_current = []
    for i in range(len(current)):
        if i == 0:
            rule = ".." + ''.join(current[i:i+3])
        if i == 1:
            rule = "." + ''.join(current[i-1:i+3])
        if i > 1 and i < len(current) - 2:
            rule = ''.join(current[i-2:i+3])
        if i == len(current) - 2:
            rule = ''.join(current[i-2:i+2]) + "."
        if i == len(current) - 1:
            rule = ''.join(current[i-2:i+1]) + ".."

        if rule not in rules:
            # print("{}: {} => {}".format(i, rule, current[i]))
            new_current.append(current[i])
        else:
            new_current.append(rules[rule])
            # print("{}: {} => {}".format(i, rule, rules[rule]))

    if new_current[0] != '.':
        prepends += 2
        new_current = [".","."].extend(new_current)
    elif new_current[1] != '.':
        prepends += 1
        a = ["."]
        a.extend(new_current)
        new_current = a

    if new_current[-1] != '.':
        new_current.extend("..")
    elif new_current[-2] != '.':
        new_current.extend(".")

    #prune the ends
    new_start = 0
    new_end = len(new_current)
    for i in range(len(new_current)):
        if new_current[i:i+5] != ['.','.','.','.','.']:
            new_start = i
            break;
    for i in range(len(new_current)-1, 0, -1):
        if new_current[i-5:i] !=  ['.','.','.','.','.']:
            new_end = i
            break;
    # print(new_current)
    # print("pruning {} and {}".format(new_current[:new_start], new_current[new_end:]))
    # print("prepends = {} => {}".format(prepends, prepends - new_start))
    # prepends -= new_start
    pruned_new_current = new_current[new_start:new_end]

    start = 0
    end = len(current)
    for i in range(len(current)):
        if current[i:i+5] != ['.','.','.','.','.']:
            start = i
            break;
    for i in range(len(current)-1, 0, -1):
        if current[i-5:i] !=  ['.','.','.','.','.']:
            end = i
            break;
    # print(current)
    # print("pruning {} and {}".format(current[:start], current[end:]))
    # print("prepends = {} => {}".format(prepends, prepends - start))
    pruned_current = current[start:end]

    # print(new_current)
    if pruned_current == pruned_new_current:
        #if 2 are the same, then stop
        current = new_current
        print("loop found on gen {}".format(generation))
        break
    current = new_current

    # print(generation)

#need to do math to get the right sum
#it grows by 1 each generation
#remove current generation from math because it is a duplicate of the last one
diff = 50000000000 - generation - 1
#subtract the existing prepends from the new number
new_prepends = diff - prepends

s = 0
for i in range(len(current)):
    if current[i] == '#':
        s += i + new_prepends
print(s)
