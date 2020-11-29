<?php
include('connect.php');

	
$query = $connect->prepare("-- TEMP HI/LO for 24hrs
SELECT t.timezone, t.temperature
FROM 	(select timezone, temperature, row_number() over (order by temperature) as rn,
			count(*) over () as total_count
			from sensor_data.humidor_decoded
		    where timezone >  date_trunc('day',NOW() AT TIME ZONE 'America/New_York' - interval '1 day' ) 
		) t
where t.rn = 1 or t.rn = total_count
and  t.timezone >  date_trunc('day',NOW()) 
order by t.temperature desc");
$query->execute();

$temp = $query->fetchAll(PDO::FETCH_ASSOC);

echo json_encode($temp);

?>





