#!/usr/bin/perl

use CGI;
use CGI::Carp qw ( fatalsToBrowser );
use File::Basename;

$q = new CGI;
$local_path = `pwd`;

$filename = $q -> param ('module') ;
($name, $path, $extension) = fileparse($filename, '\.[^\.]*');

if ($extension eq ".ko"){
	$filename = $name . $extension;
	my $upload_filehandle = $q->upload("module");
	open ( UPLOADFILE, ">$filename" ) or die "$!"; 
	binmode UPLOADFILE;
	while ( <$upload_filehandle> )
	{
		print UPLOADFILE;
	}

	close UPLOADFILE;

	$message = "<p>Su archivo se ha subido con exito</p>
	<form action='kernel.pl' method='post' enctype='multipart/form-data'> 
	<input name='filename' type='hidden' value=$filename />
	<p><input type='submit' name='operation' value='Load' /></p> 
	<p><input type='submit' name='operation' value='Unload' /></p>
	</form>";
}
else{
	$message = "<p>La extension es incorrecta. Suba un archivo con extension .ko</p>";
}


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

$message



</body>
</html>
";

print $html;
