<?PHP
	
	//Naming convention for functions
	//Example: drwsq_ldl("ph","pH",label("2"),"ph.php")
	//drwsq:draw square
	//l: label, d: dot, l: link, u: unit of measure, t: text

function drwsq_ldht($square_id,$squaretitle,$label,$link,$text,$dotcolor) {	
	
	//Define derived variables
	$label_id=$square_id."_label";
	$dot_id=$square_id."_dot";
	$value_id=$square_id."_value";
	
	//define CSS variables
	$css_square="infosquare";
	if ($dotcolor!=""){$css_dot="dot_".$dotcolor;} else {$css_dot="dot";}
	$css_value="squarevalue_small";
	$css_label="squarelabel";

	
	echo "<div id=\"".$square_id."\" class=\"".$css_square."\" title=\"".$squaretitle."\" onclick=\"window.location='".$link."';\">";
	echo "<div id=\"".$label_id."\" class=\"".$css_label."\" style=\"width:30px;top:15px;left:30px;\">".$label."</div>";
	echo "<div id=\"".$dot_id."\" class=\"".$css_dot."\" style=\"top:15px;left:165px;\"></div>";
	echo "<div id=\"".$value_id."\" class=\"".$css_value."\" style=\"top:65px; left:19px; width:171px;\">".$text;
	echo "</div>";
	echo "</div>";
}

function drwheader($headertitle, $page) {
	$headersq="headersquare";
	$back="backid";
	$css_back="backbutton";
	$css_header="headersquare_3";
	$css_frame="outerframe";
	$css_headersq="pageframe_3";
	
	$link="index.php?page=".$page;			
	
	echo "<div id=\"".$css_frame."\" class=\"".$css_headersq."\">";
	echo "<div id=\"".$headersq."\" class=\"".$css_header."\">";
	echo "<div id=\"".$headertitle."\" style=\"position: absolute; top:28px; left:28px; width:551px; height:64px;\">".$headertitle."</div>";
	echo "<div id=\"".$back."\" class=\"".$css_back."\" onclick=\"window.location='".$link."';\"></div>";
	echo "</div>";	

}

function drwfooter($active_outlet=0) {
	
	$container_id="footer_container";
	$css_footer="footer_container";
	
	echo "<div id=\"".$container_id."\" class=\"".$css_footer."\">";
	
	drwfooter_outlets($active_outlet);

	echo "</div>";

	//refresh using jQuery
	echo "<script type=\"text/javascript\">";
	echo "var auto_refresh = setInterval(";
	echo "function () {";
	echo "$('#".$container_id."').load(\"divoutletcontent.php?outlet=".$active_outlet."\").fadein(\"slow\");  },";
	echo $_SESSION['RefreshIntervalOutlets']*1000;
	echo ");";
	echo "</script>";

}

function drwfooter_outlets($active_outlet=0) {
	$footersq="outletfooter";
	$css_footer="footer_3";
	
	echo "<div id=\"".$footersq."\" class=\"".$css_footer."\">";
	
	for ($i = 1; $i <= 5; $i++) {
		if ($active_outlet==0) {$active=true;} else {$active=($i==$active_outlet);};
    	echo drwoutlet(strval($i),label(strval(20+$i)),"index.php?page=pwr".strval($i),$active);
	}
	echo "</div>";
	
}

function drwoutlet($outlet_id,$outlet_name, $link, $active){
	
	//Should have check for outlet_id values >=1 and <=5
	$css_outlet="outlet";
	
	if ($active==true) {
	
	switch (OutletColorNum($outlet_id)) {
    case 1:
		$css_outlet="outlet_grey";
		$outlet_name=$outlet_name." - ".label("27");
		break;
	case 2:
		$css_outlet="outlet_green";	
		$outlet_name=$outlet_name." - ".label("26");
		break;
    case 3:
		$css_outlet="outlet_yellow";	
		$outlet_name=$outlet_name." - ".label("28");
		break;
	case 4:
		$css_outlet="outlet_red";
		$outlet_name=$outlet_name." - ".label("29");
		break;
	}
	}
	
	//DIV gets named "outlet1" for outlet_id 1
	echo "<div id=\"outlet".$outlet_id."\" title=\"".$outlet_name."\" class=\"".$css_outlet."\" onclick=\"window.location='".$link."';\"></div>";
	
	//<img src=\"".$outlet_img."\" width=\"76\" height=\"76\" alt=\"".$outlet_id."\" />
}




