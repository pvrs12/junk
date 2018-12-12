i = 5235

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
for x in range(1,299):
    for y in range(1,299):
        a = (calc_power(x,y, i)
            + calc_power(x+1, y, i)
            + calc_power(x+2, y, i)
            + calc_power(x, y+1, i)
            + calc_power(x+1, y+1, i)
            + calc_power(x+2, y+1, i)
            + calc_power(x, y+2, i)
            + calc_power(x+1, y+2, i)
            + calc_power(x+2, y+2, i))
        if a > max_v:
            max_v = a
            max_x = x
            max_y = y
print(max_x,max_y)
