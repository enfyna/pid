import matplotlib.pyplot as plt
import numpy as np


current_speed = 0.0
target_speed = 500.0

current_acc = 0.0
inc_acc = 5.0
max_acc = 40.0

def move(amount: float):
    global current_speed
    global target_speed
    global current_acc
    global inc_acc
    global max_acc
    
    if abs(amount) > 1.0:
        amount = 1.0 * np.sign(amount)

    current_acc += amount * inc_acc
    if abs(current_acc) > max_acc:
        current_acc = max_acc * np.sign(current_acc)

    current_speed += current_acc
    
total_time = 1000

t = np.arange(0, total_time, 1)
y = np.array([0] * total_time, float)

p_k = 10.0

for c in range(total_time):
    p = p_k * ((target_speed - current_speed) / target_speed)
    move(p)

    print(current_acc)
    y[c] = current_speed


plt.plot(t, y)
plt.show()