function drwsq_sensor($sensor_id,$square_id, $sqstyle=1) {
	
	$container_id=$square_id."_container";
	$css_container="infosquare_container";
	
	$css_square="infosquare";
	$squaretitle=$square_id;
	
	echo "<div id=\"".$container_id."\" class=\"".$css_container."\";\">";
		drw_sensorcontent($sensor_id, $square_id, $sqstyle);
	echo "</div>";
	
	//refresh using jQuery
	echo "<script type=\"text/javascript\">";
	echo "var auto_refresh = setInterval(";
	echo "function () {";
	echo "$('#".$container_id."').load(\"divcontent.php?sensor=".$sensor_id."&square=".$square_id."&style=".$sqstyle."\").fadein(\"slow\");  },";
	echo $_SESSION['RefreshIntervalSensors']*1000;
	echo ");";
	echo "</script>";
}

function drw_sensorcontent($sensor_id, $square_id, $sqstyle=1)  {

	$css_dot="dot";
	
	switch ($square_id) {
		case "ph":
			$squaretitle="pH";
			$label=label("2");
			$link="index.php?page=ph";
			$active=SensorEnabled("ph");
			$decimal=1;
			$value=ndisplay(LatestSensorReading($_SESSION['MyPhSensor']),$decimal);
			//$css_dot = "dot_".SensorDotColor($square_id, $value);
		break;
		case "co2":
			$squaretitle="CO2";
			$label=label("3");
			$uom=label("7");
			$link="index.php?page=co2";
			$active=SensorEnabled("co2");
			$decimal=1;
			$value=ndisplay(LatestSensorReading($_SESSION['MyCO2Sensor']),$decimal);
			//15,664*kH*10^(6,36-pH)
			
		break;
		case "temp":
			$squaretitle="temp";
			$label=label("4");
			$uom=label("8");
			$link="index.php?page=temp";
			$active=SensorEnabled("temp");
			$decimal=1;
			$value=ndisplay(LatestSensorReading($_SESSION['MyTempSensor']),$decimal);
		break;
		case "flow":
			$squaretitle="Flow";
			$label=label("6");
			$uom=label("9");
			$link="index.php?page=flow";
			$active=SensorEnabled("flow");
			$decimal=0;
			$value=ndisplay(LatestSensorReading($_SESSION['MyFlowSensor']),$decimal);
		break;
		case "light":
			$squaretitle="light";
			$label=label("5");
			$uom=label("9");
			$link="index.php?page=light";
			$active=SensorEnabled("light");
			$decimal=0;
			$value=ndisplay(LatestSensorReading($_SESSION['MyLightSensor']),$decimal);
		break;								
	}
	
	$css_dot = "dot_".SensorDotColor($square_id, $value);
	
	//Define derived variables
	$label_id=$square_id."_label";
	$dot_id=$square_id."_dot";
	$value_id=$square_id."_value";

	//define CSS variables
	if ($active==1) {$css_square="infosquare";} else {$css_square="infosquare_inactive";$sqstyle=0;} //when inactive
	$css_value="squarevalue";
	$css_label="squarelabel";

	echo "<div id=\"".$square_id."\" class=\"".$css_square."\" title=\"".$squaretitle."\" onclick=\"window.location='".$link."';\">";
	echo "<div id=\"".$label_id."\" class=\"".$css_label."\" style=\"width:30px;top:15px;left:30px;\">".$label."</div>";
	
	//If INACTIVE infosquare - dot, value and uom should be hidden
	echo "<div id=\"".$dot_id."\" class=\"".$css_dot."\" style=\"top:15px;left:165px;\"></div>";
	echo "<div id=\"".$value_id."\" class=\"".$css_value."\" style=\"top:70px;left:15px;\">".$value."</div>";
	echo "<div id=\"".$uom_id."\" class=\"".$css_label."\" style=\"top:102px;left:122px;\">".$uom."</div>";

	echo "</div>";
	
			
	switch ($sqstyle) {
		case 0: //inactive
			echo "<script type=\"text/javascript\">";
			echo "$(document).ready(function() {";
			echo "$(\"#".$dot_id."\").hide();";
			echo "$(\"#".$value_id."\").hide();";
			echo "$(\"#".$uom_id."\").hide();";
			echo "});";
			echo "</script>";
		break;
		case 2: //no dot, no link
			echo "<script type=\"text/javascript\">";
			echo "$(document).ready(function() {";
			echo "$(\"#".$dot_id."\").hide();";
			echo "$('#".$square_id."').addClass('infosquare_nolink');";
			echo "$('#".$square_id."').removeClass('infosquare');";
			echo "});";
			echo "</script>";
		break;
		default:
		break;
	}
}

