<?PHP 

function curPageName() {
	 //returns e.g.: index.php
	 return substr($_SERVER["SCRIPT_NAME"],strrpos($_SERVER["SCRIPT_NAME"],"/")+1);
}

function currentPageURL() {
//	 returns complete content of the address bar
	 $pageURL = 'http';
	 if ($_SERVER["HTTPS"] == "on") {$pageURL .= "s";}
	 $pageURL .= "://";
	 if ($_SERVER["SERVER_PORT"] != "80") {
	  $pageURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"].$_SERVER["REQUEST_URI"];
	 } else {
	  $pageURL .= $_SERVER["SERVER_NAME"].$_SERVER["REQUEST_URI"];
	 }
	 return $pageURL;
}

function currentSiteURL(){
 $siteURL = 'http';
 if ($_SERVER["HTTPS"] == "on") {$siteURL .= "s";}
 $siteURL .= "://";
 if ($_SERVER["SERVER_PORT"] != "80") {
  $siteURL .= $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"];
 } else {
  $siteURL .= $_SERVER["SERVER_NAME"];
 }
 return $siteURL;
}

function left($inputstr, $searchstr) {
	if (substr_count($inputstr, $searchstr)==0) {
	return $inputstr;} 
	else{ 
	return substr($inputstr,0,strpos($inputstr,$searchstr));
	}
}

function right($inputstr, $searchstr) {
	if (substr_count($inputstr, $searchstr)===0) {
		return $inputstr;} 
		else{ 
		$strlength=strlen($inputstr)-strlen($searchstr);
		$startpos=strpos($inputstr,$searchstr)+strlen($searchstr);
		return substr($inputstr,$startpos, $strlength);
		}
}

function ndisplay($number,$decimals=2) {
	if (!empty($number)) { 
		return number_format($number, $decimals,$_SESSION['DecimalSeparator'],$_SESSION['ThousandSeparator']);
		} else {
		return 0;}
	}

function nformat($number,$decimals=2) {
	if (!empty($number)) {
		return number_format(strtr($number, $_SESSION['DecimalSeparator'].$_SESSION['ThousandSeparator'], ".,"),$decimals);
		} 
		else {
		return 0;}
	}

function cformat($number,$decimals=2) {
	if (!empty($number)) {
		return number_format(strtr($number, $_SESSION['DecimalSeparator'].$_SESSION['ThousandSeparator'], ".,"),$decimals);
		} 
		else {
		return null;}
	}

?>