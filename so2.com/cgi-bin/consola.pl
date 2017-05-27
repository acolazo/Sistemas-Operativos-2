#!/usr/bin/perl

$html = "Content-Type: text/html

<html>
<head>
<!-- Latest compiled and minified CSS -->
<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css' integrity='sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u' crossorigin='anonymous'>

<!-- Optional theme -->
<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css' integrity='sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp' crossorigin='anonymous'>

<!-- Latest compiled and minified JavaScript -->
<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>
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



<div class='panel-body text-left'>
<form action='showdata.pl'>
<label>Estacion</label>
<select name='estacion'>
<option value='30135'>Yacanto Norte - E01</option>
<option value='30057'>MagyaCba 60 cuadras</option>
<option value='30061'>MagyaCba La Cumbrecita</option>
<option value='30099'>Cerro Obero - Capilla del Monte</option>
<option value='30069'>Magyacba Oliva</option>
</select>
<label>Operacion</label>
<select name='operacion'>
<option value='1'>Precipitacion Diaria</option>
<option value='2'>Precipitacion Mensual</option>
<option value='3'>Promedio</option>
<option value='4'>Descargar</option>
</select>
<label>Variable a promediar</label>
<select name='variable'>
<option value='1'>Temperatura</option>
<option value='2'>Humedad</option>
<option value='3'>Punto de Rocio</option>
<option value='4'>Precipitacion</option>
<option value='5'>Velocidad Viento</option>
<option value='7'>Rafaga Maxima</option>
<option value='8'>Presion</option>
<option value='9'>Radiacion Solar</option>
<option value='10'>Temperatura Suelo 1</option>
<option value='11'>Temperatura Suelo 2</option>
<option value='12'>Temperatura Suelo 3</option>
<option value='13'>Humedad Suelo 1</option>
<option value='14'>Humedad Suelo 2</option>
<option value='15'>Humedad Suelo 3</option>
<option value='16'>Humedad de Hoja</option>
</select>
<input type='submit' value='Ejecutar'>
</form>
 
<p>@datos[21]</p>

</body>
</html>";

print $html;