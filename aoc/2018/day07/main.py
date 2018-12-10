import fileinput

depends = {
}

for line in fileinput.input():
    line = line.strip()
    first = line[5]
    second = line[36]
    if second not in depends:
        depends[second] = set()
    if first not in depends:
        depends[first] = set()
    depends[second].add(first)

# graph = {
# }
done = set()
time = 0
workers = [
    0,0,0,0,0
]
seconds = 0
avail = 0
while True:
    #find a letter which has no depends
    #print it
    #remove it
    #repeast
    for k in sorted(depends):
        if avail >= 5:
            break
        if k in done:
            continue
        depend = False
        for j in sorted(depends):
            if k==j:
                continue
            if j in depends[k] and j not in done:
                #k depnds on j to complete
                #draw a one way edge from j to k
                # if j not in graph:
                    # graph[j] = []
                # print("dep: {} - {}".format(j,k))
                depend = True
                break
            # else:
                # print("no dep: {} - {}".format(j, k))
        if not depend:
            print("cur worker = {}".format(avail))
            time = ord(k) - ord('A') + 1
            print("{} - {}".format(k, time))
            workers[avail] += time
            avail += 1
            done.add(k)
            break

    for i in range(len(workers)):
        if workers[i] > 0:
            workers[i] -= 1
        if workers[i] == 0 and i < avail:
            avail = i
    seconds += 1

    if len(done) == len(depends):
        break

remaining = 0
for worker in workers:
    if worker > remaining:
        remaining = worker

print()
print(seconds)
print(remaining)
print(seconds + remaining)

