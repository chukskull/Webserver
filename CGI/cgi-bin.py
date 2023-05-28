import sys


# while 1:
#     print("While 1 Example")

# Check if an argument was provided
if len(sys.argv) < 2:
    print("No input found !")
    sys.exit(1)

argument = sys.argv[1]
length = len(argument)

# Print the length of the argument
print("Length of the argument:", length,"")
