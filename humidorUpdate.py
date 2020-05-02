import psycopg2
import json
def lambda_handler(event,context):
	print("EVENT DATA")
	print("--------------------")
	msg = json.dumps(event, indent=2)
	print(msg)
	conn = psycopg2.connect(host="humidorpg.cwavdpplkanz.us-west-2.rds.amazonaws.com",
		database="humidor_data",
		user="postgres",
		password="db_pass",
		port="5432"
		)
	cursor = conn.cursor()
	cursor.execute("INSERT INTO sensor_data.humidors (sensor_name, entry_time, humidity, temperature ) VALUES (%s, %s, %s, %s )",( event['sensorName'], event['timestamp'], event['humidity'], event['temperature'] ))
	conn.commit()
	cursor.close()
	print("working")