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
    Dynamically calculate the maximum width for the map based on cylindrical wrapping.
    """
    widths = []
    for row in input_data:
        # Width calculation as long path through center
        width = abs(row[1] - row[0]) + abs(row[2] - row[1]) + 1
        widths.append(width)
        print(f"Row: {row}, Calculated Width: {width}")  # Debug print for row widths
    max_width = max(widths)
    print(f"Maximum Width: {max_width}")  # Debug print for maximum width
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
    height = len(input_data)  # The number of rows is the height
    print(f"Matrix Height: {height}, Matrix Width: {max_width}")  # Debug for height and width
    return matrix, max_width, height

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


## first working: missing section at front
# {"n":"Hat Mask", "width":46, "height":36, "map": [803, -1, -1, -1, -1, -1, -1, -1, 804, -1, -1, -1, -1, -1, -1, -1, 805, -1, -1, -1, -1, -1, -1, 800, -1, -1, -1, -1, -1, 801, -1, -1, -1, -1, 802, -1, -1, -1, -1, -1, 803, -1, -1, -1, -1, 804, 797, -1, -1, -1, -1, -1, -1, -1, 798, -1, -1, -1, -1, -1, -1, -1, 799, -1, -1, -1, -1, -1, -1, 794, -1, -1, -1, -1, -1, 795, -1, -1, -1, -1, 796, -1, -1, -1, -1, -1, 797, -1, -1, -1, -1, 798, 791, -1, -1, -1, -1, -1, 792, -1, -1, -1, -1, -1, 793, -1, -1, -1, -1, -1, 794, -1, -1, -1, -1, 787, -1, -1, -1, -1, 788, -1, -1, -1, 789, -1, -1, -1, -1, 790, -1, -1, -1, 791, -1, -1, -1, 792, 783, -1, -1, -1, -1, -1, 784, -1, -1, -1, -1, -1, 785, -1, -1, -1, -1, -1, 786, -1, -1, -1, -1, 779, -1, -1, -1, -1, 780, -1, -1, -1, 781, -1, -1, -1, -1, 782, -1, -1, -1, 783, -1, -1, -1, 784, 774, -1, -1, -1, -1, -1, 775, -1, -1, -1, -1, -1, 776, -1, -1, -1, -1, -1, 777, -1, -1, -1, -1, 770, -1, -1, -1, -1, 771, -1, -1, -1, 772, -1, -1, -1, -1, 773, -1, -1, -1, 774, -1, -1, -1, 775, 765, -1, -1, -1, -1, 766, -1, -1, -1, -1, 767, -1, -1, -1, 768, -1, -1, -1, -1, 769, -1, -1, -1, 760, -1, -1, -1, 761, -1, -1, -1, 762, -1, -1, 763, -1, -1, -1, 764, -1, -1, -1, 765, -1, -1, 766, 755, -1, -1, -1, 756, -1, -1, -1, 757, -1, -1, -1, 758, -1, -1, -1, 759, -1, -1, -1, 760, -1, -1, 749, -1, -1, -1, 750, -1, -1, 751, -1, -1, 752, -1, -1, 753, -1, -1, 754, -1, -1, 755, -1, -1, 756, 744, -1, -1, -1, 745, -1, -1, -1, 746, -1, -1, -1, 747, -1, -1, -1, 748, -1, -1, -1, 749, -1, -1, 738, -1, -1, -1, 739, -1, -1, 740, -1, -1, 741, -1, -1, 742, -1, -1, 743, -1, -1, 744, -1, -1, 745, 732, -1, -1, -1, 733, -1, -1, -1, 734, -1, -1, -1, 735, -1, -1, -1, 736, -1, -1, -1, 737, -1, -1, 726, -1, -1, -1, 727, -1, -1, 728, -1, -1, 729, -1, -1, 730, -1, -1, 731, -1, -1, 732, -1, -1, 733, 719, -1, -1, -1, 720, -1, -1, -1, 721, -1, -1, -1, 722, -1, -1, -1, 723, -1, -1, -1, 724, -1, -1, 713, -1, -1, -1, 714, -1, -1, 715, -1, -1, 716, -1, -1, 717, -1, -1, 718, -1, -1, 719, -1, -1, 720, 705, -1, -1, -1, 706, -1, -1, -1, 707, -1, -1, -1, 708, -1, -1, -1, 709, -1, -1, -1, 710, -1, -1, 699, -1, -1, -1, 700, -1, -1, 701, -1, -1, 702, -1, -1, 703, -1, -1, 704, -1, -1, 705, -1, -1, 706, 691, -1, -1, -1, 692, -1, -1, -1, 693, -1, -1, -1, 694, -1, -1, -1, 695, -1, -1, -1, 696, -1, -1, 685, -1, -1, -1, 686, -1, -1, 687, -1, -1, 688, -1, -1, 689, -1, -1, 690, -1, -1, 691, -1, -1, 692, 676, -1, -1, -1, 677, -1, -1, -1, 678, -1, -1, -1, 679, -1, -1, -1, 680, -1, -1, -1, 681, -1, -1, 670, -1, -1, -1, 671, -1, -1, 672, -1, -1, 673, -1, -1, 674, -1, -1, 675, -1, -1, 676, -1, -1, 677, 661, -1, -1, -1, 662, -1, -1, -1, 663, -1, -1, -1, 664, -1, -1, -1, 665, -1, -1, -1, 666, -1, -1, 655, -1, -1, -1, 656, -1, -1, 657, -1, -1, 658, -1, -1, 659, -1, -1, 660, -1, -1, 661, -1, -1, 662, 646, -1, -1, -1, 647, -1, -1, 648, -1, -1, 649, -1, -1, -1, 650, -1, -1, 651, -1, -1, 652, -1, -1, 639, -1, -1, 640, -1, -1, 641, -1, -1, 642, -1, 643, -1, -1, 644, -1, -1, 645, -1, -1, 646, -1, 647, 630, -1, -1, -1, 631, -1, -1, 632, -1, -1, 633, -1, -1, -1, 634, -1, -1, 635, -1, -1, 636, -1, -1, 623, -1, -1, 624, -1, -1, 625, -1, -1, 626, -1, 627, -1, -1, 628, -1, -1, 629, -1, -1, 630, -1, 631, 613, -1, -1, 614, -1, -1, 615, -1, -1, 616, -1, -1, 617, -1, -1, 618, -1, -1, 619, -1, -1, 620, -1, 605, -1, -1, 606, -1, 607, -1, -1, 608, -1, 609, -1, -1, 610, -1, 611, -1, -1, 612, -1, 613, -1, 614, 594, -1, -1, 595, -1, -1, 596, -1, 597, -1, -1, 598, -1, 599, -1, -1, 600, -1, 601, -1, -1, 602, -1, 585, -1, -1, 586, -1, 587, -1, 588, -1, 589, -1, 590, -1, -1, 591, -1, 592, -1, 593, -1, 594, -1, 595, 575, -1, -1, 576, -1, 577, -1, 578, -1, -1, 579, -1, 580, -1, 581, -1, -1, 582, -1, 583, -1, 584, -1, 565, -1, 566, -1, 567, -1, 568, -1, 569, -1, 570, -1, 571, -1, 572, -1, 573, -1, 574, -1, 575, -1, 576, 554, -1, -1, 555, -1, 556, -1, 557, -1, -1, 558, -1, 559, -1, 560, -1, -1, 561, -1, 562, -1, 563, -1, 544, -1, 545, -1, 546, -1, 547, -1, 548, -1, 549, -1, 550, -1, 551, -1, 552, -1, 553, -1, 554, -1, 555, 532, -1, -1, 533, -1, 534, -1, 535, -1, -1, 536, -1, 537, -1, 538, -1, -1, 539, -1, 540, -1, 541, -1, 522, -1, 523, -1, 524, -1, 525, -1, 526, -1, 527, -1, 528, -1, 529, -1, 530, -1, 531, -1, 532, -1, 533, 510, -1, -1, 511, -1, 512, -1, 513, -1, -1, 514, -1, 515, -1, 516, -1, -1, 517, -1, 518, -1, 519, -1, 500, -1, 501, -1, 502, -1, 503, -1, 504, -1, 505, -1, 506, -1, 507, -1, 508, -1, 509, -1, 510, -1, 511, 487, -1, -1, 488, -1, 489, -1, 490, -1, 491, -1, 492, -1, 493, -1, 494, -1, 495, -1, 496, -1, 497, -1, 476, -1, 477, -1, 478, -1, 479, -1, 480, -1, 481, 482, -1, 483, -1, 484, -1, 485, -1, 486, -1, 487, 488, 463, -1, -1, 464, -1, 465, -1, 466, -1, 467, -1, 468, -1, 469, -1, 470, -1, 471, -1, 472, -1, 473, -1, 452, -1, 453, -1, 454, -1, 455, -1, 456, -1, 457, 458, -1, 459, -1, 460, -1, 461, -1, 462, -1, 463, 464, 438, -1, 439, -1, 440, -1, 441, -1, 442, -1, 443, -1, 444, -1, 445, -1, 446, -1, 447, -1, 448, -1, 449, 426, -1, 427, -1, 428, -1, 429, 430, -1, 431, -1, 432, 433, -1, 434, -1, 435, 436, -1, 437, -1, 438, 439, 412, -1, 413, -1, 414, -1, 415, -1, 416, 417, -1, 418, -1, 419, -1, 420, 421, -1, 422, -1, 423, -1, 424, 399, -1, 400, -1, 401, 402, -1, 403, 404, -1, 405, 406, -1, 407, -1, 408, 409, -1, 410, 411, -1, 412, 413, 383, -1, 384, -1, 385, -1, 386, -1, 387, 388, -1, 389, -1, 390, -1, 391, 392, -1, 393, -1, 394, -1, 395, 370, -1, 371, -1, 372, 373, -1, 374, 375, -1, 376, 377, -1, 378, -1, 379, 380, -1, 381, 382, -1, 383, 384, 354, -1, 355, -1, 356, 357, -1, 358, -1, 359, 360, -1, 361, -1, 362, 363, -1, 364, -1, 365, 366, -1, 367, 340, -1, 341, 342, -1, 343, 344, -1, 345, 346, -1, 347, 348, -1, 349, 350, -1, 351, 352, -1, 353, 354, 355, 324, -1, 325, 326, -1, 327, 328, -1, 329, 330, -1, 331, 332, 333, -1, 334, 335, -1, 336, 337, -1, 338, 339, 308, -1, 309, 310, 311, -1, 312, 313, 314, -1, 315, 316, 317, 318, -1, 319, 320, 321, -1, 322, 323, 324, 325, 291, -1, 292, 293, -1, 294, 295, -1, 296, 297, -1, 298, 299, 300, -1, 301, 302, -1, 303, 304, -1, 305, 306, 275, -1, 276, 277, 278, -1, 279, 280, 281, -1, 282, 283, 284, 285, -1, 286, 287, 288, -1, 289, 290, 291, 292, 258, -1, 259, 260, -1, 261, 262, 263, -1, 264, 265, 266, -1, 267, 268, 269, -1, 270, 271, 272, -1, 273, 274, 241, -1, 242, 243, -1, 244, 245, -1, 246, 247, -1, 248, 249, -1, 250, 251, -1, 252, 253, -1, 254, 255, 256, 223, -1, 224, 225, -1, 226, 227, 228, -1, 229, 230, 231, -1, 232, 233, 234, -1, 235, 236, 237, -1, 238, 239, 206, -1, 207, 208, 209, 210, -1, 211, 212, 213, 214, 215, -1, 216, 217, 218, 219, -1, 220, 221, 222, 223, 224, 180, -1, 181, 182, 183, -1, 184, 185, 186, 187, -1, 188, 189, 190, -1, 191, 192, 193, 194, -1, 195, 196, 197, 162, -1, 163, 164, 165, 166, 167, 168, -1, 169, 170, 171, 172, 173, 174, -1, 175, 176, 177, 178, 179, 180, 181, 143, -1, 144, 145, 146, 147, -1, 148, 149, 150, 151, 152, -1, 153, 154, 155, 156, 157, -1, 158, 159, 160, 161, 124, -1, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, -1, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 105, -1, 106, 107, 108, 109, 110, 111, -1, 112, 113, 114, 115, 116, 117, 118, -1, 119, 120, 121, 122, 123, 124, 85, -1, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 65, -1, 66, 67, 68, 69, 70, 71, -1, 72, 73, 74, 75, 76, 77, 78, -1, 79, 80, 81, 82, 83, 84, 45, -1, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 25, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 26]}
