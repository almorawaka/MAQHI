


<!DOCTYPE html>
<html>

   <head>
     <title>MAQHI</title>
   </head>
	

</html>
<?php
	include("config.php");
	$sql = "SELECT * FROM maqhi_table ORDER BY time DESC";
	$trn_result = mysqli_query($db, $sql);
	if (!$trn_result) {
		die("Map Database failed! Please contact admin.");
	}
?>

<html>

<head>
	<title>MAQHI</title>
	<meta name="description" content="fpa vendom" />
	<meta name="keywords" content="fpa, vendom" />
	<meta http-equiv="content-type" content="text/html; charset=windows-1252" />
	<link rel="stylesheet" type="text/css" href="style/style.css" title="style" />
	<link rel="stylesheet" type="text/css" href="style/custom-style.css" title="style" />
</head>

<body>
	<div id="main">
	<div id="header">
	  <div id="logo">
		<div id="logo_text">
		  <!-- class="logo_colour", allows you to change the colour of the text -->
		  <h1><a href="home.php">MODERATE AIR  QUALITY HEALTH INDEX <span class="logo_colour">(MAQHI)</span></a></h1>
		  <h2>Before you breathe, Check MAQHI around you …!</h2>
		</div>
	  </div>
	  <div id="menubar">
		<ul id="menu">
		  <!-- put class="selected" in the li tag for the selected page - to highlight which page you're on -->
		  <li class="selected"><a href="index.php">Home</a></li>
		  <li><a href= "g_chart/index.php">Charts</a></li>
		  <li><a href="location.php">location</a></li>
		  <li ><a href="table.php">Table</a></li>
		</ul>
	  </div>
	</div>
	<div id="content_header"></div>
	<div id="site_content">
		<h1 class="purpose_header">Purpose of MAQHI </h1>
			<p>We should aware of surrounding air pollutant before they kill us! Particulate matter (PM2.5), particulate matter (PM10), Carbon monoxide (CO), Ammonia (NH3) and Carbon dioxide (CO2) are measured with respect to the geographical locations along with Temperature and Humidity to analyze the MAQHI.</p>
			
				<h1 class="sub_header"> <li> <a href="https://thingspeak.com/channels/1268567" target="_blank"> Colombo - MAQHI </a> </li> </h1>
				<iframe width="450" height="260" style="border: 0px solid #cccccc;" src="https://thingspeak.com/channels/1268567/widgets/271644"></iframe>
				<iframe width="250" height="260" style="border: 0px solid #cccccc;" src="https://thingspeak.com/channels/1268567/widgets/274418"></iframe>
				<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1268567/charts/8?average=10&bgcolor=%23c8f8cc&color=%2320d62f&dynamic=true&results=60&title=MODIFIED+AIR+QUALITY+HEALTH+INDEX&type=spline&xaxis=time&yaxis=MAQHI"></iframe>
				
				<h1 class="sub_header"> <li><a href="https://thingspeak.com/channels/1312697" target="_blank"> Kandy - MAQHI </a> </li> </h1> 
				<iframe width="450" height="260" style="border: 0px solid #cccccc;" src="https://thingspeak.com/channels/1312697/widgets/275652"></iframe>
				<iframe width="250" height="260" style="border: 0px solid #cccccc;" src="https://thingspeak.com/channels/1312697/widgets/275650"></iframe>
				<iframe width="450" height="260" style="border: 1px solid #cccccc;" src="https://thingspeak.com/channels/1312697/charts/8?average=10&bgcolor=%23c8f8cc&color=%2320d62f&dynamic=true&results=60&title=MODIFIED+AIR+QUALITY+HEALTH+INDEX&type=spline&xaxis=time&yaxis=MAQHI"></iframe>
				
			<h1 class="purpose_header">MAQHI colour code</h1>
			<img src = "image/maqhi.png" alt = "Test Image" width = "450" height = "300"/>	
			
		<h1>Good </h1>	
		<p>Air quality is considered satisfactory, and air pollution poses little or no risk</p>
		<h1>Moderate </h1>	
		<p>Air quality is acceptable; however, for some pollutants there may be a moderate health concern for a very small number of people who are unusually sensitive to air pollution. Active children and adults, and people with respiratory disease, such as asthma, should limit prolonged outdoor exertion</p>
		<h1>Unhealthy for Sensitive Group </h1>	
		<p>Members of sensitive groups may experience health effects. The general public is not likely to be affected. Active children and adults, and people with respiratory disease, such as asthma, should avoid prolonged outdoor exertion</p>
		<h1>Unhealthy </h1>	
		<p>Everyone may begin to experience health effects, members of sensitive groups may experience more serious health effects, Active children and adults, and people with respiratory disease, such as asthma, should avoid all outdoor exertion.</p>
		<h1>Very Unhealthy </h1>	
		<p>Health alert: everyone may experience more serious health effects, everyone should avoid all outdoor exertion</p>
		
	
	<h1>Read more </h1>	
		<h5> <li><a href="maqhi.html">CARBON MONOXIDE</a></li> </h5>	
		<h5> <li><a href="maqhi.html">PARTICULATE MATTERS</a></li> </h5>	
		<h5> <li><a href="maqhi.html">MQ 135 AIR SENSOR</a></li> </h5>	
		<h5> <li><a href="maqhi.html">TEMPERATURE AND HUMIDITY</a></li> </h5>		

   
	</div>
		<div id="content_footer"></div>
			<div id="footer">

			<a href="https://twitter.com/MAQHI3"> Receive MAQHI Live Update on twitter</a> 
	</div>
	
	<div id="content_footer"></div>
		<div id="footer">
			<a href = "mailto: almorawaka@gmail.com">Send Email</a>
			Copyright &copy; Asanka | 2021
		</div>
	</div>
	
</body>
</html>









