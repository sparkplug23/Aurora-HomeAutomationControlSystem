import json

# Input data: [left edge, center point, right edge]
input_data = [
    [803, 800, 804],
    [797, 794, 798],
    [791, 787, 792],
    [783, 779, 784],
    [774, 770, 775],
    [765, 760, 766],
    [755, 749, 756],
    [744, 738, 745],
    [732, 726, 733],
    [719, 713, 720],
    [705, 699, 706],
    [691, 685, 692],
    [676, 670, 677],
    [661, 655, 662],
    [646, 639, 647],
    [630, 623, 631],
    [613, 605, 614],
    [594, 585, 595],
    [575, 565, 576],
    [554, 544, 555],
    [532, 522, 533],
    [510, 500, 511],
    [487, 476, 488],
    [463, 452, 464],
    [438, 426, 439],
    [412, 399, 413],
    [383, 370, 384],
    [354, 340, 355],
    [324, 308, 325],
    [291, 275, 292],
    [258, 241, 256],
    [223, 206, 224],
    [180, 162, 181],
    [143, 124, 144],
    [105, 85, 106],
    [65, 45, 66],
    [25, 3, 26]
]



def calculate_width(input_data):
    """Calculate the maximum width required for the map."""
    max_width = 0
    for row in input_data:
        start, mid, end = row
        left_length = abs(mid - start)
        right_length = abs(end - mid)
        max_width = max(max_width, left_length + right_length + 1)
    return max_width


def generate_row(start, mid, end, width):
    """Generate a single row of the matrix with -1 for unfilled positions."""
    output = [-1] * width
    midpoint = width // 2

    # Populate left to mid
    left_length = abs(mid - start)
    left_step = left_length / midpoint if midpoint > 0 else 0
    prev = start - 1
    for i in range(midpoint):
        pos = int(start + left_step * i)
        if pos > prev:
            output[i] = pos
            prev = pos

    # Set the midpoint
    output[midpoint] = mid

    # Populate mid to right
    right_length = abs(end - mid)
    right_step = right_length / (width - midpoint - 1) if (width - midpoint - 1) > 0 else 0
    prev = mid
    for i in range(midpoint + 1, width):
        pos = int(mid + right_step * (i - midpoint))
        if pos > prev:
            output[i] = pos
            prev = pos

    return output


def generate_matrix(input_data):
    """Generate the full matrix for all rows."""
    max_width = calculate_width(input_data)
    matrix = []
    for row in input_data:
        start, mid, end = row
        matrix.append(generate_row(start, mid, end, max_width))
    return matrix


def save_to_json(matrix, filename, pretty=False):
    """Save the matrix to a JSON file."""
    with open(filename, 'w') as f:
        if pretty:
            # Pretty-print for debugging
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
            # Standard JSON output
            json.dump({'map': matrix}, f)


# Generate the structured matrix
matrix = generate_matrix(input_data)

# Save the matrix in two formats
save_to_json(matrix, 'ledmap__generated.json', pretty=True)  # Easier-to-view structured output
flattened_matrix = [val for row in matrix for val in row]
save_to_json(flattened_matrix, 'ledmap.json')  # Flattened output for general use

print("Matrix generation complete. Files saved:")
print("- ledmap__generated.json (pretty)")
print("- ledmap.json (flattened)")
