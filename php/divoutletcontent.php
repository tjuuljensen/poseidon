<?php

// Configure connection settings
	session_start(); 
	include('php/labels.php');
	include('php/squares.php'); 
	include('php/dbconnect.php');
	include('php/poseidon.php'); 
	include('php/general.php'); 

// Fetch the data
	$active_outlet = $_GET['outlet']; //Get the request URL
	
	drwfooter_outlets($active_outlet);

?>