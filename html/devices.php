
###### Not ready #######
##  But what is it meant to do really? ##

<?php
error_reporting(E_ALL);ini_set('display_errors','On');
require_once('init_db.php');

$sql="SELECT device_id,serial_no,sensorlabel(device_id) FROM device JOIN sensor USING (device_id)";

$res=pg_query($db,"$sql");
#$t2=microtime(true)-$time_start;
$cnt=pg_num_rows($res);
echo "<html><head><title>Current solar data</title>\n";
echo "<style>\n";

echo "</style>\n</head>\n<body>\n";
#$t3=microtime(true)-$time_start;
echo "<div id='container'><img id='bgimage' src='./images/plain_drawing.jpg' /></div>\n";
for ($i=0;$i<$cnt;$i++) {
  $devid=pg_fetch_result($res,$i,"device_id");
  $serial_no=pg_fetch_result($res,$i,"serial_no");
  $newlabel=pg_fetch_result($res,$i,"sensorlabel");
  $num_value=pg_fetch_result($res,$i,"num_value");
  $credtm=pg_fetch_result($res,$i,"credtm");
  $num_value_disp=round($num_value,0);
  echo "<p class='vid' id='lbl_$devid' title='$newlabel \n($serial_no) \nHires:$num_value\n[ID:$devid] \nLast read: $credtm'>$num_value_disp";
  $sqlunit="SELECT unit FROM sensor JOIN quantity ON (sensor.quantity=quantity.quantity_id) WHERE device_id=$devid";
  $resunit=pg_query($db,"$sqlunit");
  $countunit=pg_num_rows($resunit);
  if ($countunit==1) {
   $unit=pg_fetch_result($resunit,0,"unit");
   echo " $unit</p>\n";
  } else {
   echo " ---</p>\n";
  }
}
#$t4=microtime(true)-$time_start;
echo "<p class='spinimg' id='solpump'><img src='spin3.gif' /></p>\n";
echo "<p class='spinimg' id='pvvpump'><img src='anim4.gif' /></p>\n";
echo "<p></p>\n<h2>Dalvik solv&auml;rme aktuella data</h2>\n";
echo "<p>Data uppdateras automatiskt var 10:e minut</p>";
pg_free_result($res); pg_close($db);
#echo "<div id='debugdiv'>\n";
#$t5=microtime(true)-$time_start;
#echo "<p>t1: ".$t1."</p>\n";
#echo "<p>t2: ".$t2."</p>\n";
#echo "<p>t3: ".$t3."</p>\n";
#echo "<p>t4: ".$t4."</p>\n";
#echo "<p>t5: ".$t5."</p>\n";
#echo "</div>\n";
#echo "<div id='footer'>\n";
include "footer.php";
#echo "</div>\n";
echo "</body></html>"; 
#
?>