<!DOCTYPE html>
<html>
<head>
    <title>Process Variable</title>
</head>
<body>
    <?php
    // Check if the variable exists in the URL
    // if (isset($_GET['variable'])) {
        // Retrieve the variable from the URL
        $variable = $_GET['variable'];
        echo "<p>The variable from the URL is: $variable</p>";
    // } else {
    //     echo "<p>No variable found in the URL.</p>";
    // }
    ?>
</body>
</html>