<?php
require('session.php');
$m = new MongoClient("mongodb://127.0.0.1/twitterCannesLions");
$db = $m->selectDB('twitterCannesLions');
$collection = new MongoCollection($db, 'following');

$sort  = array('user_name' => 1);
$cursor = $collection->find();

?>

<!DOCTYPE html>
<html lang="en">
  
  <head>
    <meta charset="utf-8">
    <title>Twitter #CannesLions | Following</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">
    <meta http-equiv="refresh" content="300">

    <!-- Le styles -->
    <link href="assets/css/bootstrap.css" rel="stylesheet">
    <link href="assets/css/bootstrap-responsive.css" rel="stylesheet">
    <!-- Le HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
      <script src="http://html5shim.googlecode.com/svn/trunk/html5.js">
      </script>
    <![endif]-->
    <!-- Le fav and touch icons -->
    <link rel="shortcut icon" href="assets/ico/favicon.ico">
    <link rel="apple-touch-icon-precomposed" sizes="144x144" href="assets/ico/apple-touch-icon-144-precomposed.png">
    <link rel="apple-touch-icon-precomposed" sizes="114x114" href="assets/ico/apple-touch-icon-114-precomposed.png">
    <link rel="apple-touch-icon-precomposed" sizes="72x72" href="assets/ico/apple-touch-icon-72-precomposed.png">
    <link rel="apple-touch-icon-precomposed" href="assets/ico/apple-touch-icon-57-precomposed.png">
    <style>
    </style>
  </head>
  <body>
    <!--
    <div class="navbar navbar-inverse navbar-fixed-top">
    	<div class="navbar-inner">
    		<div class="container">
    			<button type="button" class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse">
    				<span class="icon-bar"></span>
    				<span class="icon-bar"></span>
    				<span class="icon-bar"></span>
    			</button>
    			<a class="brand">Cannes Lions</a>
    			<div class="nav-collapse collapse">
    				<ul class="nav">
    					<li class="active">
    						<a>Home</a>
    					</li>
    					<li>
    						<a href="#about">About</a>
    					</li>
    					<li>
    						<a href="#contact">Contact</a>
    					</li>
    				</ul>
    			</div>
    		</div>
    	</div>
    </div>
    -->
    
    <div class="container">
    	<h1>
    		#CannesLions Moderation Admin
    	</h1>
    	<br>
    	<ul class="nav nav-tabs">
            <li>
                <a href="main.php?status=Pending">Pending</a>
            </li>
            <li>
                <a href="main.php?status=Approved">Approved</a>
            </li>
            <li>
                <a href="main.php?status=Denied">Denied</a>
            </li>
            <li>
                <a href="whitelist.php">Whitelist</a>
            </li>
            <li class="active">
                <a href="following.php">Following</a>
            </li>
    	</ul>

        <form id="following-add">
        	<div class="input-append">
        		<input type="text" placeholder="Add new @user">
        		<button type="submit" class="btn"><i class="icon-plus"></i></button>
        	</div>
        </form>
    	
    	<table class="table">
    		<thead>
    			<tr>
    				<th width="6%">
    					Delete
    				</th>
    				<th width="15%">
    					Handle
    				</th>
    				<th width="79%">
    					Name
    				</th>
    			</tr>
            </thead>
            <tbody>
                <?php foreach ($cursor as $doc) { ?>
                <tr id="<?= $doc['_id'] ?>">
    				<td>
    					<a class="btn btn-small following-remove"><i class="icon-remove"></i></a>
    				</td>
    				<td>
    					<a href="http://twitter.com/<?= $doc['user_screen_name'] ?>" target="_blank">@<?= $doc['user_screen_name'] ?></a>
    				</td>
    				<td>
    					<?= $doc['user_name'] ?>
    				</td>
    			</tr>
    			<?php } ?>
    		</tbody>
    	</table>
    </div>
    <!-- /container -->

    <style>
      .moderate-image {
          width: 25%;
      }
    </style>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
    <script src="assets/js/bootstrap.js"></script>
    <script type="text/javascript" src="assets/js/main.js"></script>
  </body>
</html>
