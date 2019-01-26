<?php

// Configure connection settings
	session_start(); 
	include('php/labels.php');
	include('php/squares.php'); 
	include('php/dbconnect.php');
	include('php/poseidon.php'); 
	include('php/general.php'); 

// Fetch the data
	$sensor = $_GET['sensor']; //Get the request URL
	$square = $_GET['square'];
	$sqstyle =  $_GET['style'];
//	echo $sensor;
//	echo ";";
//	echo $square;	
	
	drw_sensorcontent($sensor, $square, $sqstyle);

?>