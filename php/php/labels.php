<?PHP

$booShowId = false;
$arrLabels = array();
$strLanguage = "dk";

function initialize_labels(){
	global $arrLabels;
	global $strLanguage;
	$arrDefaultLabels = parse_ini_file("php/labels/dk.label");
	if($strLanguage != "dk"){
		$arrLangLabels = parse_ini_file("php/labels/" . $strLanguage . ".label");
		foreach($arrLangLabels as $idx => $value){
			$arrDefaultLabels[$idx] = $value;
		}
	}
	$arrLabels = $arrDefaultLabels;
	$_SESSION['labels'] = $arrLabels;	
}

function label($id){
	global $booShowId;
	global $arrLabels;

	if (array_key_exists($id, $arrLabels)) {
		if($booShowId){
			return "<font color=\"red\">". $id ."<font> ".nl2br($arrLabels[$id]);
		} else {
			return "". nl2br($arrLabels[$id]);
		}
	} else {
		return "<font color=\"red\"> - no such index - <font>";
	}
}

function set_language($lang){
	global $strLanguage;
	$arrAllowedLang = array("dk","uk");
	$lang = strtolower($lang);
	if(in_array($lang,$arrAllowedLang)) $strLanguage = $lang;
	initialize_labels();
}
//always initialize (TEST purpose)
initialize_labels();

if (!isset($_SESSION['labels'])) {
	initialize_labels();
} else {
	$arrLabels = $_SESSION['labels'];
}
//set_language("dk");
?>