function drwsq_threshold($sensor_id,$square_id) {	
		
		switch ($sensor_id) {
		case "ph":
			$squaretitle="pH";
			$label=label("10");
			$link="index.php?page=ph";
			$active=SensorEnabled("ph");
			$decimal=1;
			$thigh=ndisplay(ThresholdHigh($_SESSION['MyPhSensor']),$decimal);
			$tlow=ndisplay(ThresholdLow($_SESSION['MyPhSensor']),$decimal);
			$value=ndisplay(LatestSensorReading($_SESSION['MyPhSensor']),$decimal);
			$text="</br>"."> ".$tlow."</br>"."< ".$thigh;
			$link="index.php?page=threshold&sensor=".$_SESSION['MyPhSensor'];
		break;
		case "co2":
			$squaretitle="CO2";
			$label=label("10");
			$uom=label("7");
			$link="index.php?page=co2";
			$active=SensorEnabled("co2");
			$decimal=1;
			$thigh=ndisplay(ThresholdHigh($_SESSION['MyCO2Sensor']),$decimal);
			$tlow=ndisplay(ThresholdLow($_SESSION['MyCO2Sensor']),$decimal);
			$value=ndisplay(LatestSensorReading($_SESSION['MyCO2Sensor']),$decimal);
			$text="</br>"."> ".$tlow."</br>"."< ".$thigh;
			$link="index.php?page=threshold&sensor=".$_SESSION['MyCO2Sensor'];
		break;
		case "temp":
			$squaretitle="temp";
			$label=label("10");
			$uom=label("8");
			$link="index.php?page=temp";
			$active=SensorEnabled("temp");
			$decimal=1;
			$thigh=ndisplay(ThresholdHigh($_SESSION['MyTempSensor']),$decimal);
			$tlow=ndisplay(ThresholdLow($_SESSION['MyTempSensor']),$decimal);
			$value=ndisplay(LatestSensorReading($_SESSION['MyTempSensor']),$decimal);
			$text="</br>"."> ".$tlow."</br>"."< ".$thigh;
			$link="index.php?page=threshold&sensor=".$_SESSION['MyTempSensor'];
		break;
		case "flow":
			$squaretitle="Flow";
			$label=label("10");
			$uom=label("9");
			$link="index.php?page=flow";
			$active=SensorEnabled("flow");
			$decimal=0;
			$tlow=ndisplay(ThresholdLow($_SESSION['MyFlowSensor']),$decimal);
			$value=ndisplay(LatestSensorReading($_SESSION['MyFlowSensor']),$decimal);
			$text="</br>"."> ".$tlow;
			$link="index.php?page=threshold&sensor=".$_SESSION['MyFlowSensor'];
		break;
		case "light":
			$squaretitle="light";
			$label=label("10");
			$uom=label("9");
			$link="index.php?page=light";
			$active=SensorEnabled("light");
			$decimal=0;
			$tlow=ndisplay(ThresholdLow($_SESSION['MyLightSensor']),$decimal);
			$value=ndisplay(LatestSensorReading($_SESSION['MyLightSensor']),$decimal);
			$text="</br>"."> ".$tlow;
			$link="index.php?page=threshold&sensor=".$_SESSION['MyLightSensor'];
		break;								
	}
	
	$css_dot = "dot_".SensorDotColor($sensor_id, $value);
	
	//Define derived variables
	$label_id=$square_id."_label";
	$dot_id=$square_id."_dot";
	$value_id=$square_id."_value";

	//define CSS variables
	if ($active==1) {$css_square="infosquare";} else {$css_square="infosquare_inactive";} //when inactive
	$css_value="squarevalue_small";
	$css_label="squarelabel";


	
	echo "<div id=\"".$square_id."\" class=\"".$css_square."\" title=\"".$squaretitle."\" onclick=\"window.location='".$link."';\">";
	echo "<div id=\"".$label_id."\" class=\"".$css_label."\" style=\"width:30px;top:15px;left:30px;\">".$label."</div>";
	echo "<div id=\"".$dot_id."\" class=\"".$css_dot."\" style=\"top:15px;left:165px;\"></div>";
	echo "<div id=\"".$value_id."\" class=\"".$css_value."\" style=\"top:65px; left:19px; width:171px;\">".$text;
	echo "</div>";
	echo "</div>";

}


