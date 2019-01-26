<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Power Control</title>
</head>

<body>

<?PHP 
	session_start(); 
	include('php/labels.php');
	include('php/squares.php'); 
	include('php/dbconnect.php');
	include('php/poseidon.php'); 
	include('php/general.php'); 

echo "Det virker!"."</br>";

$teststr="01234567890987654321";
echo $teststr."</br>";
echo "right (90): ".right($teststr,"90")."</br>";
echo "left (90): ".left($teststr,"90")."</br>";
echo "</br>";


function displayLocales($number, $isMoney, $lg='en_US.utf8') {
    $ret = setLocale(LC_ALL, $lg);
    setLocale(LC_TIME, 'Europe/Paris');
    if ($ret===FALSE) {
        echo "Language '$lg' is not supported by this system.\n";
        return;
    }
    $LocaleConfig = localeConv();
    forEach($LocaleConfig as $key => $val) $$key = $val;

    // Sign specifications:
    if ($number>0) {
        $sign = $positive_sign;
        $sign_posn = $p_sign_posn;
        $sep_by_space = $p_sep_by_space;
        $cs_precedes = $p_cs_precedes;
    } else {
        $sign = $negative_sign;
        $sign_posn = $n_sign_posn;
        $sep_by_space = $n_sep_by_space;
        $cs_precedes = $n_cs_precedes;
    }

    // Number format:
    $n = number_format(abs($number), $frac_digits,
        $decimal_point, $thousands_sep);
    $n = str_replace(' ', '&nbsp;', $n);
    switch($sign_posn) {
        case 0: $n = "($n)"; break;
        case 1: $n = "$sign$n"; break;
        case 2: $n = "$n$sign"; break;
        case 3: $n = "$sign$n"; break;
        case 4: $n = "$n$sign"; break;
        default: $n = "$n [error sign_posn=$sign_posn&nbsp;!]";
    }

    // Currency format:
    $m = number_format(abs($number), $frac_digits,
        $mon_decimal_point, $mon_thousands_sep);
    if ($sep_by_space) $space = ' '; else $space = '';
    if ($cs_precedes) $m = "$currency_symbol$space$m";
    else $m = "$m$space$currency_symbol";
    $m = str_replace(' ', '&nbsp;', $m);
    switch($sign_posn) {
        case 0: $m = "($m)"; break;
        case 1: $m = "$sign$m"; break;
        case 2: $m = "$m$sign"; break;
        case 3: $m = "$sign$m"; break;
        case 4: $m = "$m$sign"; break;
        default: $m = "$m [error sign_posn=$sign_posn&nbsp;!]";
    }
    if ($isMoney) return $m; else return $n;
}

//$number = -12345.12345;
//echo '<div>'.displayLocales($number, FALSE)."</div>\n";
//echo '<div>'.displayLocales($number, TRUE)."</div>\n";

?>

</body>
</html>