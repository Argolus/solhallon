<?php
/*
Edit länkar när inloggad - sno generella editsidan från otn-lab
Visningsmetadata i table-fields
Rättigheter i canmove-tables 

*/

error_reporting(E_ALL & ~E_NOTICE);
ini_set('display_errors',TRUE);
ini_set('display_startup_errors',TRUE);


$result="";
$debug=true;
$debug=false;
$t_align="center";
$database="solar";
$meta_table="tables";
$meta_field="table_fields";
$log="";
$cssurl="std.css";
$xrnd=mt_rand();
if (isset($_REQUEST['t']) && ($_REQUEST['t']!='')) { $table=$_REQUEST['t']; } else { $table="none"; }
if (isset($_REQUEST['o']) && ($_REQUEST['o']!='')) { $orderby=$_REQUEST['o']; }
if (isset($_REQUEST['l']) && ($_REQUEST['l']!='')) { $lng=$_REQUEST['l']; } else { $lng="en"; }
if (isset($_REQUEST['d']) && ($_REQUEST['d']=='d')) { $dir="a"; $sortdir="desc"; } else { $sortdir="asc"; $dir="d";}
if ($lng=='sv') { $page_thema="poster"; } else $page_thema="records";
if (isset($_REQUEST['of']) && ($_REQUEST['of']!='')) { $offset=$_REQUEST['of']; } else { $offset=0; }
if (isset($_REQUEST['li']) && ($_REQUEST['li']!='')) { $limit=$_REQUEST['li']; } else { $limit=100; }
#if (isset($id)) { $orderby ="$id "; } else {}
# check that t in_array tablename, o in fieldname, l in en,sv, d in asc,desc of,li is integer


$qstr=$_SERVER['QUERY_STRING'];

$select="SELECT ";
$select.=" * ";
$join="";

if (!isset($db)) {
  require_once('/var/www/html/init_db.php');
 }
 
/*############  Get table metadata from $meta_table ############# */
 
$tablesql="SELECT * FROM $meta_table WHERE table_name='$table'";
$tableres=pg_query($db,"$tablesql");
$count=pg_num_rows($tableres);
if ($count==1) {
#  if ($lng=='sv') { $table_title=pg_fetch_result($tableres,0,"displayname_swe"); } else { $table_title=pg_fetch_result($tableres,0,"displayname_eng");}
#  $table_id=pg_fetch_result($tableres,0,"id");
} else {
  $table_title=$table;
  $log.="<p class='log'>Table not described in system!!</p>\n";
}  
pg_free_result($tableres);
$page_title="List"; if ($lng=='sv') : $page_title.="a"; endif; $page_title.=" $table_title";

echo "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n";
echo "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n";
echo "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"sv\">\n";
echo "<head>\n";
echo "<title>$page_title"; 
echo "</title>\n";
echo "<link rel='stylesheet' type='text/css' media='all' href='$cssurl'/>\n";
echo "<link rel='stylesheet' type='text/css' media='print' href='prn.css' />\n";
echo "<style>\n";
echo " a.clean { text-decoration : none; }";
echo " span.bl { font-weight : bold; font-size : larger; }\n";
echo "</style>\n";
echo "</head>\n";
echo "<body>\n";
echo "<h2 class='spaced'>$page_title</h2>";

