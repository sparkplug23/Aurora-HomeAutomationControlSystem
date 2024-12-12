import json
import os

def load_input_data(filename):
    """Load input data from a JSON file."""
    with open(filename, "r") as f:
        input_data = json.load(f)
    return input_data

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

    if missing_indices:
        print(f"Validation Error: Missing indices: {missing_indices}")
    if duplicate_indices:
        print(f"Validation Error: Duplicate indices: {duplicate_indices}")
    if not missing_indices and not duplicate_indices:
        print("Validation Passed: All indices are present and unique.")

    return not missing_indices and not duplicate_indices

def generate_matrix(input_data, reverse=False):
    """Generate the matrix and calculate width and height."""
    if reverse:
        input_data = input_data[::-1]  # Reverse the rows if the option is enabled

    # Determine the maximum width (largest interpolated row)
    widths = [max(abs(row[2] - row[0]) + 1, abs(row[1] - row[0]) + 1) for row in input_data]
    max_width = max(widths)

    # Generate rows using the calculated width
    matrix = [generate_row(row[0], row[1], row[2], max_width) for row in input_data]

    # Validate the matrix
    validate_matrix(matrix)

    return matrix, max_width, len(input_data)

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

# Main execution
if __name__ == "__main__":
    # Specify the input and output directories
    input_file_dir = r"C:\pool_local\Github\PulSar\tools\python_scripts"
    output_file_dir = r"C:\pool_local\Github\PulSar\tools\python_scripts"

    # Define file names
    input_file = os.path.join(input_file_dir, "mapping_range_rows__tree_01.json")
    output_file = os.path.join(output_file_dir, "ledmap__reversed_or_normalized.json")

    # Load input data
    input_data = load_input_data(input_file)

    # Generate the matrix with optional reversal
    reverse_rows = True  # Change to False to keep the original order
    matrix, width, height = generate_matrix(input_data, reverse=reverse_rows)

    # Save the structured output
    save_to_json_with_newlines(matrix, width, height, "Reversed Map" if reverse_rows else "Original Map", output_file)
