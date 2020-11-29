-------------------------------
for the project
-----------------------------
-- TEMP HI/LO for 24hrs
SELECT t.timezone, t.temperature
FROM 	(select timezone, temperature, row_number() over (order by temperature) as rn,
			count(*) over () as total_count
			from sensor_data.humidor_decoded
		    where timezone >  date_trunc('day',NOW()) 
		) t
where t.rn = 1 or t.rn = total_count
-- and  t.timezone >  date_trunc('day',NOW()) 
order by t.temperature desc

-- HUMIDITY HI/LO for 24hrs
SELECT h.timezone, h.humidity
FROM 	(select timezone, humidity, row_number() over (order by humidity) as rn,
			count(*) over () as total_count
			from sensor_data.humidor_decoded
		    where timezone >  date_trunc('day',NOW()) 
		) h
where h.rn = 1 or h.rn = total_count
order by h.humidity desc

-- CURRENT STATUS
select timezone, temperature, humidity
from sensor_data.humidor_decoded
order by timezone desc
limit 1

SELECT t.timezone, t.temperature
FROM 	(select timezone, temperature, row_number() over (order by temperature) as rn,
			count(*) over () as total_count
			from sensor_data.humidor_decoded
		) t
where t.rn = 1 or t.rn = total_count
order by t.temperature desc

-- look at view
Select * 
From sensor_data.humidor_decoded
order by timezone desc;

-- get the dates - DONT MESS WITH THIS
select sensor_name, to_timestamp(cast(entry_time as integer)) at time zone 'America/New_York' 
		, humidity
		, temperature
from sensor_data.humidors 
--where entry_time not like '2020%'
order by entry_time desc;


-- TIME STUFF
select  NOW();

select * 
from sensor_data.humidor_decoded 
where timezone > date_trunc('day',NOW()) 
order by timezone desc;



select * 
from sensor_data.humidors
where entry_time  like '2020%';

DELETE from sensor_data.humidors
WHERE entry_time > NOW();
commit;
select to_char(entry_time at time zone 'America/New_York', 'yyyy-mm-dd hh24:mi:ss')
, MIN(humidity)
from sensor_data.humidors
group by entry_time
order by entry_time desc LIMIT 1
;
SELECT NOW() at time zone 'America/New_York';

ALTER TABLE sensor_data.humidors 
  ALTER entry_time DROP DEFAULT
 ,ALTER entry_time type timestamp USING to_timestamp(cast(entry_time as integer))
 ,ALTER entry_time SET DEFAULT '1970-01-01 01:00:00'::timestamp;

create table sensor_data.test as
select * from sensor_data.humidors
where 1=1;

SELECT MAX(temperature), MIN(temperature) FROM sensor_data.humidors;

SELECT entry_time 
FROM sensor_data.humidors 
order by 1 asc limit 100;

SELECT to_timestamp(cast(entry_time as integer)) at time zone 'America/New_York'
FROM sensor_data.humidors 
where entry_time not like '%UTC%'
order by 1 desc;

SELECT NOW() at time zone 'America/New_York';

select to_timestamp(entry_time, 'yyyy-mm-ddUTChh24miss') at time zone 'EST' as new_time      
FROM sensor_data.humidors 
where entry_time like '%UTC%'
order by 1 desc; limit 500; 

select a.entry_time, extract(epoch from to_timestamp(b.entry_time, 'yyyy-mm-ddUTChh24miss')) as epoch
	from  sensor_data.test a join sensor_data.test b USING(entry_time)
	where a.entry_time like '%-%'
		and b.entry_time like '%-%'
;

UPDATE sensor_data.humidors a
SET entry_time=b.correction
FROM 
	(select 
	   extract(epoch from  sub.stamp) as correction
	from 
	   (select to_timestamp(replace(entry_time, 'UTC', ''),   'yyyy-mm-ddhh24:mi:ss')  stamp 
	   from sensor_data.humidors
	   where entry_time  like '2020%') as sub) as b
;

select extract(epoch from timestamp without time zone entry_time)
from sensor_data.test;

UPDATE sensor_data.humidors a
SET entry_time = c.correction
-- select a.entry_time,c.entry_time,c.correction
FROM 
	(select 
	   entry_time, extract(epoch from  b.stamp) as correction
	from 
	   (select entry_time,to_timestamp(replace(entry_time, 'UTC', ''),   'yyyy-mm-ddhh24:mi:ss')  stamp 
	   from sensor_data.humidors
	   where entry_time  like '2020%') as b ) as c
WHERE a.entry_time = c.entry_time;

select * from sensor_data.humidors order by 1 desc;



SELECT humidity, temperature
FROM sensor_data.humidors
WHERE entry_time = (
  SELECT MAX(entry_time)
  FROM sensor_data.humidors);
  
select sensor_name, to_timestamp(cast(entry_time as integer)) at time zone 'America/New_York' 
		, humidity
		, temperature
from sensor_data.humidors 
--where entry_time not like '2020%'
order by 1 desc;