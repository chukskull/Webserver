import sys

# Check if an argument was provided
if len(sys.argv) < 2:
    print("No input found !")
    sys.exit(1)

# Get the first argument and remove newlines
argument = sys.argv[1]
length = len(argument)

# Print the length of the argument
print("Length of the argument:", length)
