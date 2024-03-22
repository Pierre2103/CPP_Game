from PIL import Image
import numpy as np

# Convert hex to RGB
hex_to_rgb = lambda hx: tuple(int(hx[i:i+2], 16) for i in (1, 3, 5))

# Mapping of terrain colors to their values with correct RGB color values
terrain_colors = {
    hex_to_rgb("#4a9d0a"): 0,  # Grass
    hex_to_rgb("#00d8ff"): 1,  # Water
    hex_to_rgb("#fff200"): 2,  # Sand
    hex_to_rgb("#828282"): 3,  # Rock
    hex_to_rgb("#80430d"): 4,  # Tree
    hex_to_rgb("#1500ff"): 5,  # Firecamp
    hex_to_rgb("#118056"): 6,  # Workbench
    hex_to_rgb("#00ff9f"): 7,  # House
    hex_to_rgb("#f87e00"): 8,  # Plantation
    hex_to_rgb("#7a00ff"): 9,  # Animal Enclosures
    hex_to_rgb("#000000"): 10, # Blackbox
    hex_to_rgb("#ff6b6b"): 11, # Shelter Walls
    hex_to_rgb("#ffffff"): 12, # Animal Spawner
}

# Load the image
image_path = 'assets/tilemap.png'
image = Image.open(image_path)
pixels = np.array(image)

# Function to match colors with a tolerance
def match_color(rgb, tolerance=50):
    for color, value in terrain_colors.items():
        if all(abs(c - t) <= tolerance for c, t in zip(color, rgb)):
            return value
    return -1

# Create the tilemap array
tilemap = np.full((pixels.shape[0], pixels.shape[1]), -1, dtype=int)

# Process each pixel and match it to the terrain type
for y in range(pixels.shape[0]):
    for x in range(pixels.shape[1]):
        # Get the RGB values of the pixel
        pixel_rgb = tuple(pixels[y, x][:3])
        tilemap[y, x] = match_color(pixel_rgb)

# Convert the tilemap array to a C++ style array
tilemap_lines = ['{' + ', '.join(str(cell) for cell in row) + '}' for row in tilemap]
tilemap_cpp = 'int tilemap[{}][{}] = {{\n{}\n}};'.format(len(tilemap), len(tilemap[0]), ',\n'.join(tilemap_lines))

# Save the tilemap to a file
tilemap_path = 'tilemap.txt'
with open(tilemap_path, 'w') as file:
    file.write(tilemap_cpp)

# then output the size of the tilemap
print('Tilemap size:', len(tilemap), 'x', len(tilemap[0]))

print('Tilemap saved to', tilemap_path)
