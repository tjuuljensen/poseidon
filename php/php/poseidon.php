<?php     
	
function initSessionVars() {
	$_SESSION['MyTank'] = 1;
	$_SESSION['MyLightSensor'] = 1;
	$_SESSION['MyFlowSensor'] = 2;
	$_SESSION['MyTempSensor'] = 3;
	$_SESSION['MyPhSensor'] = 4;
	$_SESSION['MyCO2Sensor'] = 5;

	$_SESSION['MyLight1Output'] = 1;
	$_SESSION['MyLight2Output'] = 2;
	
	$_SESSION['RefreshIntervalSensors'] = 3;
	$_SESSION['RefreshIntervalOutlets'] = 3;

	$_SESSION['MyLight1Output'] = 1;
	$_SESSION['MyLight2Output'] = 2;

	$_SESSION['DecimalSeparator'] = ".";
	$_SESSION['ThousandSeparator'] = ",";
	//$_SESSION['Decimals'] = 2;


	}

function LatestSensorReading($intInputSensor) {
	
	$sql = "SELECT calculated, ts FROM sensorreading WHERE sensor_id = ".$intInputSensor." ORDER BY id DESC LIMIT 0,1;";
	$rs = mysql_query($sql);
	$data = mysql_fetch_array($rs);
	return $data["calculated"];

}
	
function StopTime($outlet_id){
	
	$sql = "SELECT starttime, stoptime FROM outlettimer WHERE outlet_id = " . $outlet_id . ";";
	$rs = mysql_query($sql);
	$data = mysql_fetch_array($rs);
	return date("H:i",strtotime($data["stoptime"]));
}
	
function StartTime($outlet_id){
	
	$sql = "SELECT starttime, stoptime FROM outlettimer WHERE outlet_id = " . $outlet_id . ";";
	$rs = mysql_query($sql);
	$data = mysql_fetch_array($rs);
	return date("H:i",strtotime($data["starttime"]));
}

function ThresholdHigh($sensor_id){
	
	$sql = "SELECT thresholdhigh, thresholdlow FROM sensor WHERE id = " . $sensor_id . ";";
	$rs = mysql_query($sql);
	$data = mysql_fetch_array($rs);
	return $data["thresholdhigh"];
}

function ThresholdLow($sensor_id){
	
	$sql = "SELECT thresholdhigh, thresholdlow FROM sensor WHERE id = " . $sensor_id . ";";
	$rs = mysql_query($sql);
	$data = mysql_fetch_array($rs);
	return $data["thresholdlow"];
}

function OutletStatus($intOutletId){
	$sql = "SELECT name, value FROM outlet WHERE id = " . $intOutletId . ";";
	$rs = mysql_query($sql);
	$data = mysql_fetch_array($rs);
	return $data["value"];
	
}

function SetOutletManual($intOutletId, $override, $value){
	$sql = "UPDATE outlet SET manualoverride=".$override.", value=".$value." WHERE id = " . $intOutletId . ";";
	$rs = mysql_query($sql);
	//return mysql error handler?
	
}

function SetOverrideOff($intOutletId, $override){
	$sql = "UPDATE outlet SET manualoverride=".$override." WHERE id = " . $intOutletId . ";";
	$rs = mysql_query($sql);
	//return mysql error handler?
	
}
function SensorEnabled($sensortype){

	$sql = "SELECT active FROM sensor WHERE sensortype = '" . $sensortype . "';";
	$rs = mysql_query($sql);
	$data = mysql_fetch_array($rs);
	return $data["active"];
}

function OutletColorNum($outlet_id){
	
	$sql = "SELECT manualoverride, value FROM outlet WHERE id = " . $outlet_id . ";";
	$rs = mysql_query($sql);
	$data = mysql_fetch_array($rs);
	
	switch ($data["manualoverride"]) {
    case 0:
        if ($data["value"] == 0) {
		//Scheduled off, outletcolor="grey";
		return 1;
		}
		else
 		{//Scheduled on, outletcolor="green";	
		return 2;
		}
    case 1:
        if ($data["value"] == 1) {
		//ManualOverride ON, outletcolor="yellow";
		return 3;
		}
		else
 		{//ManualOverride OFF, outletcolor="red";	
		return 4;
		}
        break;
	}

}

function ReturnColor($InputValue) {
	switch ($InputValue) {
		case 1:
			return "grey";
		case 2:
			return "green";	
		case 3:
			return "yellow";	
		case 4:
			return "red";
			
		}
}

function SensorDotColor($sensortype, $value){
	
	$sql = "SELECT * FROM sensor WHERE sensortype='" . $sensortype . "';";
	$rs = mysql_query($sql);
	$data = mysql_fetch_array($rs);
//	echo $data["id"];

			$maxvalue=999;
			$minvalue=0;
			
			//set alerthigh
			if (!isset($data["thresholdhigh"])) {
				$alerthigh = $maxvalue	;
			} else {
				$alerthigh=$data["thresholdhigh"];
			}
			//set warnhigh
			if (!isset($data["warnhigh"])) {
				$warnhigh = $alerthigh	;
			} else {
				$warnhigh=$data["warnhigh"];
			}
			//set alertlow
			if (!isset($data["thresholdlow"])) {
				$alertlow = $minvalue	;
			} else {
				$alertlow=$data["thresholdlow"];
			}
			//set warnlow
			if (!isset($data["warnlow"])) {
				$warnlow = $alertlow	;
			} else {
				$warnlow=$data["warnlow"];
			}

	
	switch($sensortype) {
		case "co2":
			if ($value>$alertlow and $value<$warnhigh) {
				return "green"; 
			} elseif ($value>=$alerthigh) {
				return "red";
			} else {
				return "yellow"; //  (($value>$alertlow and $value<=$warnlow) or  ($value<$alerthigh and $value>=$warnhigh)) 
			}
			//			return "grey";
			break;
		case "light":

			$light1color=OutletColorNum($_SESSION['MyLight1Output']);
			$light2color=OutletColorNum($_SESSION['MyLight2Output']);
			
			if (($light1color==1 or $light2color==1) or ($light1color==4 or $light2color==4)) {//gray or red
				return "grey";
			} elseif($value>$warnlow and $value<$warnhigh) {
				return "green"; 
			} elseif ($value<=$alertlow or $value>=$alerthigh) {
				return "red";
			} else {
				return "yellow"; //  (($value>$alertlow and $value<=$warnlow) or  ($value<$alerthigh and $value>=$warnhigh)) 
			}
			//			return "grey";
			break;
		default:
			if ($value>$warnlow and $value<$warnhigh) {
				return "green"; 
			} elseif ($value<=$alertlow or $value>=$alerthigh) {
				return "red";
			} else {
				return "yellow"; //  (($value>$alertlow and $value<=$warnlow) or  ($value<$alerthigh and $value>=$warnhigh)) 
			}
		break;
	}

	return "blue";
}

function sensorid($sensortype) {
	$sql = "SELECT id FROM sensor WHERE sensortype = '" . $sensortype . "';";
	$rs = mysql_query($sql);
	$data = mysql_fetch_array($rs);
	return $data["id"];
}
?>