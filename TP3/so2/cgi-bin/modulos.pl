#!/usr/bin/perl



use CGI;
use CGI::Carp qw ( fatalsToBrowser );
use File::Basename;


$q = new CGI;

$operation = $q -> param('operation');
$module = $q -> param('module');
$password = $q -> param('password');

if ($operation eq Remove and $password eq pass){
	(system("sudo /sbin/rmmod $module") == 0 and $message = "Se ha removido el modulo $module")
	or $message = "system failed $?";
}
elsif ($operation eq Remove and $password ne pass){
	$message = "Incorrect password";
}

@lista_modulos=`lsmod`;
@modulos = split (" ", shift @lista_modulos);

$remove = "<form action='modulos.pl' method='post'>
<label>Modulo</label>
<select name='module'>";

$table = "<table class='table-striped' style='width:100%'>
<tr>
<th>@modulos[0]</th>
<th>@modulos[1]</th>
<th>@modulos[2] @modulos[3]</th>
</tr>
";
foreach (@lista_modulos) {
  my @modulos = split(" ", $_);
  $table .= "
  <tr>
  <td>@modulos[0]</td>
  <td>@modulos[1]</td>
  <td>@modulos[2] @modulos[3]</td>
  </tr>
  ";
  $remove .= "<option value='@modulos[0]'>@modulos[0]</option>";
}
$table .= "</table>";
$remove .= "</select><br>
<b>Password</b> <input type='text' name='password'>
<input type='submit' name='operation' value='Remove' />
</form>";



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
<li><a href='home.pl'>Home</a></li>
<li><a href='consola.pl'>Consola</a></li>
<li class='active'><a href='modulos.pl'>Modulos</a></li>
</ul>
</div>
</nav>

<div class='page-header'>

<div>
<p>$message</p>
</div>

<h1>Upload New Module</h1>
<form action='myupload.pl' method='post' enctype='multipart/form-data'> 
<p>Module to Upload: <input type='file' name='module' /></p> 
<p><input type='submit' name='Submit' value='Submit Form' /></p> 
</form> 
<h1>Remove Module</h1>
<div>$remove</div>
<h1>Installed Modules</h1>	
</div>
<div>$table</div>


</body>
</html>";

print $html;