if ($table=='none') {
 $sql="SELECT tablename FROM pg_tables WHERE schemaname='public' AND tablename!~*'_log' ORDER BY tablename";
 $tableresult=pg_query($db,"$sql");
 $cnt=pg_num_rows($tableresult);
 echo "<table id='tables'>\n";
 echo "<tr><th>tablename</th><th>Size</th><th>description</th><th>displayname</th><th>Created/Updated</th><th>id</th></tr>\n";
 for ($q=0;$q<$cnt;$q++) {
  $tname=pg_fetch_result($tableresult,$q,"tablename");
  $sql="SELECT * FROM $meta_table WHERE table_name='$tname'";
  $res=pg_query($db,"$sql");
  $numrows=pg_num_rows($res);
  $id="-";$swedsc="-";$credtm="-";$upddtm="-";$engdsc="-";$disp_eng="-";$disp_swe="-";
  if ($numrows>0) {
#   $id=pg_fetch_result($res,0,"id");
   $swedsc=pg_fetch_result($res,0,"swedescr");
#   $credtm=pg_fetch_result($res,0,"credtm");
#   $upddtm=pg_fetch_result($res,0,"upddtm");
   $engdsc=pg_fetch_result($res,0,"engdescr");
#   $disp_eng=pg_fetch_result($res,0,"displayname_eng");
#   $disp_swe=pg_fetch_result($res,0,"displayname_swe");
  }
  $csql="SELECT count(*) FROM $tname";
  $cres=pg_query($db,"$csql");
  $ccount=pg_fetch_result($cres,0,"count");
  echo "<tr><td class='lal'><a href='?t=$tname'>$tname</a></td><td>$ccount</td><td class='lal'>$engdsc</td><td class='lal'>$disp_eng</td><td style='width : 14em;'>C: $credtm U: $upddtm</td><td>$id</td></tr>";
#  <td></td><td></td><td></td><td></td></tr>\n";
 } // end for
 echo "</table>\n";
} else { // long section.....

 $sql="SELECT count(*) FROM $table";
 $cnt_result=pg_query($db,"$sql");
 $value=pg_fetch_result($cnt_result, 0, "count");
 if ($lng=='sv') echo "<p>Det finns <b>$value</b> $page_thema i datak&auml;llan."; else echo "<p>There are <b>$value</b> $page_thema in our data source.";
 echo " | <a href='?t=none'>Reset</a> table choice. | Show ";
 $lstr1="<a class='clean' href='?t=$table&amp;o=$orderby&amp;l=$lng&amp;d=".$_REQUEST['d']."&amp;li=";
 echo $lstr1."10&amp;of=$offset'><span"; if ($limit==10) { echo " class='bl'"; } echo ">10</span></a> ";
 echo $lstr1."25&amp;of=$offset'><span"; if ($limit==25) { echo " class='bl'"; } echo ">25</span></a> ";
 echo $lstr1."50&amp;of=$offset'><span"; if ($limit==50) { echo " class='bl'"; } echo ">50</span></a> ";
 echo $lstr1."100&amp;of=$offset'><span"; if ($limit==100) { echo " class='bl'"; } echo ">100</span></a> ";
 echo $lstr1."0&amp;of=$offset'><span"; if ($limit==0) { echo " class='bl'"; } echo ">All</span></a> ";
 echo "</p>\n"; 
 pg_free_result($cnt_result);
 
 $themasql="$select FROM $table $join";
 if (isset($orderby)) : $themasql.=" ORDER BY $orderby $sortdir"; endif;
 if (isset($offset)) : $themasql.=" OFFSET $offset"; endif;
 if (isset($limit)) : $themasql.=" LIMIT $limit"; endif;
 if ($debug) : echo "<p>SQL: $sql</p><p>themaSQL: $themasql</p>\n"; endif;
 $result=pg_query($db,"$themasql");
 $count=pg_num_rows($result);
 ## Continue editing from here ##
 echo "<table cellpadding='3' cellspacing='0'>\n";
 $numfields=pg_num_fields($result);
 
 echo "<tr>";
 for ($q=0;$q<$numfields;$q++) {  // create table head
  $fnam=pg_field_name($result,$q);
  $fieldlabel=str_replace("_"," ",$fnam);
  $fieldlabel=ucfirst(strtolower($fieldlabel));
  $label="";
  $label_sql="SELECT * FROM $meta_field WHERE table_id=$table_id AND fieldname='".strtolower($fnam)."'";
#  if ($debug) : echo "<span>label_sql: $label_sql</span>"; endif;
###############################################################
####   Dokumentera och avkommentera sedan nedan 2 rader    ####
###############################################################
#  $label_res=pg_query($db,"$label_sql");
#  $label_count=pg_num_rows($label_res);
  if ($label_count>0): $label=pg_fetch_result($label_res,0,"description"); pg_free_result($label_res); endif;
  echo "<th title='$label'><a href='".htmlentities($_SERVER['PHP_SELF'])."?o=$fnam&amp;d=$dir";
  if ($table!='none') : echo "&amp;t=$table"; endif;
  if ($limit) : echo "&amp;li=$limit"; endif;
  if ($offset) : echo "&amp;of=$offset"; endif;
  echo "&amp;q=$xrnd'>$fieldlabel</a></th>"; 
 }
 echo "</tr>\n";
 
 for ($i=0;$i<$count;$i++) {
   echo "<tr class='";
   if (fmod($i,2)==0) {echo "e";} else {echo "o";}
   echo "'>";
   $clas="lal";
   for ($q=0;$q<$numfields;$q++) {
    $fnam=pg_field_name($result,$q);
    $dta=pg_fetch_result($result, $i, "$fnam");
    echo "<td class='$clas'>$dta</td>"; 
   }
   echo "</tr>\n";
 }
 echo "</table>";
}
#echo "<p id='backlink'><a href='.'>Back to Internal main page</a></p>\n";
#include('validate.phps');
if ($result) : pg_free_result($result); endif;
pg_close($db);
$browse_str="<p id='browser' style='left:auto; right:auto; text-align : center;'>";

if ($offset>0) : $browse_str.="<a class='clean' href='?t=$table&amp;of=0&amp;li=$limit&amp;&o=$orderby'><< &nbsp;&nbsp;</a>"; endif;

if (($offset-$limit)>=0) {
  $browse_str.="<a class='clean' href='?";
  $browse_str.="t=$table&amp;";
  if ($orderby) : $browse_str.="o=$orderby&amp;"; endif;
  if ($lng) : $browse_str.="l=$lng&amp;"; endif;
  if ($offset) : $browse_str.="of=".($offset-$limit)."&amp;"; endif;
  if ($limit) : $browse_str.="li=$limit&amp;"; endif;
  $browse_str.="q=$xrnd'><-</a> ";
}

$browse_str.="<span id='listpos'>".($offset+1)."-";
if (($offset+$limit)<$value) { 
  $browse_str.=($offset+$limit); 
  $browse_str.="</span> ";
  $browse_str.="<a class='clean' href='?";
  $browse_str.="t=$table&amp;";
  if ($orderby) : $browse_str.="o=$orderby&amp;";  endif;
  if ($lng) : $browse_str.="l=$lng&amp;";  endif;
#  if (($offset>0) AND (($offset+$limit)<$count)) : $browse_str.="of=".($offset+$limit)."&amp;";  endif;
  if (($offset+$limit)<$value) : $browse_str.="of=".($offset+$limit)."&amp;";  endif;
  if ($limit) : $browse_str.="li=$limit&amp;";  endif;
  $browse_str.="q=$xrnd'>-></a> ";
} else { 
  $browse_str.=$value; 
  $browse_str.="</span> ";
}

$browse_str.="<a class='clean' href='?t=$table&amp;of=".($value-$limit)."&amp;li=$limit&amp;&o=$orderby'> &nbsp;&nbsp;>></a>";

$browse_str.="</p>\n";
#$browse_str.=
#$browse_str.=
if ($table) : echo "$browse_str"; endif;
echo "<p style='font-size : 9px;'>t/tab, o/ord, l/lng, d/sort</p>\n";
echo "</body>\n";
echo "</html>\n";

?>