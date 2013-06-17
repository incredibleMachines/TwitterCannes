<?php 
	session_start();
/*
	echo $_SESSION['creds'];
	echo '<br>';
	echo $_SESSION['authenticated'];
	echo '<br>';
	echo $_SESSION['fert'];
	print_r($_SESSION);
*/
	//check time for user to relogin
	
	if(time() > strtotime('+1 day', $_SESSION['_a']) ) session_destroy();
	
	
	//check hash 
	$salt = $_SESSION['_a'];
	$fert = $_SESSION['_f'];
	$key = 'somethingspecial'.'bill'.$fert.'board'.$fert.'20-BLOCKERATOR-13'.$salt.'moder'.$fert.'ator'.'somethingspecial';
	$md5 = md5($key);
	$hash = crypt($key, $salt);
	
	for($i = 0; $i<15; $i++) $hash = crypt($hash . $password, $salt);
		
	if($_SESSION['_x'] !== $hash){
		
		header('Location: login.php');
		exit();
		
	}
?>