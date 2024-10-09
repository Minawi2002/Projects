import numpy as np

# Define the shape of the array
shape = (100, 100, 3)

# Create the array with random RGB values
arr = np.random.randint(0, 256, size=shape, dtype=np.uint8)

# Print the array
print(arr)

