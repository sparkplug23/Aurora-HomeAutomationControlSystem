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


def calculate_max_width(input_data):
    """
    Dynamically calculate the maximum width for the map based on the largest range
    between the left edge and right edge in the input data.
    """
    max_width = 0
    for row in input_data:
        # The width includes the distance from left to right and the center point
        max_width = max(max_width, abs(row[2] - row[0]) + 1)
    return max_width


def generate_row(start, mid, end, width):
    """Generate a row of the map with interpolated and scaled values."""
    output = [-1] * width

    # Compute positions for left, mid, and right in the row
    midpoint = width // 2
    left_idx = 0
    mid_idx = midpoint
    right_idx = width - 1

    # Assign values at key positions
    output[left_idx] = start
    output[mid_idx] = mid
    output[right_idx] = end

    # Fill in values between start and mid
    if midpoint > 1:
        left_step = (mid - start) / (midpoint - 1)
        for i in range(1, midpoint):
            output[i] = int(start + i * left_step)

    # Fill in values between mid and end
    if width - midpoint - 1 > 0:
        right_step = (end - mid) / (width - midpoint - 1)
        for i in range(midpoint + 1, width):
            output[i] = int(mid + (i - midpoint) * right_step)

    return output


def generate_matrix(input_data):
    """Generate the matrix and calculate width and height."""
    # Dynamically determine the maximum width based on the input data
    max_width = calculate_max_width(input_data)

    # Generate rows using the consistent maximum width
    matrix = [generate_row(row[0], row[1], row[2], max_width) for row in input_data]

    return matrix, max_width, len(matrix)


def save_to_json(matrix, width, height, name, filename):
    """Save the matrix to a JSON file."""
    json_data = {
        "n": name,
        "width": width,
        "height": height,
        "map": [val for row in matrix for val in row]  # Flattened map
    }

    with open(filename, 'w') as f:
        json.dump(json_data, f, indent=2)


# Generate the matrix and metadata
matrix, width, height = generate_matrix(input_data)

# Save the structured output
save_to_json(matrix, width, height, "Hat Mask", "ledmap__generated.json")

# Print success message
print("Matrix generation complete.")
