<?php
include('connect.php');

	
$query = $connect->prepare("-- HUMIDITY HI/LO for 24hrs
SELECT h.timezone, h.humidity
FROM 	(select timezone, humidity, row_number() over (order by humidity) as rn,
			count(*) over () as total_count
			from sensor_data.humidor_decoded
		    where timezone >  date_trunc('day',NOW()) 
		) h
where h.rn = 1 or h.rn = total_count
order by h.humidity desc
");
$query->execute();

$temp = $query->fetchAll(PDO::FETCH_ASSOC);

echo json_encode($temp);

?>