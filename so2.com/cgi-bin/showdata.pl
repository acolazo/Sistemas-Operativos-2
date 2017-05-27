#!/usr/bin/perl
use CGI;



$var = new CGI;

open($data, '<', 'datos_meteorologicos.CSV');
@datos = <$data>;
@datos = split(",",@datos[$i]);
@datos[0] = "Numero";
close ($data);

$inicio = 0;
$i=20; 
$salto_nodo=20;
$offset_fecha = 3;
$offset_variable_fijo = $offset_fecha;

$estacion = $var -> param('estacion');
$offset_variable = $var -> param('variable');

while(@datos[$i] != $estacion){
  $i = $i + $salto_nodo;
}
$nombre_estacion = @datos[$i +1];
$nombre_variable = @datos[$inicio + $offset_variable_fijo + $offset_variable];

$message = "";
if($var -> param('operacion') == 1){

  $title="Precipitacion Diaria";
  $offset=7;
  $acumulado=@datos[$i+$offset];
  $i = $i + $salto_nodo;
  while (@datos[$i] == $estacion){
    @datos[$i + $offset_fecha] = substr @datos[$i + $offset_fecha], 0, 10;
    @datos[$i + $offset_fecha - $salto_nodo] = substr @datos[$i + $offset_fecha - $salto_nodo], 0, 10;
    if(@datos[$i+$offset_fecha] == @datos[$i+$offset_fecha-$salto_nodo]){
      $acumulado=$acumulado+@datos[$i+$offset];
      $i=$i+$salto_nodo;
    }
    else{
      $message = $message . "<p>@datos[$i-$salto_nodo+$offset_fecha] : $acumulado mm</p>" ;
      $acumulado=@datos[$i+$offset];
      $i = $i + $salto_nodo;
    }
  }
  $message = $message . "<p>@datos[$i-$salto_nodo+$offset_fecha] : $acumulado mm</p>" ;
}
elsif($var -> param('operacion') ==2){
  $title="Precipitacion Mensual";
  $offset=7;
  $acumulado=@datos[$i+$offset];
  @datos[$i + $offset_fecha] = substr @datos[$i + $offset_fecha], 3, 7;
  $i = $i + $salto_nodo;
  while (@datos[$i] == $estacion){
    @datos[$i + $offset_fecha] = substr @datos[$i + $offset_fecha], 3, 7;
    if(@datos[$i+$offset_fecha] == @datos[$i+$offset_fecha-$salto_nodo]){
      $acumulado=$acumulado+@datos[$i+$offset];
      $i=$i+$salto_nodo;
    }
    else{
      $message = $message . "<p>@datos[$i-$salto_nodo+$offset_fecha] : $acumulado mm</p>" ;
      $acumulado=@datos[$i+$offset];
      $i = $i + $salto_nodo;
    }
  }
  $message = $message . "<p>@datos[$i-$salto_nodo+$offset_fecha] : $acumulado mm</p>" ;
}
elsif($var -> param('operacion') ==3){
  $contador=0;
  $promedio=0;
  $title="Promedio" . " de la $nombre_variable";
  while (@datos[$i] == $estacion){
    $promedio = $promedio + @datos[$i + $offset_variable_fijo + $offset_variable];
    $i = $i + $salto_nodo;
    $contador++;
  }
  $promedio = $promedio / $contador;
  $message = "<p>El promedio de la $nombre_variable es $promedio</p>";
}
else{
  #descargar
  $title = "Descarga";
  $filename = 'tmp.CSV';
  open($fh, '>', $filename) or $message="<p>Could not open file '$filename' $!</p>";
  while(@datos[$i] == $estacion){
    $j=0;
    while($salto_nodo != $j){
      print $fh "@datos[$i + $j],";
      $j++;
    }
    $i=$i + $salto_nodo;
  }
  close $fh;
  $message = "<p><a href='download.pl'>Click aqui para descargar</a></p>";
  
}
$title = $title . " de la estacion $nombre_estacion";



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
<h1>$title</h1>
$message
</body>
</html>";

print $html;
