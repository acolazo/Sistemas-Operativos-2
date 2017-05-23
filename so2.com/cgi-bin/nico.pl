#!/usr/bin/perl
$datestring = localtime();
sub procesador{

open(my $input_fh,'<','/proc/cpuinfo');
my @procesador = <$input_fh>;
return @procesador[1].", ". @procesador[4] ;

}

sub memoria{

open(my $input_fh,'<','/proc/meminfo');
my @lines = <$input_fh>;
return "".@lines[0]. ", ".@lines[1];

}

sub uptime{

open(my $input_fh,'<','/proc/uptime');
my @lines = <$input_fh>;
return "".@lines[0]. "";

}
$html = "Content-Type: text/html

<html>
<head>
<!-- Latest compiled and minified CSS -->
<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' integrity='sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u' crossorigin='anonymous'>

<!-- Optional theme -->
<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css' integrity='sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp' crossorigin='anonymous'>

<!-- Latest compiled and minified JavaScript -->
<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js' integrity='sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa' crossorigin='anonymous'></script>
<title>Recursos</title>
</head>
<body>
<!-- Nav tabs -->
<ul class='nav nav-tabs' role='tablist'>
  <li class='nav-item'>
    <a class='nav-link active' data-toggle='tab' href='#home' role='tab'>Recursos</a>
  </li>
  <li class='nav-item'>
    <a class='nav-link' data-toggle='tab' href='comandos.pl' role='tab'>Comandos AWS</a>
  </li>
  <li class='nav-item'>
    <a class='nav-link' data-toggle='tab' href='#messages' role='tab'>Listado Modulos</a>
  </li>
  <li class='nav-item'>
    <a class='nav-link' data-toggle='tab' href='#settings' role='tab'>Agregar Modulo!</a>
  </li>
</ul>

<div class='page-header'>
<h1 class='text-center'>Recursos del sistema embebido</h1>
</div>

<div class='panel-body'>
<P class='text-center' >
Direccion IP es: <strong> $ENV{REMOTE_ADDR}</strong>
</p>
<p class='text-center'>Informacion del procesador:<strong> ${\procesador()}</strong></p>
<p class='text-center'>Informacion de la memoria:<strong> ${\memoria()}</strong></p>
<p class='text-center'>Uptime:<strong> ${\uptime()}</strong></p>
<p class='text-center'>Uptime:<strong> $datestring</strong></p>
</div>

</body>
</html>";

print $html;