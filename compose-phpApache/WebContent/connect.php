<?php
# FileName="connect.php"
$hostname = "AWS_RDS_END_POINT";
$database = "DB_NAME";
$username = "postgres";
$password = "admin";

// Create PDO DSN
$dsn = "pgsql:host=$hostname;port=5432;dbname=$database;user=$username;password=$password";

// Create connection
$opt = array(
    PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC
);
$connect = new PDO($dsn, $username, $password, $opt);
?>