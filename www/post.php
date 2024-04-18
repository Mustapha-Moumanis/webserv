<?php
    echo "CGI PHP";
    while (1);
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // echo "under post";
    if (isset($_POST["name"]) && isset($_POST["email"])) {
        echo "name and email are seat? ";
        echo $_POST["name"];
        echo $_POST["email"];
    } 
    else
        echo "Name or email not set";
    echo "\n";
    var_dump($_POST);
}
?>
