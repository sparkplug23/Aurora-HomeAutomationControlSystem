import json


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
    left_step = (mid - start) / max(midpoint, 1)
    for i in range(1, midpoint):
        output[i] = int(start + i * left_step)

    # Fill in values between mid and end
    right_step = (end - mid) / max(width - midpoint - 1, 1)
    for i in range(midpoint + 1, width):
        output[i] = int(mid + (i - midpoint) * right_step)

    return output


def validate_matrix(matrix):
    """Validate that all indices from 0 to max value are included exactly once."""
    flat_map = [val for row in matrix for val in row if val != -1]
    max_value = max(flat_map)
    missing_indices = sorted(set(range(max_value + 1)) - set(flat_map))
    duplicate_indices = {idx: flat_map.count(idx) for idx in flat_map if flat_map.count(idx) > 1}

    # Debug information only
    if missing_indices:
        print(f"Validation Error: Missing indices: {missing_indices}")
    if duplicate_indices:
        print(f"Validation Error: Duplicate indices: {duplicate_indices}")
    if not missing_indices and not duplicate_indices:
        print("Validation Passed: All indices are present and unique.")

    # Return validation status
    return not missing_indices and not duplicate_indices


def generate_matrix(input_data):
    """Generate the matrix and calculate width and height."""
    # Determine the maximum width (largest interpolated row)
    widths = [max(abs(row[2] - row[0]) + 1, abs(row[1] - row[0]) + 1) for row in input_data]
    max_width = max(widths)

    # Generate rows using the calculated width
    matrix = [generate_row(row[0], row[1], row[2], max_width) for row in input_data]

    # Validate the matrix (debug only)
    validate_matrix(matrix)

    return matrix, max_width, len(input_data)


def save_to_json_with_newlines(matrix, width, height, name, filename):
    """Save the matrix to a JSON file with new lines at the end of each row."""
    # Start the JSON structure
    json_data = {
        "n": name,
        "width": width,
        "height": height,
        "map": matrix,
    }

    # Format the map to include new lines
    json_data_str = json.dumps(json_data, indent=2)
    formatted_map = json_data_str.replace("],", "],\n")

    # Write to the file
    with open(filename, "w") as f:
        f.write(formatted_map)

    print(f"JSON file saved as {filename}")


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
    [25, 3, 26],
]

# Generate the matrix and metadata
matrix, width, height = generate_matrix(input_data)

# Save the structured output
save_to_json_with_newlines(matrix, width, height, "Hat Mask", "ledmap__validated.json")