function drwsq_lt($square_id,$squaretitle,$link,$text) {
	
	//Define derived variables
	$label_id=$square_id."_label";
	$value_id=$square_id."_value";
	
	//define CSS variables
	$css_square="infosquare";
	$css_value="squarevalue_small";
	$css_label="squarelabel";
	
	echo "<div id=\"".$square_id."\" class=\"".$css_square."\" title=\"".$squaretitle."\" onclick=\"window.location='".$link."';\">";
	echo "<div id=\"".$label_id."\" class=\"".$css_label."\" style=\"width:30px;top:15px;left:30px;\">".$label."</div>";
	echo "<div id=\"".$value_id."\" class=\"".$css_value."\" style=\"top:65px; left:25px; width:150px;\">".$text;
	echo "</div>";
	echo "</div>";

}


function drwsq($square_id){
	//define CSS variables
	$css_square="infosquare_nolink";
	
	echo "<div id=\"".$square_id."\" class=\"".$css_square."\">  </div>";
}

function drwsq_full($square_id){
	//define CSS variables
	$css_square="infosquare_huge";
	
	echo "<div id=\"".$square_id."\" class=\"".$css_square."\">";
	echo "</div>";
}

function drwsq_threshold_input($sensor_id){
	//define CSS variables
	$css_square="infosquare_huge";
	
	$sql = "SELECT thresholdhigh, thresholdlow, warnhigh, warnlow FROM sensor WHERE id = " . $sensor_id . ";";
	$rs = mysql_query($sql);
	$data = mysql_fetch_array($rs);
	$callingURL="form_threshold.php?url=".CurrentPageURL();	
	
	echo "<div id=\"".$square_id."\" class=\"".$css_square."\">";

	echo "<div id=\"inner\" class=\"infosquare_huge_inner\">";

	echo "<form action=\"".$callingURL."\" method=\"post\">";	
	echo "<table> ";
	
	if ($sensor_id!=1 and $sensor_id!=2) { //differs from flow and light
		echo "<tr><td>";
		echo label("34");
		echo "</td><td>";	 
		echo "<input style=\"height:20px; width:60px; align:right;\" type=\"number\" step=\"0.1\" name=\"thigh\" value=\"".cformat($data['thresholdhigh'],1)."\">";
		echo "</td></tr> " ;
	
		echo "<tr><td>";
		echo label("35");
		echo "</td><td>";	 
		echo "<input style=\"height:20px; width:60px\" type=\"number\" step=\"0.1\" name=\"warnhigh\" value=\"".cformat($data['warnhigh'],1)."\">";
		echo "</td></tr> " ;
	
		
		echo "<tr><td>";
		echo label("36");
		echo "</td><td>";	 
		echo "<input style=\"height:20px; width:60px\" type=\"number\" step=\"0.1\" name=\"warnlow\" value=\"".cformat($data['warnlow'],1)."\" >";
		echo "</td></tr> " ;
	
		echo "<tr><td>";
		echo label("37");
		echo "</td><td>";	 
		echo "<input style=\"height:20px; width:60px\" type=\"number\" step=\"0.1\" name=\"tlow\" value=\"".cformat($data['thresholdlow'],1)."\">";
		echo "</td></tr> " ;
	} else {
			echo "<tr><td>";
		echo label("36");
		echo "</td><td>";	 
		echo "<input style=\"height:20px; width:60px\" type=\"number\"  step=\"1\" name=\"warnlow\" value=\"".cformat($data['warnlow'],0)."\" >";
		echo "</td></tr> " ;
	
		echo "<tr><td>";
		echo label("37");
		echo "</td><td>";	 
		echo "<input style=\"height:20px; width:60px\" type=\"number\"  step=\"1\" name=\"tlow\" value=\"".cformat($data['thresholdlow'],0)."\">";
		echo "</td></tr> " ;
	}

	echo "<tr><td colspan=\"2\" align=\"center\">";
	echo "<input type=\"hidden\" name=\"sensor_id\" value=\"".$sensor_id."\"> ";
	echo "<input style=\"height:20px; width:40px\" type=\"submit\" name=\"submit\" value=\"".label("30")."\">";
	echo "</td></tr> ";
 	
	echo "</table> ";
	echo "</form>";

 	echo "</div>"; //inner
	echo "</div>";

}

