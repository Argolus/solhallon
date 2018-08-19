<?php
echo "<html><head><title></title>\n";
echo "<style>";
echo " #container {position:relative;}\n";
echo " #bgimage {position:absolute; left:0; top:10;}";
$lbl_offset_left=0;
$lbl_offset_top=0;
echo " .temp_label { z-index:100; position:absolute; color:black; font-size:12px; font-weight:normal; font-family:Verdana, Arial, sans-serif; background-color:#ddd;}\n";
echo " #id15 { left:".(350+$lbl_offset_left)."px; top:".(320+$lbl_offset_top)."px;}\n"; // temp vrum
echo " #id46 { left:".(350+$lbl_offset_left)."px; top:".(100+$lbl_offset_top)."px;}\n"; // temp kök
echo " #id130 { left:".(580+$lbl_offset_left)."px; top:".(320+$lbl_offset_top)."px;}\n"; // temp biorum
echo " #id169 { left:".(70+$lbl_offset_left)."px; top:".(80+$lbl_offset_top)."px;}\n"; // temp badrum
echo " #id29 { left:".(420+$lbl_offset_left)."px; top:".(400+$lbl_offset_top)."px; background-color:#555; color:#ddd;}\n"; // tempkg
echo " #id26 { left:".(500+$lbl_offset_left)."px; top:".(480+$lbl_offset_top)."px;}\n"; // temp sydvägg
echo " #id47 { left:".(350+$lbl_offset_left)."px; top:".(10+$lbl_offset_top)."px;}\n"; // temp norrvägg
echo " #id27 { left:".(600+$lbl_offset_left)."px; top:".(400+$lbl_offset_top)."px; background-color:#555; color:#ddd;}\n"; // temp verkstad/källare
echo " #id28 { left:".(500+$lbl_offset_left)."px; top:".(430+$lbl_offset_top)."px; background-color:#fc0; color:#777;}\n"; // temp i acktank
echo " #id45 { left:".(379+$lbl_offset_left)."px; top:".(57+$lbl_offset_top)."px; font-size:9px; background-color:#fff; color:#999;}\n"; // temp kyl
echo "\n";
echo "\n";
echo " #cam121 { position:absolute; left:100px; top:400px;}\n";
echo "</style>";
echo "</head>\n";
echo "<body>\n";

#$alldata = file_get_contents('http://10.0.0.106:3480/data_request?id=user_data&output_format=xml');

#$data = file_get_contents('http://10.0.0.106:3480/data_request?id=status&output_format=xml&DeviceNum=136'); // temp vrum
$temp_id15 = file_get_contents('http://10.0.0.106:3480/data_request?id=variableget&DeviceNum=51&serviceId=urn:upnp-org:serviceId:TemperatureSensor1&Variable=CurrentTemperature'); // temp vrum
$temp_id45 = file_get_contents('http://10.0.0.106:3480/data_request?id=variableget&DeviceNum=127&serviceId=urn:upnp-org:serviceId:TemperatureSensor1&Variable=CurrentTemperature'); // temp i kyl
$temp_id46 = file_get_contents('http://10.0.0.106:3480/data_request?id=variableget&DeviceNum=128&serviceId=urn:upnp-org:serviceId:TemperatureSensor1&Variable=CurrentTemperature'); // temp kök
$temp_id47 = file_get_contents('http://10.0.0.106:3480/data_request?id=variableget&DeviceNum=129&serviceId=urn:upnp-org:serviceId:TemperatureSensor1&Variable=CurrentTemperature'); // temp norrvägg
$temp_id130 = file_get_contents('http://10.0.0.106:3480/data_request?id=variableget&DeviceNum=64&serviceId=urn:upnp-org:serviceId:TemperatureSensor1&Variable=CurrentTemperature'); // temp biorum
$temp_id169 = file_get_contents('http://10.0.0.106:3480/data_request?id=variableget&DeviceNum=56&serviceId=urn:upnp-org:serviceId:TemperatureSensor1&Variable=CurrentTemperature'); // temp badrum
$temp_id29 = file_get_contents('http://10.0.0.106:3480/data_request?id=variableget&DeviceNum=17&serviceId=urn:upnp-org:serviceId:TemperatureSensor1&Variable=CurrentTemperature'); // temp krypgrund
$temp_id26 = file_get_contents('http://10.0.0.106:3480/data_request?id=variableget&DeviceNum=14&serviceId=urn:upnp-org:serviceId:TemperatureSensor1&Variable=CurrentTemperature'); // temp sydvägg
$temp_id27 = file_get_contents('http://10.0.0.106:3480/data_request?id=variableget&DeviceNum=15&serviceId=urn:upnp-org:serviceId:TemperatureSensor1&Variable=CurrentTemperature'); // temp källare
$temp_id28 = file_get_contents('http://10.0.0.106:3480/data_request?id=variableget&DeviceNum=16&serviceId=urn:upnp-org:serviceId:TemperatureSensor1&Variable=CurrentTemperature'); // temp acktank

$temp_id45=round($temp_id45,1);
$temp_id28=round($temp_id28,1);

#$cmd = "wget -O /tmp/file.jpg \"http://10.0.0.106:3480/data_request?id=request_image&res=low&cam=121\" ";  // får inte low res att fungera.....
system($cmd,$retval);

echo "<div id='container'><img id='bgimage' src='./images/house_up.jpg' />";
#echo "<img id='cam121' src='file.jpg' />";
echo "</div>\n";
echo "<p class='temp_label' id='id15' title='Temperature vardagsrum'>$temp_id15&deg;C</p>\n";
echo "<p class='temp_label' id='id45' title='Temperature i kyl'>$temp_id45&deg;</p>\n";
echo "<p class='temp_label' id='id46' title='Temperature kök'>$temp_id46&deg;C</p>\n";
echo "<p class='temp_label' id='id130' title='Temperature biorum'>$temp_id130&deg;C</p>\n";
echo "<p class='temp_label' id='id169' title='Temperature badrum'>$temp_id127&deg;C</p>\n";
echo "<p class='temp_label' id='id29' title='Temperature krypgrund'>$temp_id29&deg;C</p>\n";
echo "<p class='temp_label' id='id26' title='Temperature ute sydvägg'>$temp_id26&deg;C</p>\n";
echo "<p class='temp_label' id='id27' title='Temperature verkstad/k&auml;llare'>$temp_id27&deg;C</p>\n";
echo "<p class='temp_label' id='id28' title='Temperature acktank'>$temp_id28&deg;C</p>\n";
echo "<p class='temp_label' id='id47' title='Temperature ute norrvägg'>$temp_id47&deg;C</p>\n";
#echo "$alldata\n";
#echo "<p>$data</p>\n";
#echo "<p style='position:absolute; left:10px; top:700px;'> $cmd</p>\n";
#echo "<p style='position:absolute; left:10px; top:730px;'> $retval</p>\n";
echo "</body>\n";
echo "</html>\n";
?>