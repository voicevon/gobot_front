import queue

q= queue.Queue(maxsize=9)

for i in range(5):
    q.put(i)


while not q.empty():
    print(q.get())