<?PHP
// Der oprettes forbindelse til databasen
    $link = mysql_connect("localhost:3306", "poseidonUSER", "poseidon") or die("Could not connect: " . mysql_error() . "<br>");
	mysql_select_db("poseidondb", $link) or die("Could not select DB: " . mysql_error() . "<br>");
	
?>