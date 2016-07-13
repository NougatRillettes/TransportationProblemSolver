import sys
import random

n = int(sys.argv[1])

sumX = 0
x = []
for _ in range(n):
    a = random.randrange(10)+1
    sumX += a
    x.append(a)
    
delta = sumX
m = 0
y = []
while(delta > 10):
    i = random.randrange(10)+1
    delta -= i
    y.append(i)
    m += 1
if delta > 0:
    y.append(delta)
    m +=1

print(n,m)
for (k,v) in enumerate(x):
    print(2*k,v,end=' ',sep=':')
print()
for (k,v) in enumerate(y):
    print(2*k+1,v,end=' ',sep=':')
print()
for _ in range(n):
    for _ in range(m):
        print(random.randrange(10,100),end=' ')
    print()

