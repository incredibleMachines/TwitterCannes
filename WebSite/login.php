<?php 
	session_start();
	
	
	
	$error = false;
	$error_details= 'Login Errors: ';
	$success = -1;

	if(isset($_POST['login']) && isset($_POST['pass'])){
				
		if( $_POST['login']===""){
			$error = true;
			$error_details.= ' No Login';	
		}
		
		if($_POST['pass']==""){
			if($error) $error_details.=',';
			$error = true;
			$error_details.= ' No Password';
		}
	
		if($_POST['login']!=='moderator'){
			if($_POST['login']!==""){
				if($error) $error_details.=',';
				$error = true;
				$error_details.=' Incorrect Login';
			}
			
		}else{
			//$error = true;
			//$error_details.=' Correct Login';	
			$success++;
		}	
		
						
		if($_POST['pass']!=='billboard2013'){
			if($_POST['pass']!==""){
				if($error) $error_details.=',';
				$error = true;
				$error_details.=' Incorrect Password';
			}
		}else{
			//$error = true;
			//$error_details.=' Correct Password';	
			$success++;
			
		}	
		
		if($success===1){ //login success
		
			$_SESSION['_a']=time();
			$_SESSION['_f'] = rand(0,45);
			
			$salt = $_SESSION['_a'];
			$fert = $_SESSION['_f'];
			$key = 'somethingspecial'.'bill'.$fert.'board'.$fert.'20-BLOCKERATOR-13'.$salt.'moder'.$fert.'ator'.'somethingspecial';
			$md5 = md5($key);
			$hash = crypt($key, $salt);
			
			for($i = 0; $i<15; $i++) $hash = crypt($hash . $password,$salt);
			
			$_SESSION['_x']= $hash;
			
			$m = new MongoClient("mongodb://127.0.0.1/twitterCannesLions");
			$db = $m->selectDB('twitterCannesLions');
			$collection = new MongoCollection($db, 'logins');
			
			$collection->insert(array('ip'=> $_SERVER['REMOTE_ADDR'], 'timestamp'=> new MongoDate() ));
			
			
			header('Location: main.php');
		
		}
	
	} 
	
?>
<!DOCTYPE html>
<html lang="en">
  
  <head>
    <meta charset="utf-8">
    <title>Twitter #CannesLions</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">
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
  	<div class="container" style="margin: 60px auto 0; max-width: 500px;">
  			<?php if($error): ?> 
  				<h6 class="text-error">
  				<?php echo $error_details;?>
	  			</h6>			
  			<?php endif;?>	
			<form class="form-horizontal" method="post">
			  <div class="control-group">
			    <label class="control-label" for="inputEmail">Login</label>
			    <div class="controls">
			      <input type="text" id="inputEmail" name="login" placeholder="Login">
			    </div>
			  </div>
			  <div class="control-group">
			    <label class="control-label" for="inputPassword">Password</label>
			    <div class="controls">
			      <input type="password" id="inputPassword" name="pass" placeholder="Password">
			    </div>
			  </div>
			  <div class="control-group">
			    <div class="controls">
			      <button type="submit" class="btn">Sign in</button>
			    </div>
			  </div>
			</form>
  	</div>

  </body>