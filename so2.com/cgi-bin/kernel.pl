#!/usr/bin/perl

use CGI;
use CGI::Carp qw ( fatalsToBrowser );
use File::Basename;


$q = new CGI;

$operation = $q -> param('operation');
$filename = $q -> param('filename');

if ($operation eq Load){
	system("sudo /sbin/insmod $filename") == 0
	or $mensaje = "system failed $?";
	#terminal = `sudo insmod $filename`;
	$message = "<p>Se ha instalado el modulo</p>";
}
elsif ($operation eq Unload){
	system("sudo /sbin/rmmod $filename") == 0
	or $mensaje = "system failed $?";
	#@terminal = `sudo rmmod $filename`;
	$message = "<p>Se ha removido el modulo</p>";
}

$path = `pwd`;

$html = "Content-Type: text/html

<html>
<head>
<!-- Latest compiled and minified CSS -->
<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' integrity='sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u' crossorigin='anonymous'>

<!-- Optional theme -->
<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css' integrity='sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp' crossorigin='anonymous'>

<!-- Latest compiled and minified JavaScript -->
<title>Modulos</title>
</head>

<body>
<nav class='navbar navbar-inverse'>
<div class='container-fluid'>
<div class='navbar-header'>
<a class='navbar-brand' href='#'>Estacion Meteorologica</a>
</div>
<ul class='nav navbar-nav'>
<li class='active'><a href='home.pl'>Home</a></li>
<li><a href='consola.pl'>Consola</a></li>
<li><a href='modulos.pl'>Modulos</a></li>
</ul>
</div>
</nav>

<div class='page-header'>
<h1>Kernel Module</h1>
</div>


<form action='kernel.pl' method='post' enctype='multipart/form-data'> 
<input type = 'hidden' name='filename' value=$filename />
<p><input type='submit' name='operation' value='Load' /></p> 
<p><input type='submit' name='operation' value='Unload' /></p> 
</form>


<p>La operacion es $operation y el nombre del archivo es $filename</p>
$message

<p>Terminal: @terminal</p>
<p>$mensaje</p>
<p>$path</p>
</body>
</html>
";

print $html;