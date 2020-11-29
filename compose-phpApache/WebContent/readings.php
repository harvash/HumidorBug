<?php
include('connect.php');

	
$query = $connect->prepare(" -- CURRENT STATUS
select timezone, temperature, humidity
from sensor_data.humidor_decoded
order by timezone desc
limit 1");
$query->execute();

$survivors = $query->fetchAll(PDO::FETCH_ASSOC);

echo json_encode($survivors);

?>

1588042234