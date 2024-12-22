import json
from collections import defaultdict


matrix = defaultdict(list)
width = 30

# each row in the tree, from top to bottom.
# [first led, middle led, last led], counting from 0
input = [
    [258, 263, 265],
    [246, 251, 257],
    [232, 238, 245],
    [218, 224, 231],
    [201, 208, 217],
    [184, 192, 200],
    [161, 172, 183],
    [140, 151, 162],
    [115, 127, 139],
    [90, 101, 114],
    [60, 74, 89],
    [30, 43, 59],
    [0, 14, 29]
]


def generate_row(start, mid, end, width):
    output = []

    midpoint = int(width / 2)
    left_length = mid - (start - 1)
    right_length = end - mid

    step = left_length/midpoint
    prev = start - 1
    for i in range(midpoint):
        val = -1
        pos = start + int(step * i)
        if pos > prev:
            val = pos
        prev = pos
        output.append(val)

    step = right_length/midpoint
    for i in range(1, width-midpoint+1):
        val = -1
        pos = mid + int((step * i) + 0.5)
        if pos > prev:
            val = pos
            prev = pos
        output.append(val)

    return output


matrix = []
for i in input:
    matrix.extend(generate_row(i[0], i[1], i[2], width))

with open('ledmap__generated.json', 'w') as f:
     json.dump({'map': matrix}, f)