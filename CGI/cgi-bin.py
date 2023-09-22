import os

# Get the query string from the environment
query_string = os.environ.get('QUERY_STRING', '')


# Split the query string into individual key-value pairs
params = query_string.split('&')

# Access the values of first_name and last_name
first_name = ''
last_name = ''

# Iterate over the key-value pairs
for param in params:
    if '=' in param:
        key, value = param.split('=')
        if key == 'first_name':
            first_name = value
        elif key == 'last_name':
            last_name = value
    else:
        # Handle parameters without values
        if param == 'first_name':
            first_name = ''
        elif param == 'last_name':
            last_name = ''




print ("\r\n")
print ("<html>")
print ("<head>")
print ("<title>Hello py cgi</title>")
print ("</head>")
print ("<body>")
print("<div style='text-align: center; margin-top: 100px;'>")
print ("<h1>Hello %s %s</h1>" % (first_name, last_name))
print ("<p>Thank you for using our service!</p>")
print ("</body>")
print ("</html>")