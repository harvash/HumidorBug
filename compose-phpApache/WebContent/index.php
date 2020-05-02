<!DOCTYPE html>
<!-- Template by Quackit.com -->
<html lang="en">
<head>

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->

    <title>Humidor Web</title>

    <!-- Bootstrap Core CSS -->
     <link href="css/bootstrap.min.css" rel="stylesheet"> 

    <!-- Custom CSS: You can use this stylesheet to override any Bootstrap styles and/or apply your own styles -->
    <link href="css/custom.css" rel="stylesheet">

    <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
        <script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
        <script src="https://oss.maxcdn.com/libs/respond.js/1.4.2/respond.min.js"></script>
    <![endif]-->
	  	<script src="https://cdn.anychart.com/releases/8.7.1/js/anychart-core.min.js"></script>
		<script src="https://cdn.anychart.com/releases/8.7.1/js/anychart-bundle.min.js"></script>
		<script src="https://cdn.anychart.com/releases/8.7.1/js/anychart-base.min.js"></script>
	<!-- Include the data adapter -->
		<script src="https://cdn.anychart.com/releases/8.7.1/js/anychart-data-adapter.min.js"></script>
		
</head>
<?php
    function requireToVar($file){
        ob_start();
        require($file);
        return ob_get_clean();
    }
    $query=requireToVar('readings.php');
    ?>
<html><body>    
	<div>
		<H1> Welcome to Humidor Web  </H1> 
	</div>
			<div id="container"></div>
	<script>
				var readings = <?php echo $query ?>;
				anychart.onDocumentReady(function () {
			    
					// create data set on our data
			    var dataIn = anychart.data.set(readings);
				var temp = dataIn.mapAs({value:"temperature"});
			     var humid = dataIn.mapAs({value:"humidity"});
				var output = temp.concat(humid);
					// set the gauge type
			    var gauge = anychart.gauges.circular();

					// gauge settings
			    gauge.data(output);
			    gauge.padding("1%");
			    gauge.startAngle(270);
			    gauge.sweepAngle(180);
			    gauge.fill(["Tan"], .5, .5, null, 1, 0.5, 0.9);
			   
					// axis settings
			    var axis = gauge.axis()
			.radius(95)
			.width(0);

					// scale settings
			    axis.scale()
			.minimum(50)
			.maximum(100)
			.ticks({interval: 10})
			.minorTicks({interval: 1});

					// ticks settings
			    axis.ticks()
			.type("trapezium")
			.fill("blue")
			.length(9);

					// minor ticks settings
			    axis.minorTicks()
			.enabled(true)
			.fill("black")
			.length(1.5);

			// labels settings
			    axis.labels()
			.fontSize("16px")
			.fontColor("Blue");

			// second axis settings
			    var axis_1 = gauge.axis(1)
			.radius(50)
			.width(0);

					// second scale settings
			    axis_1.scale()
			.minimum(50)
			.maximum(100)
			.ticks({interval: 5})
			.minorTicks({interval: 1});

					// second ticks settings
			    axis_1.ticks()
			.type("trapezium")
			.length(15)
			.fill('red');


					// second minor ticks settings
			    axis_1.minorTicks()
			.enabled(true)
			.length(5)
			.fill('black');

			// labels 2 settings
			    axis_1.labels()
			.fontSize("12px")
			.fontColor("Red");

			// needle 1
			    gauge.needle(0)
			 .dataIndex(1)
			.enabled(true)
			.startRadius("65%")
			.endRadius("100%")
			.middleRadius(0)
			.startWidth("1%")
			.endWidth("1%")
			.fill("white")
			.stroke("none")
			.middleWidth("1%");

			// needle 2
			    gauge.needle(1)
			.dataIndex(0)
			.enabled(true)
			.startRadius("0%")
			.endRadius("40%")
			.middleRadius(0)
			.startWidth("1%")
			.endWidth("1%")
			.fill("white")
			.stroke("none")
			.middleWidth("1%");
			    


			    // gap
			    gauge.cap()
			.radius("3%");

			    // gauge label 0
			    gauge.label(0)
			.text("Temp(F)")
			.anchor("center") // set the position of the label
			.adjustFontSize(true)
			.hAlign("center")
			.offsetY("15%")
			.offsetX("50%")
			.width("50%")
			.height("10%")
			.zIndex(10)
			 .fontColor('Brown')
			.fontFamily('Bookman');

				 // gauge label 1
			    gauge.label(1)
			.text("Humid(%)")
			.anchor("center") // set the position of the label
			.adjustFontSize(true)
			.hAlign("center")
			.offsetY("60%")
			.offsetX("50%")
			.width("50%")
			.height("10%")
			.zIndex(10)
			.fontColor('Blue')
			.fontFamily('Bookman');


			    // range
			    gauge.range({
				 from: 0,
				 to: 120,
				 fill: {keys: ["red", "green", "yellow","yellow","red","red"]},
				 position: "inside",
				 radius: 100,
				 endSize: "3%",
				 startSize:"3%",
				 zIndex: 10
			  });

					// draw the chart
			    gauge.container("container").draw();
			});
	</script>
</body></html>
