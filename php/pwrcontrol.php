<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Power Control</title>
</head>

<body>

<?PHP 
	session_start(); 
//	include('php/labels.php');
//	include('php/squares.php'); 
	include('php/dbconnect.php');
	include('php/poseidon.php'); 
	include('php/general.php'); 

	$outlet_id = $_GET['outlet']; //Get the request URL
	$override = $_GET['override']; //Get the request URL
	$value = $_GET['value']; //Get the request URL
	$referring_url = $_GET['url']; //Get the request URL
	
	//set outlet
	if (isset($value)) {
		echo SetOutletManual( $outlet_id, $override, $value);
	} else
	{	echo SetOverrideOff( $outlet_id, $override);
	}
	
	//reload web page
	header("Location: ".$referring_url);
	

?>

</body>
</html>