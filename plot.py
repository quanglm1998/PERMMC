import numpy as np
import matplotlib.pyplot as plt

path = "result/small_cases.txt"

file = open(path, 'r')

x = []
y0 = []
y1 = []
y2 = []

cnt = 0
cur0 = 0
cur1 = 0
cur2 = 0
num = 0
for line in file.readlines():
  line = line.strip();
  if cnt % 21  == 0:
    x.append(int(line))
    if cnt > 0:
      y0.append(float(cur0) / num)
      y1.append(float(cur1) / num)
      y2.append(float(cur2) / num)
    cur0 = cur1 = cur2 = num = 0
  else:
    (val0, val1, val2) = line.split();
    cur0 += int(val0)
    cur1 += int(val1)
    cur2 += int(val2)
    num += 1

  cnt += 1

y0.append(float(cur0) / num)
y1.append(float(cur1) / num)
y2.append(float(cur2) / num)
# for i in range(0, len(x)):
#   x[i] = float(x[i]) / 10000

t = range(len(x))
plt.xticks(t, x)


print(y0)
print(y1)
print(y2);

# z = []
# for i in range(0, len(y1)):
#   z.append(float(y2[i]) / y1[i])

# print(z)


plt.xlabel('Số lượng cảm biến')
plt.ylabel('Máy sạc tối thiểu')

plt.plot(t, y0, 'g*-', markerfacecolor='none', label='BMDP')
plt.plot(t, y1, 'bs-', markerfacecolor='none', label='LOA')
plt.plot(t, y2, 'ro-', markerfacecolor='none', label='MCSA')
plt.legend()

plt.show()

file.close()