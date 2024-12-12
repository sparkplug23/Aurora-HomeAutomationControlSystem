import json
from collections import defaultdict

matrix = defaultdict(list)
width = 6

# each row in the tree, from top to bottom.
# [first led, middle led, last led], counting from 0
input_data = [
    [25, 3, 26],
    [65, 45, 66],
    [105, 85, 106],
    [143, 124, 144],
    [180, 162, 181],
    [223, 206, 224],
    [258, 241, 256],
    [291, 275, 292],
    [324, 308, 325],
    [354, 340, 355],
    [383, 370, 384],
    [412, 399, 413],
    [438, 426, 439],
    [463, 452, 464],
    [487, 476, 488],
    [510, 500, 511],
    [532, 522, 533],
    [554, 544, 555],
    [575, 565, 576],
    [594, 585, 595],
    [613, 605, 614],
    [630, 623, 631],
    [646, 639, 647],
    [661, 655, 662],
    [676, 670, 677],
    [691, 685, 692],
    [705, 699, 706],
    [719, 713, 720],
    [732, 726, 733],
    [744, 738, 745],
    [755, 749, 756],
    [765, 760, 766],
    [774, 770, 775],
    [783, 779, 784],
    [791, 787, 792],
    [797, 794, 798],
    [803, 800, 804]
]



def generate_row(start, mid, end, width):
    output = []

    midpoint = int(width / 2)
    left_length = abs(mid - start) + 1  # Absolute distance to handle reverse
    right_length = abs(end - mid)

    # Check if the row should be reversed
    left_step = -1 if start > mid else 1
    right_step = -1 if mid > end else 1

    # Generate left side from `start` to `mid`
    step = left_length / midpoint
    prev = start - left_step  # Adjust based on direction
    for i in range(midpoint):
        pos = int(start + left_step * step * i)
        if pos != prev:
            output.append(pos)
            prev = pos
        else:
            output.append(-1)  # Placeholder if no new position

    # Generate right side from `mid` to `end`
    step = right_length / (width - midpoint)
    prev = mid - right_step  # Adjust based on direction
    for i in range(1, width - midpoint + 1):
        pos = int(mid + right_step * step * i)
        if pos != prev:
            output.append(pos)
            prev = pos
        else:
            output.append(-1)  # Placeholder if no new position

    return output


def generate_matrix(input_data, width):
    matrix = []
    for row in input_data:
        matrix.append(generate_row(row[0], row[1], row[2], width))
    return matrix


def save_to_json(matrix, filename, pretty=False):
    with open(filename, 'w') as f:
        if pretty:
            # Manually format each row as a separate line in JSON
            f.write('{\n  "map": [\n')
            for i, row in enumerate(matrix):
                json_row = json.dumps(row)
                f.write(f'    {json_row}')
                if i < len(matrix) - 1:
                    f.write(',\n')
                else:
                    f.write('\n')
            f.write('  ]\n}\n')
        else:
            json.dump({'map': matrix}, f)


# Generate the matrix and save it to JSON
matrix = generate_matrix(input_data, width)
save_to_json(matrix, 'ledmap__generated.json', pretty=True)  # Set pretty=True for debugging output

# Flattened matrix for extended output
flat_matrix = []
for row in input_data:
    flat_matrix.extend(generate_row(row[0], row[1], row[2], width))

with open('ledmap.json', 'w') as f:
    json.dump({'map': flat_matrix}, f)
