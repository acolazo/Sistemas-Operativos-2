#!/usr/bin/perl


open($data, '<', '/proc/cpuinfo');
@cpu_info = <$data>;
open($data, '<', '/proc/meminfo');
@mem_info = <$data>;
open($data, '<', '/proc/uptime');
@uptime = <$data>;
$time = localtime();



$html = "Content-Type: text/html

<html>
<head>
<!-- Latest compiled and minified CSS -->
<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' integrity='sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u' crossorigin='anonymous'>

<!-- Optional theme -->
<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css' integrity='sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp' crossorigin='anonymous'>

<!-- Latest compiled and minified JavaScript -->
<title>Recursos</title>
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
<h1>Recursos</h1>
</div>

<div class='panel-body'>
<ul>
<li>Procesador
<ul>
<li>@cpu_info[4]</li>
</ul></li>
<li>Memoria
<ul>
<li>@mem_info[0]</li>
<li>@mem_info[1]</li>
<li>@mem_info[2]</li>
</ul></li>
<li>Uptime
<ul><li>@uptime</li></ul></li>
<li>Fecha y hora actual 
<ul><li>$time</ul></li></li>
</ul>
</div>


</body>
</html>";

print $html;