<?php
### TODO:
#   Limit data insert at nighttime - maybe better in cron?
#   This script reads data stream from serial port (MAX32) and inserts it into database

error_reporting(E_ALL);
#error_reporting(E_NONE);
ini_set('display_errors','On');
date_default_timezone_set('Europe/Stockholm');
include ('PhpSerial.php');
require_once('init_db.php');
$debug=true;
#$debug=false;  // OBS! If debugging, remember to turn of error_reporting above...

$serial = new phpSerial;
$serial->deviceSet("/dev/ttyS0");
$serial->confBaudRate(115200);
$serial->confParity("none");
$serial->confCharacterLength(8);
$serial->confStopBits(1);
$serial->deviceOpen();
sleep(3);
$serial->serialflush();
$read = $serial->readPort();
$serial->deviceClose();

$style = "<style>\ntd.data {text-align:right;}\n</style>\n";

if ($debug) {
  echo "<html>\n<head>\n".$style."<title>Solar data</title>\n</head>\n<body>\n";
  echo "<h3>Data from Solarsystem @ Dalvik NOW</h3>\n";
  echo "<br/><p>Data read from MAX32 at Pi systime: <b>".date('Y-m-d H:i:s')."</b></p><br/>\n";
  echo "<table><tbody>\n";
  echo "<tr><th>Serial</th><th>Xlated</th><th>Value</th></tr>\n";
}

$read=trim($read);
$items=explode("\n",$read);
#print_r($items);
$cycleread=false;
for ($i=0;$i<count($items);$i++) {
 if (strlen($items[$i])>1) {
   $pair=preg_replace('/\s\s+/', ' ',$items[$i]);
   $pair=explode(" ",$pair);
   if ($pair[0]=='COM_1:' || $pair[0]=='COM_2:' || $pair[0]=='COM_3:') {
    $pair[0]=$pair[1];
    $pair[1]=$pair[2];
    }
   $devsql="SELECT device_id FROM device WHERE serial_no='".$pair[0]."'";
   $devres=pg_query($db,"$devsql");
   $devcnt=pg_num_rows($devres);
   if ($devcnt==1) {
     $sql="INSERT INTO measurement (device_id,num_value,status) VALUES ((SELECT device_id FROM device WHERE serial_no='".$pair[0]."'),$pair[1],true)";
     $res=pg_query($db,"$sql");
#     if ($debug) : echo "<div>devid ok</div>"; endif;
   } else {
     $sql="INSERT INTO message (messagetext) VALUES ('Device read with unrecognized serial no: ".$pair[0]." and value:".$pair[1]."')";
     $res=pg_query($db,"$sql");
   }
   if ($debug) {
     echo "<tr><td>$pair[0]</td>";  // Code
#     echo "<td>".$decode[$pair[0]]."</td>";
     $id_sql="SELECT 'ID:'||device_id||' - '||sensorlabel(device_id) AS decode FROM device WHERE serial_no='$pair[0]'";
     $id_res=pg_query($db,"$id_sql");
     if ($id_res) {
       $id_decode=pg_fetch_result($id_res,0,"decode");
       echo "<td>$id_decode</td>"; }
     else {
       echo "<td>--</td>";
      }
     echo "<td class='data'>$pair[1]</td><td>";
     if ($res) { 
       echo "ok"; 
       if (!$cycleread) {
         $cycleread=true;
         $logsql="INSERT INTO message (messagetext) VALUES ('Data read from serial and written to db')";
         $logres=pg_query($db,"$logsql");
         if ($logres) : echo ":dbWrite"; endif;
       }
      } else { 
       echo "fail"; 
      }
     echo "</td></tr>\n";
   }
 }
}

if ($debug) {
  echo "</tbody></table>\n";
  echo "<hr/>\n";
  echo "<p>Kommande sensorer: Solinstr&aring;lning, webkamera. Temperaturer inomhus? </p>\n";
  echo "<hr/>\n";
  echo "<p>OBS! M&aring;ste kolla att dessa beteckningar st&auml;mmer helt.<br /> 
  Tryck &amp; fl&ouml;den ej inkopplade &auml;nnu, 
  Pt-givare ej omr&auml;knade & kalibrerade. <br/>
  OBS, ibland f&aring;r man inte alla v&auml;rden, ibland kommer de mer &auml;n en g&aring;ng - buffert?<br /><br />
  OBS, det saknas vissa &ouml;vers&auml;ttningar av koder fr&aring;n sensorer som s&auml;llan rapporterar. D&aring; ser man felet<br/>
  \"<b>Notice</b>: Undefined index: _____ in <b>/var/www/solardata.php</b> on line <b>65</b>\" f&ouml;re tabellen.<br />
  D&aring; &auml;r det bra ifall man <a href='mailto:Johan.Backman@biol.lu.se'>mailar Johan</a> och p&aring;pekar detta.</p>
  <p>T&auml;nker logga data fr&aring;n solkrets, tank, rum etc. h&ouml;gst varje minut, PVVdata med sekunduppl&ouml;sning<br /> men 
  bara vid \"aktivitet\", dvs. n&auml;r man f&ouml;rbrukar varmvatten.</p><hr/>";
  echo "</body></html>";
} // debug

if ($res) : pg_free_result($res); endif;
if ($db) : pg_close($db); endif;
?>