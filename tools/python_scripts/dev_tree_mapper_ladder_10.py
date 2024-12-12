import json
import os
import numpy as np


def load_input_data(filename):
    """Load input data from a JSON file."""
    with open(filename, "r") as f:
        input_data = json.load(f)
    return input_data


def interpolate_row(start, mid, end, width):
    """Interpolate a row with equal spacing and unique indices."""
    # Generate positions for start, mid, and end
    positions = [0, width // 2, width - 1]
    values = [start, mid, end]

    # Linear interpolation for the entire row
    interpolated = np.interp(range(width), positions, values)
    return [int(value) for value in interpolated]


def generate_matrix(input_data, reverse=False):
    """Generate the matrix with equal spacing and unique indices."""
    if reverse:
        input_data = input_data[::-1]

    # Determine the width of the matrix (widest row)
    max_width = max(abs(row[2] - row[0]) + 1 for row in input_data)

    # Pre-fill the matrix with -1
    height = len(input_data)
    matrix = [[-1 for _ in range(max_width)] for _ in range(height)]

    # Populate each row
    all_indices = set()
    for row_idx, row in enumerate(input_data):
        start, mid, end = row
        interpolated_row = interpolate_row(start, mid, end, max_width)

        # Populate the matrix row while ensuring no conflicts with existing indices
        for col_idx, value in enumerate(interpolated_row):
            if value not in all_indices:
                matrix[row_idx][col_idx] = value
                all_indices.add(value)

    return matrix, max_width, height


def validate_matrix(matrix):
    """Validate that all indices in the matrix are unique."""
    flat_map = [val for row in matrix for val in row if val != -1]
    duplicates = {x for x in flat_map if flat_map.count(x) > 1}

    if duplicates:
        print(f"Validation Error: Duplicates found - {duplicates}")
        return False
    print("Validation Passed: All indices are unique.")
    return True


def flatten_matrix(matrix):
    """Flatten the 2D matrix into a single array."""
    return [val for row in matrix for val in row]


def save_to_json_with_newlines(matrix, width, height, name, filename):
    """Save the matrix to a JSON file with new lines at the end of each row."""
    json_data = {
        "n": name,
        "width": width,
        "height": height,
        "map": matrix,
    }

    json_data_str = json.dumps(json_data, indent=2)
    formatted_map = json_data_str.replace("],", "],\n")

    with open(filename, "w") as f:
        f.write(formatted_map)

    print(f"JSON file saved as {filename}")


def save_flattened_map(flattened_map, width, height, name, filename):
    """Save the flattened map to a compact JSON file."""
    json_data = {
        "n": name,
        "width": width,
        "height": height,
        "map": flattened_map,
    }

    with open(filename, "w") as f:
        json.dump(json_data, f, separators=(",", ":"))

    print(f"Flattened JSON file saved as {filename}")


# Main execution
if __name__ == "__main__":
    input_file_dir = r"C:\pool_local\Github\PulSar\tools\python_scripts"  # Specify the input JSON file location
    input_file = "mapping_range_rows__tree_01.json"  # Specify the input JSON file
    output_file_matrix = "ledmap_matrix.json"  # Output file for the full matrix
    output_file_flat = "ledmap_flat.json"  # Output file for the flattened map

    # Load input data
    input_data = load_input_data(os.path.join(input_file_dir, input_file))

    # Generate the matrix with optional reversal
    reverse_rows = True  # Change to False to keep the original order
    matrix, width, height = generate_matrix(input_data, reverse=reverse_rows)

    # Validate the generated matrix
    validate_matrix(matrix)

    # Save the structured output as matrix
    save_to_json_with_newlines(matrix, width, height, "Reversed Map" if reverse_rows else "Original Map",
                               os.path.join(input_file_dir, output_file_matrix))

    # Save the structured output as flattened array
    flattened_map = flatten_matrix(matrix)
    save_flattened_map(flattened_map, width, height, "Reversed Map" if reverse_rows else "Original Map",
                       os.path.join(input_file_dir, output_file_flat))
