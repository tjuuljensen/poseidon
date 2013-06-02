<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<?php 
	session_start(); 
	include('php/labels.php');
	include('php/squares.php'); 
	include('php/dbconnect.php');
	include('php/poseidon.php'); 
	include('php/general.php'); 
		
	function layout($page_id) {

	initSessionVars();
	
	echo "<script src=\"js/jquery.min.js\"></script>";

	switch($page_id) {
        default: //Default, ie when the page_id does not match with predefined cases
            //echo '<p class="red">The page was not found. Showing Home page instead</p>';
        case '': //When it is null
        case 'home':
			echo drwheader(label("1"),"home");
			echo "<script type=\"text/javascript\">";
				echo "$(\"#backid\").hide();";
			echo "</script>";
			drwsq_sensor(sensorid("ph"),"ph");
			drwsq_sensor(sensorid("co2"),"co2");
			drwsq_sensor(sensorid("temp"),"temp");
			drwsq_sensor(sensorid("flow"),"flow");
			drwlsq_jpg("tank","My Planted Tank","photo/2013-04-22 22.16.22.jpg","");
			drwsq_sensor(sensorid("light"),"light");
			drwsq_ldht("light1","Light #1",label("16"),"","<p>#1 off at: ".StopTime($_SESSION['MyLight1Output']),ReturnColor(OutletColorNum($_SESSION['MyLight1Output'])));
			echo drwsq_ldht("light2","Light #2",label("17"),"","<p>#2 off at: ".StopTime($_SESSION['MyLight2Output']),ReturnColor(OutletColorNum($_SESSION['MyLight2Output'])));
			drwfooter();
            break;
        case 'ph':
			echo drwheader(label("2"),"home");
			echo drwsq_sensor(sensorid("ph"),"ph",2);
			//echo drwsq_l("ph","pH",label("2"),number_format(LatestSensorReading($_SESSION['MyPhSensor']),1));
			echo drwsq_threshold("ph","threshold");
			echo drwsq_lt("calibrate","Calibrate","","</br>".label("18"));
			echo drwsq_lt("statistics","Statistic","",label("14"));
			echo drwsq_lt("timer","Timer","",label("20"));
					
			//convert this into function
			//Start here *************
			echo "<div id=\"onoff\" class=\"infosquare\">";
			echo "<div id=\"onoffdot\" class=\"dot\" style=\"top:15px;left:165px;\"></div>";
			echo "<div id=\"onofvalue\" class=\"squarevalue_small\" style=\"top:85px; left:45px; width:115px;\">".label("13")."</div>";
			echo "</div>";
			//End here *************
			
			echo drwsq("Empty1");
			echo drwsq("Empty2");
			echo drwsq("Empty3");
			echo drwfooter();
            break;
        case 'co2':
			echo drwheader(label("3"),"home");
			echo drwsq_sensor(sensorid("co2"),"co2",2);
			
			
			//echo drwsq_l("co2","CO2",label("3"),"0.0");
			echo drwsq_threshold("co2","threshold");
			echo drwsq_lt("calibrate","Calibrate","",label("18"));
			echo drwsq_lt("co2control","CO2 control","",label("19"));
			echo drwsq_lt("statistics","Statistic","",label("14"));
			echo drwsq_lt("timer","Timer","",label("20"));
			echo drwsq("Empty2");
			echo drwsq("Empty3");
			echo drwsq("Empty4");
			echo drwfooter();
            break;
	  case 'temp':
			echo drwheader(label("4"),"home");
			drwsq_sensor(sensorid("temp"),"temp",2);
			//echo drwsq_l("temp","Temperature",label("4"),"0.0");
			echo drwsq_threshold("temp","threshold");
			echo drwsq_lt("calibratetemp","Calibrate","",label("31"));
			echo drwsq_lt("tempcontrol","Temperature control","",label("32"));
			echo drwsq_lt("statistics","Statistic","",label("14"));
			echo drwsq_lt("heaterpower","Heater","",label("33"));
			echo drwsq("Empty2");
			echo drwsq("Empty3");
			echo drwsq("Empty4");
			echo drwfooter();
            break;
		case 'light':
			echo drwheader(label("5"),"home");
			drwsq_sensor(sensorid("light"),"light",2);
			echo drwsq_threshold("light","threshold");
			echo drwsq_lt("calibratelight","Calibrate","",label("38"));
			echo drwsq_lt("lightcontrol","Light control","",label("39"));
			echo drwsq_lt("statistics","Statistic","",label("14"));
			echo drwsq("Empty1");
			echo drwsq("Empty2");
			echo drwsq("Empty3");
			echo drwsq("Empty4");
			echo drwfooter();
            break;
	  case 'flow':
			echo drwheader(label("6"),"home");
			drwsq_sensor(sensorid("flow"),"flow",2);
			//echo drwsq_l("temp","Temperature",label("4"),"0.0");
			echo drwsq_threshold("flow","threshold");
			echo drwsq_lt("calibrateflow","Calibrate","",label("40"));
			echo drwsq_lt("flowcontrol","flow control","",label("41"));
			echo drwsq_lt("statistics","Statistic","",label("14"));
			echo drwsq("Empty1");
			echo drwsq("Empty2");
			echo drwsq("Empty3");
			echo drwsq("Empty4");
			echo drwfooter();
            break;
		case 'pwr1':
			echo drwheader(label("21"),"home");
			echo drwlsq_pwr("off","Turn off","red","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=1&override=1&value=0");
			echo drwsq("Empty0");
			echo drwlsq_pwr("on","Turn on","yellow","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=1&override=1&value=1");
			echo drwsq("Empty1");
			echo drwlsq_pwr("auto","Automatic","green","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=1&override=0");
			echo drwsq("Empty2");
			echo drwfooter(1);
            break;
		case 'pwr2':
			echo drwheader(label("22"),"home");
			echo drwlsq_pwr("off","Turn off","red","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=2&override=1&value=0");
			echo drwsq("Empty0");
			echo drwlsq_pwr("on","Turn on","yellow","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=2&override=1&value=1");
			echo drwsq("Empty1");
			echo drwlsq_pwr("auto","Automatic","green","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=2&override=0");
			echo drwsq("Empty2");
			echo drwfooter(2);
            break;
		case 'pwr3':
			echo drwheader(label("23"),"home");
			echo drwlsq_pwr("off","Turn off","red","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=3&override=1&value=0");
			echo drwsq("Empty0");
			echo drwlsq_pwr("on","Turn on","yellow","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=3&override=1&value=1");
			echo drwsq("Empty1");
			echo drwlsq_pwr("auto","Automatic","green","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=3&override=0");
			echo drwsq("Empty2");
			echo drwfooter(3);
            break;
		case 'pwr4':
			echo drwheader(label("24"),"home");
			echo drwlsq_pwr("off","Turn off","red","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=4&override=1&value=0");
			echo drwsq("Empty0");
			echo drwlsq_pwr("on","Turn on","yellow","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=4&override=1&value=1");
			echo drwsq("Empty1");
			echo drwlsq_pwr("auto","Automatic","green","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=4&override=0");
			echo drwsq("Empty2");
			echo drwfooter(4);
            break;
		case 'pwr5':
			echo drwheader(label("25"),"home");
			echo drwlsq_pwr("off","Turn off","red","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=5&override=1&value=0");
			echo drwsq("Empty0");
			echo drwlsq_pwr("on","Turn on","yellow","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=5&override=1&value=1");
			echo drwsq("Empty1");
			echo drwlsq_pwr("auto","Automatic","green","pwrcontrol.php?url=".left(currentPageURL(),"index.php?")."&outlet=5&override=0");
			echo drwsq("Empty2");
			echo drwfooter(5);
            break;	
		case 'threshold':
			$sensor_id = $_GET['sensor'];
			switch($sensor_id) {
				case 1:
					$label_id=5; //light
					$page="light";
				break;
				case 2:
					$label_id=6; //flow 
					$page="flow";
				break;
				case 3:
					$label_id=4; //Temp
					$page="temp";
				break;
				case 4:
					$label_id=2; //pH
					$page="ph";
				break;
				case 5:
					$label_id=3; //co2
					$page="co2";
				break;															
			}
						
			echo drwheader(label($label_id),$page);
			echo drwsq_threshold_input($sensor_id);
			echo drwfooter();
		break;
    }
}
?>


<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title><?php echo label("1")?></title>
<link href="css/sitestyle.css" rel="stylesheet" type="text/css" />
</head>

<body>

<?php
$page_id = $_GET['page']; //Get the request URL
layout($page_id); //Call the function with the argument
?>

</body>
</html>
