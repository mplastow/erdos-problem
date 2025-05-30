# Generated by ChatGPT on May 14, 2025

import math
import matplotlib.pyplot as plt
from sympy import isprime

def is_visible(u, v):
    return math.gcd(u, v) == 1

def is_forbidden(u, v):
    return isprime(u) and isprime(v)

def next_step(u, v):
    if is_visible(u + 1, v) and not is_forbidden(u + 1, v):
        return (u + 1, v)
    if is_visible(u, v + 1) and not is_forbidden(u, v + 1):
        return (u, v + 1)
    for k in range(2, 10):
        if is_visible(u + k, v) and not is_forbidden(u + k, v):
            return (u + k, v)
        if is_visible(u, v + k) and not is_forbidden(u, v + k):
            return (u, v + k)
    return None

def generate_visible_path(n_steps=100, start=(1, 2)):
    path = [start]
    u, v = start
    for _ in range(n_steps - 1):
        next_p = next_step(u, v)
        if not next_p:
            print("No valid move found at:", (u, v))
            break
        path.append(next_p)
        u, v = next_p
    return path

def plot_path(path, show_forbidden=True):
    x_vals, y_vals = zip(*path)

    plt.figure(figsize=(10, 10))
    plt.plot(x_vals, y_vals, '-o', color='blue', label='Visible path')

    # Optional: highlight forbidden points in bounding box
    if show_forbidden:
        max_u = max(x_vals) + 2
        max_v = max(y_vals) + 2
        fx, fy = [], []
        for u in range(1, max_u):
            for v in range(1, max_v):
                if is_visible(u, v) and is_forbidden(u, v):
                    fx.append(u)
                    fy.append(v)
        plt.scatter(fx, fy, color='red', s=20, marker='x', label='Forbidden points')

    plt.title('Path of Visible Points Avoiding (Prime, Prime)')
    plt.xlabel('u')
    plt.ylabel('v')
    plt.grid(True)
    plt.axis('equal')
    plt.legend()
    plt.show()

# Run the path generator and plot
if __name__ == "__main__":
    path = generate_visible_path(100)
    plot_path(path)