<?php
#error_reporting(E_ALL);ini_set('display_errors','On');header('refresh:60;');

function date_range($first, $last, $step = '+1 day', $output_format = 'Y-m-d' ) {

    $dates = array();
    $current = strtotime($first);
    $last = strtotime($last);

    while( $current <= $last ) {

        $dates[] = date($output_format, $current);
        $current = strtotime($step, $current);
    }

    return $dates;
}

$date=date("Y-m-d"); $d=$date;
$d=$_REQUEST['d']; if((strlen($d)>10) OR (strlen($d)<1)) : $d=$date; endif;
require_once('init_db.php');

$page_title="Dalvik One solar system temperatures";
$graph_title="Solar temperatures";
#$devid="2";
# Denna SQL borde ge en temperatur / timme för en device hitills innevarande dag. 
# Temp. är maxtemp mellan minutrarna 10-50 (verkar det vettigt?) Gör om till en funktion...
#$valSQL="SELECT max(num_value), date_trunc('hour',credtm) AS ts FROM measurement WHERE 
#         date_part('minute',credtm)<50 AND date_part('minute',credtm)>10 AND 
#         credtm > date_trunc('day',NOW()) AND device_id=$devid GROUP BY ts ORDER BY ts";
# Man kan ta fram vilka timmar hittills 8och därmed timintervall att plotta) genom
# "SELECT DISTINCT date_part('hour',credtm) FROM measurement WHERE credtm>date_trunc('day',NOW())";
# men det borde inte spela nån roll, man kan ha fasta timintervall och bara låta det fylla på

### make loop here that extracts data for a set of sensors

$devids = array(1,2,29,45,11,18,13); // 1=SPO_T1, 2=ATL_T1, 29=ATH_T4, 45=SPI_T1, 11=HTI_T1, 18=VOD_T1, 13=HTO_T1
for ($i=0;$i<count($devids);$i++){
 /*  $valSQL="SELECT max(num_value) AS val, date_trunc('hour',credtm) AS ts FROM measurement WHERE 
            date_part('minute',credtm)<50 AND date_part('minute',credtm)>10 AND 
            credtm > date_trunc('day',NOW()) AND device_id=$devids[$i] GROUP BY ts ORDER BY ts";
 */
$stopdate=date_create("$d");
date_add($stopdate, date_interval_create_from_date_string('1 day'));
#$end_day
   $valSQL="SELECT avg(num_value) AS val, date_trunc('hour',credtm) AS ts FROM measurement WHERE 
            date_part('minute',credtm)<50 AND date_part('minute',credtm)>10 AND 
            credtm > '".$d."' AND ";
   $valSQL.="credtm < date '".$d."' + interval '1 day' AND ";
   $valSQL.="device_id=$devids[$i] GROUP BY ts ORDER BY ts";

   $res=pg_query($db,"$valSQL");
   $count=pg_num_rows($res);
#   $t_str="";
   for ($j=0;$j<$count;$j++) {
      $t=pg_fetch_result($res,$j,"val");
      $t_str[$i].=", ".round($t,1);
   }
   $t_str[$i]=ltrim($t_str[$i],",");
 } // end for

$t_SPO_T1=ltrim($t_str[0]); // index 0 is ID:1 is SPO_T1 is out from last SP
$t_bottom=ltrim($t_str[1]); // index 1 is ID:2 is ATL_T1 is bottom of tank
$t_top=ltrim($t_str[2]); // index 2 is ID:29 is ATH_T4 is top of tank
$t_SPI_T1=ltrim($t_str[3]); // index 3 is ID:45 is SPI_T1 is in to first SP
$t_HTI_T1=ltrim($t_str[4]); // index 4 is ID:11 is HTI_T1 is heat exc in from tank
$t_VOD_T1=ltrim($t_str[5]); // index 5 is ID:18 is VOD_T1 is output dump pipe
$t_HTO_T1=ltrim($t_str[6]); // index 5 is ID:13 is HTO_T1 is heat exc out to tank botton
###  end loop here

echo"
<html><head>
<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>
<title>$page_title</title>

<script type='text/javascript' src='https://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js'></script>
<style type='text/css'> ${demo.css}</style>
<script type='text/javascript'>
$(function () {
    $('#container').highcharts({
        title: {
            text: '$graph_title',
            x: -20 //center
        },
        subtitle: {
            text: 'Today $date',
            x: -20
        },
        xAxis: {
            categories: ['06-07','07-08', '08-09', '09-10', '10-11', '11-12', '12-13',
                '13-14', '14-15', '15-16', '16-17', '17-18', '18-19','19-20','20-21','21-22']
        },
        yAxis: {
            title: {
                text: 'Temperature (°C)'
            },
            plotLines: [{
                value: 0,
                width: 1,
                color: '#808080'
            }]
        },
        tooltip: {
            valueSuffix: '°C'
        },
        legend: {
            layout: 'vertical',
            align: 'right',
            verticalAlign: 'middle',
            borderWidth: 0
        },
        series: [{
            name: 'Tank top',
            data: [$t_top]
        }, {
            name: 'Tank bottom',
            data: [$t_bottom]
        }, {
            name: 'Solar Panel in',
            data: [$t_SPI_T1]
        }, {


            name: 'Solar Panel out',
            data: [$t_SPO_T1]
        }, {
            name: 'Heatexc tank in',
            data: [$t_HTI_T1]
        }, {
            name: 'Heat dump pipe',
            data: [$t_VOD_T1]
        }, {
            name: 'Heatexc out to tank',
            data: [$t_HTO_T1]
        }]
    });
});
		</script>
	</head>
	<body>
<script src='/graf/js/highcharts.js'></script>
<script src='/graf/js/modules/exporting.js'></script>

<div id='container' style='min-width: 310px; height: 400px; margin: 0 auto'></div>\n";
#echo "<pre><p id='test'>$t_str</p></pre>\n";
$datearr=date_range("2016-04-16",date("Y-m-d"));
echo "<select name='date' onchange=\"location=this.options[this.selectedIndex].value;\">\n";
$num=count($datearr);
for ($i=0;$i<($num);$i++) {
 echo "<option value='$PHP_SELF?r=r&amp;d=$datearr[$i]'";
# om datumvariabeln inte är satt, sätt den till idag
 if ($d==$datearr[$i]) : echo " selected='selected'"; endif;
 echo">$datearr[$i]</option>\n";
}
#echo "<option value='".date("Y-m-d")."' selected='selected'>".date("Y-m-d")."</option>\n";
echo "</select>\n";
#print_r($datearr);
#echo "<p>$valSQL  --- </p>\n";
echo "   </body>
</html>";
?>