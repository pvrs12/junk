import sys

count = 0
i = 1
for line in sys.stdin.readlines():
    words = set()
    good = True
    for word in line.split():
        if word in words:
            good = False
            break
        else:
            words.add(word)

    if good:
        print("Line {} good".format(i))
        count += 1
    else:
        print("Line {} bad".format(i))
    i += 1
print(count)
