<?php

// Load up mongo client & connect to db
$m = new MongoClient("mongodb://127.0.0.1/twitterCannesLions");
$db = $m->selectDB('twitterCannesLions');

// Generate mongo id object
$id = new MongoID($_GET['id']);

// Set up our timezone as UTC (all tweets come in with UTC timestamps)
date_default_timezone_set('UTC');
$date = date('Y-m-d H:i:s');

// --------------------------------------------------
// Tweet moderation -- aprove or deny
// --------------------------------------------------
if ($_GET['mode'] == 'moderate') {

	$collection = new MongoCollection($db, 'tweets');
	
	if ($_GET['action'] == 'approve') {
		$collection->update(array('_id' => $id),  array( '$set' => array('status' => 'Approved', 'approved_at' => $date)) );
		print $_GET['id'];
	}
	if ($_GET['action'] == 'deny') {
		$collection->update(array('_id' => $id),  array( '$set' => array('status' => 'Denied', 'approved_at' => $date)) );
		print $_GET['id'];
	}
}

// --------------------------------------------------
// Tweet moderation -- set a category
// --------------------------------------------------
if ($_GET['mode'] == 'category') {
	$collection = new MongoCollection($db, 'tweets');

	$collection->update(array('_id' => $id),  array( '$set' => array('category' => $_GET['category'], 'approved_at' => $date)) );
	print $_GET['category'];
}

// --------------------------------------------------
// Whitelist/Following moderation -- add or remove users
// --------------------------------------------------
if ($_GET['mode'] == 'whitelist' || $_GET['mode'] == 'following') {

	$collection = new MongoCollection($db, $_GET['mode']);

	if ($_GET['action'] == 'add') {

		require_once 'twitter-php/twitter.class.php';
		$twitter = new Twitter('ZTioLF2GOz5ZR3xtRyl7g', 'oji9yVuN024t7C67yc18GyHmGnwYxxaNoRP7Z98jEOw', '371351910-f4pp7PjaM6ny3frWyImB1JIg2kpfGqtcKl1Glt4n', 'qod1HsYuMhcM7eImbipb1p7bYkcEDracyLx89XVCfik');
		
		$json = $twitter->loadUserInfo($_GET['handle']);
		$doc = array('id' => $json->id_str, 'user_name' => $json->name, 'user_screen_name' => $json->screen_name);

		$collection->insert($doc);
		print json_encode($doc);
	}
	if ($_GET['action'] == 'remove') {
		$collection->remove(array('_id' => $id));
		print $_GET['id'];
	}
}



// --------------------------------------------------
// Utilities for debugging
// --------------------------------------------------
if ($_GET['mode'] == 'users_show') {

	require_once 'twitter-php/twitter.class.php';
	$twitter = new Twitter('ZTioLF2GOz5ZR3xtRyl7g', 'oji9yVuN024t7C67yc18GyHmGnwYxxaNoRP7Z98jEOw', '371351910-f4pp7PjaM6ny3frWyImB1JIg2kpfGqtcKl1Glt4n', 'qod1HsYuMhcM7eImbipb1p7bYkcEDracyLx89XVCfik');
	
	$json = $twitter->request('users/show', 'GET', array('screen_name' => $_GET['handle']));
	var_dump($json);


}
if ($_GET['mode'] == 'statuses_show') {

	require_once 'twitter-php/twitter.class.php';
	$twitter = new Twitter('ZTioLF2GOz5ZR3xtRyl7g', 'oji9yVuN024t7C67yc18GyHmGnwYxxaNoRP7Z98jEOw', '371351910-f4pp7PjaM6ny3frWyImB1JIg2kpfGqtcKl1Glt4n', 'qod1HsYuMhcM7eImbipb1p7bYkcEDracyLx89XVCfik');
	
	$json = $twitter->request('statuses/show', 'GET', array('id' => $_GET['tweet_id']));
	var_dump($json);


}

?>