function drwlsq_jpg($square_id,$squaretitle,$imgpath,$link){
	//define CSS variables
	$css_square="infosquare_large";
	
	echo "<div id=\"".$square_id."\" title=\"".$squaretitle."\" class=\"".$css_square."\" onclick=\"window.location='".$link."';\">";
	echo "<img src=\"".$imgpath."\" width=\"400\" height=\"195\" alt=\"".$squaretitle."\" />";
	echo "</div>";
	
}

function drwlsq_pwr($outlet_id,$squaretitle,$color,$link){
	//define CSS variables
	$css_square="infosquare_large";
	$css_sqimg="infosquare_large_pwr_".$color;
	$css_text="text";
	$square_id="pwr".$outlet_id;
	$sqimg_id=$square_id."_".$color;
	$img="image/outlet_".$color.".gif";
	$sqtxt_id=$square_id."_text";
	
	echo "<div id=\"".$square_id."\" title=\"".$squaretitle."\" class=\"".$css_square."\" onclick=\"window.location='".$link."';\">";
//ORG:	echo "<div id=\"".$square_id."\" title=\"".$squaretitle."\" class=\"".$css_square."\" onclick=\"window.location='".$link."';\">";
//	echo "<img src=\"".$img."\" alt=\"".$squaretitle."\" />";
	echo "<div id=\"".$sqimg_id."\" class=\"".$css_sqimg."\" style=\"top:65px; left:25px; width:76px;\"> </div>";
	echo "<div id=\"".$sqtxt_id."\" class=\"".$css_text."\" style=\"top:80px; left:130px; width:210px;\">".$squaretitle." </div>";
	echo "</div>";
	
}
?>