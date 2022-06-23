<!DOCTYPE html>
<html>
<head>
<title>Contact</title>
<link href="css/bootstrap.css" rel="stylesheet" type="text/css" media="all" />
<!-- Custom Theme files -->
<script src="js/jquery.min.js"></script>
<!--theme-style-->
<link href="css/style.css" rel="stylesheet" type="text/css" media="all" />	
<!--//theme-style-->
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta name="keywords" content="" />
<script type="application/x-javascript"> addEventListener("load", function() { setTimeout(hideURLbar, 0); }, false); function hideURLbar(){ window.scrollTo(0,1); } </script>
<link href='http://fonts.googleapis.com/css?family=Simonetta:400,900' rel='stylesheet' type='text/css'>
<link href='http://fonts.googleapis.com/css?family=Questrial' rel='stylesheet' type='text/css'>
<script src="js/bootstrap.min.js"></script>
<!-- animation-effect -->
<link href="css/animate.min.css" rel="stylesheet"> 
<script src="js/wow.min.js"></script>
<script>
 new WOW().init();
</script>
<?php
    exec('./verify',$result);
    $re=$result[1];
    if(strcmp('pair_success!',$re)==0)
    {
        echo "<script type='text/javascript'>alert('pair_success!');</script>";
    }
    else if(strcmp('pair_fail',$re)==0){
        echo "<script type='text/javascript'>alert('pair_fail!');</script>";
    }
?>
<!-- //animation-effect -->
</head>
<body>
<!--header-->
<div class="header">
	<div class="container">
		<div class="header-top">
			<div class="col-sm-3 header-login">
				<div class=" logo animated wow shake" data-wow-duration="1000ms" data-wow-delay="500ms">
					<h1><a href="index.html">ZCAGE</a>	</h1>
				</div>
			</div>	
			<div class="col-sm-9 header-social  col-md-8 col-md-offset-1">
							
					<ul class="social-icon">
						<li><a href="#" ><i></i></a></li>
						<li><a href="#"><i class="ic1"></i></a></li>
						<li><a href="#"><i class="ic2"></i></a></li>
						<li><a href="#"><i class="ic3"></i></a></li>
						<li><a href="#"><i class="ic4"></i></a></li>
					</ul>
					<div class="clearfix"> </div>
     		<div class="head">
			<nav class="navbar navbar-default">
					<!-- Brand and toggle get grouped for better mobile display -->
					<div class="navbar-header">
					  <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1">
						<span class="sr-only">Toggle navigation</span>
						<span class="icon-bar"></span>
						<span class="icon-bar"></span>
						<span class="icon-bar"></span>
					  </button>
						
					</div>

					<!-- Collect the nav links, forms, and other content for toggling -->
					<div class="collapse navbar-collapse nav-wil col-md-11 col-md-offset-0" id="bs-example-navbar-collapse-1">
					 <ul class="nav navbar-nav">
						<li ><a class="nav-in" href="index.html"><span data-letters="Home">Home</span></a> </li>
						<li ><a class="nav-in" href="about.html"><span data-letters="About">About</span></a> </li>
						<li><a class="nav-in" href="services.html"><span data-letters="Services">Services</span></a></li>
						<li><a class="nav-in" href=""><span data-letters="Codes">Codes</span></a></li>
						<li><a class="nav-in" href="contact.html"><span data-letters="Contact">Contact</span></a></li>
					</ul>
					   <form class="navbar-form navbar-left" role="search">
        <div class="form-group">
          <input type="text" class="form-control form-cont" value="Coming Soon" onfocus="this.value='';" onblur="if (this.value == '') {this.value ='Coming Soon';}">
        </div>
        <button type="submit" class="btn-search"></button>
      </form>
					</div><!-- /.navbar-collapse -->
					
				</nav>
				
		</div>
					
			</div>
				<div class="clearfix"> </div>
		</div>

	</div>
</div>
<!--content-->
<!-- map -->
<div class="contact">
	<h2> Tax system</h2>
	<div class="map">
		<iframe ></iframe>
	</div>

<div class="map-grids animated wow fadeInUp" data-wow-duration="1000ms" data-wow-delay="500ms">
	
		
		<div class="col-md-8 map-middle">
			
			<form action="proof.php" method="get">
					<input type="text" name="min" value="Min Number" onfocus="this.value = '';" onblur="if (this.value == '') {this.value = 'Min Number';}" required="">
					<input type="text" name="max" value="Max Number" onfocus="this.value = '';" onblur="if (this.value == '') {this.value = 'Max Number';}" required="">
					<input type="text" name="val" value="Privacy Value" onfocus="this.value = '';" onblur="if (this.value == '') {this.value = 'Privacy Value';}" required="">
					
					<input type="submit" value="PROOF"><br><br>
					</a>
			</form>
			</form>
		</div>
		<div class="col-md-4 map-left">
			<h3>Introduction</h3>
				<div class="col-md-15 come">
									<p>You can use the system to prove the tax situation of the enterprise without divulging privacy.</p> 
						
						
					</div>

		</div>
		<div class="clearfix"></div>
	</div>

</div>
<!-- //map -->

 		<!--//content-slide-->
</div>
<div class="footer animated wow fadeInUp" data-wow-duration="1000ms" data-wow-delay="500ms">
	<div class="container">
			<h4>Later Development</h4>
			<p class="footer-in">Later we will launch more functions <br>e.g hash function <br>Coming soon </p>
			<ul class="footer-top">
			<li><span><i class="glyphicon glyphicon-earphone"></i>+123456789</span></li>
			<li><a href="mailto:info@example.com"><i class="glyphicon glyphicon-envelope"></i> sunkai@250.com</a>
			</ul>
			<p class="footer-class">It is used in competition and will not be transmitted</a></p>			
	</div>
</div>	
</body>
</html>