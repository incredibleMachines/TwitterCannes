<?php
require('session.php');
header("Refresh: ");
$m = new MongoClient("mongodb://127.0.0.1/twitterCannesLions");
$db = $m->selectDB('twitterCannesLions');
$collection = new MongoCollection($db, 'tweets');

$status = isset($_GET['status']) ? $_GET['status'] : 'Pending';
$page  = isset($_GET['page']) ? (int) $_GET['page'] : 1;
$limit = isset($_GET['limit']) ? (int) $_GET['limit'] : 10;
$search = isset($_GET['search']) ? $_GET['search'] : '';
$skip  = ($page - 1) * $limit;
$next  = ($page + 1);
$prev  = ($page - 1);
$sort  = array('approved_at' => -1, 'created_at' => -1);

if ($search != '') {
    $split = explode(' ', $search);
    for ($i = 0; $i < sizeOf($split); $i++) {
        $or[] = array('user_name' => new MongoRegex("/$split[$i]/i"));
        $or[] = array('user_screen_name' => new MongoRegex("/$split[$i]/i"));
        $or[] = array('text' => new MongoRegex("/$split[$i]/i"));
    }
}

$and = array(
    array('status' => $status)
);
if ($or) {
    $and[] = array('$or' => $or);
}

$find = array(
    '$and' => $and
);


$total = $collection->count();
$cursor = $collection->find($find)->skip($skip)->limit($limit)->sort($sort);

?>

<!DOCTYPE html>
<html lang="en">
  
  <head>
    <meta charset="utf-8">
    <title>Twitter #CannesLions</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">
    <meta http-equiv="refresh" content="300">
    <!-- Le styles -->
    <link href="assets/css/bootstrap.css" rel="stylesheet">
    <link href="assets/css/bootstrap-responsive.css" rel="stylesheet">
    <link href="assets/css/main.css" rel="stylesheet">
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
            <li <?= ($status == 'Pending') ? 'class="active"' : '' ?>>
    			<a href="main.php?status=Pending">Pending</a>
    		</li>
            <li <?= ($status == 'Approved') ? 'class="active"' : '' ?>>
    			<a href="main.php?status=Approved">Approved</a>
    		</li>
    		<li <?= ($status == 'Denied') ? 'class="active"' : '' ?>>
    			<a href="main.php?status=Denied">Denied</a>
    		</li>
    		<li>
    			<a href="whitelist.php">Whitelist</a>
    		</li>
    		<li>
    			<a href="following.php">Following</a>
    		</li>
    	</ul>

        <form class="form-inline form-search-tweets" action="main.php" method="get">
            <input type="hidden" name="status" value="<?= $status ?>">
            <div class="input-append">
                <input type="text" name="search" placeholder="Search">
                <button type="submit" class="btn"><i class="icon-search"></i></button>
            </div>
        </form>

        <!--
    	<div class="btn-group">
    		<a class="btn dropdown-toggle" data-toggle="dropdown">
                #CannesLions &nbsp;<span class="caret"></span>
            </a>
    		<ul class="dropdown-menu">
    			<li>
    				<a>#CannesLions</a>
    			</li>
    			<li class="divider">
    			</li>
    			<li class="disabled"><a>Following users</a></li>
    			<li>
    				<a>@TeamCoco</a>
    			</li>
    			<li>
    				<a>@Adobe</a>
    			</li>
    			<li>
    				<a>@Yahoo</a>
    			</li>
    			<li>
    				<a>@Arnoldworldwide‎</a>
    			</li>
    			<li>
    				<a>@BBDONY‎</a>
    			</li>
    		</ul>
    	</div>
        -->

        <ul class="pager pager-mini">
            <li class="previous <?php if ($page == 1) print 'disabled' ?>">
                <a href="<?= $_SERVER["SCRIPT_NAME"]."?page=".($page-1)."&limit=$limit&status=$status" ?>" >&larr; Newer</a>
            </li>
            <li class="next <?php if ($page * $limit >= $total) print 'disabled' ?>">
                <a href="<?= $_SERVER["SCRIPT_NAME"]."?page=".($page+1)."&limit=$limit&status=$status" ?>" >Older &rarr;</a>
            </li>
        </ul>
    	
        <table class="table table-striped">
    		<thead>
    			<tr>
    				<th width="11%">
    					Moderate
    				</th>
    				<th width="12%">
    					Category
    				</th>
    				<th width="15%">
    					User
    				</th>
    				<th width="32%">
    					Text
    				</th>
    				<th width="30%">
    					Photo
    				</th>
    			</tr>
            </thead>
            <tbody>
                <?php foreach ($cursor as $doc) { ?>
    			<tr id="<?= $doc['_id'] ?>">
    				<td>
    					<div class="btn-group">
    						<a class="btn btn-small approve"><i class="icon-ok"></i></a>
    						<a class="btn btn-small deny"><i class="icon-remove"></i></a>
    					</div>
                        <a class="btn btn-small favorite <?= $doc['starred'] ? 'btn-danger' : '' ?>"><i class="icon-heart <?= $doc['starred'] ? 'icon-white' : '' ?>"></i></a>
    				</td>
    				<td>
    					<div class="btn-group">
    						<a class="btn btn-small dropdown-toggle" data-toggle="dropdown">
                                <span class="current-category"><?= $doc['category'] ?></span>
                                <span class="caret"></span>
                            </a>
    						<ul class="dropdown-menu category-select">
                                <li data-category="None"><a>None</a></li>
                                <li class="divider"></li>
                                <li data-category="Celebrities"><a>Celebrities</a></li>
    							<li data-category="Executive Tweets"><a>Executive Tweets</a></li>
    						</ul>
    					</div>
    				</td>
    				<td>
    					<a href="http://twitter.com/<?= $doc['user_screen_name'] ?>" target="_blank">@<?= $doc['user_screen_name'] ?></a>
    					<br>
    					<?= $doc['user_name'] ?>
    				</td>
    				<td>
    					<a href="http://twitter.com/<?= $doc['user_screen_name'] ?>/status/<?= $doc['id'] ?>" target="_blank"><?= $doc['text'] ?></a>
    				</td>
    				<td>
    					<?php if ($doc['media_url']) { ?><img class="img-polaroid moderate-image" src="<?= $doc['media_url'] ?>"><?php } ?>
    				</td>
    			</tr>
                <?php } ?>
    		</tbody>
    	</table>
    </div>
    <!-- /container -->

    <style>
      .moderate-image {
          width: 80%;
      }
    </style>
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"></script>
    <script src="assets/js/bootstrap.js"></script>
    <script type="text/javascript" src="assets/js/main.js"></script>
  </body>
</html>
