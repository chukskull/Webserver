<?php
// Get the query string from the URL
$query_string = $_SERVER['QUERY_STRING'];

// Split the query string into individual key-value pairs
$params = explode('&', $query_string);

// Access the values of first_name and last_name
$first_name = '';
$last_name = '';

// Iterate over the key-value pairs
foreach ($params as $param) {
    if (strpos($param, '=') !== false) {
        list($key, $value) = explode('=', $param);
        if ($key === 'first_name') {
            $first_name = $value;
        } elseif ($key === 'last_name') {
            $last_name = $value;
        }
    } else {
        // Handle parameters without values
        if ($param === 'first_name') {
            $first_name = '';
        } elseif ($param === 'last_name') {
            $last_name = '';
        }
    }
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Hello PHP CGI</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-image: url(img.png); /* Replace with the actual file path */
            background-repeat: no-repeat;
            background-size: cover;
            margin: 0;
            padding: 20px;
        }

        .container {
            max-width: 600px;
            margin: 0 auto;
            background-color: rgba(255, 255, 255, 0.8);
            padding: 20px;
            border-radius: 5px;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
            text-align: center;
        }

        h1 {
            color: #FF4500;
        }

        p {
            color: #333;
        }

        .thank-you {
            font-size: 18px;
            margin-top: 20px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Hello <?php echo $first_name . ' ' . $last_name; ?></h1>
        <p class="thank-you">Thank you for using our service!</p>
    </div>
</body>
</html>
