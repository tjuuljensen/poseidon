<?php

	include('php/labels.php');
	include('php/squares.php'); 
	include('php/dbconnect.php');
	include('php/poseidon.php'); 
	include('php/general.php'); 
		

$thigh=$_POST['thigh']; 
$warnhigh=$_POST['warnhigh']; 
$warnlow=$_POST['warnlow']; 
$tlow=$_POST['tlow']; 
$sensor_id=$_POST['sensor_id']; 

if (empty($thigh)) $thigh= "NULL";
if (empty($warnhigh)) $warnhigh= "NULL";
if (empty($warnlow)) $warnlow= "NULL";
if (empty($tlow)) $tlow= "NULL";

switch ($sensor_id) {
	case $_SESSION['MyFlowSensor']:
		$data = "UPDATE sensor SET thresholdlow=".$tlow.", warnlow=".$warnlow." WHERE id=".$sensor_id.";";
		break;
	case $_SESSION['MyLightSensor']:
		$data = "UPDATE sensor SET thresholdlow=".$tlow.", warnlow=".$warnlow." WHERE id=".$sensor_id.";";
		break;
	default:
		$data = "UPDATE sensor SET thresholdhigh=".$thigh.", thresholdlow=".$tlow.", warnhigh=".$warnhigh.", warnlow=".$warnlow." WHERE id=".$sensor_id.";";
		break;
}


//echo $data; 
$query = mysql_query($data) or die("Couldn't execute query. ". mysql_error()); 
//echo $query;
//echo $data."</br>"; //DEBUG
//echo CurrentPageURL()."</br>"; //DEBUG

$referring_url = right(CurrentPageURL(),"url="); //$_GET does not work, cause it's the FULL url we want incl. PHP page variables<½qq
header("Location: ".$referring_url);
//echo $referring_url;
	
?>
