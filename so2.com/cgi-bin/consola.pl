#!/usr/bin/perl
$html = "Content-Type: text/html

<html>
<head>
<!-- Latest compiled and minified CSS -->
<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' integrity='sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u' crossorigin='anonymous'>

<!-- Optional theme -->
<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css' integrity='sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp' crossorigin='anonymous'>

<!-- Latest compiled and minified JavaScript -->
<title>Consola</title>
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
<h1>Consola</h1>
</div>


</body>
</html>";

print $html;