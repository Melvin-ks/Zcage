<?php
use MYPDF as GlobalMYPDF;

$Company=$_GET['unname'];
$command='python3 console.py call IndexTable 0x1f494c56c3ad1e6738f3500d19499cd3541160ea select ';
$para='"'.$Company.'"';
exec($command,$para,$array);
$str=$array[6];
$str_RE=<<<EOD
'[a-z,0-9,A-Z,_]+'
EOD;
preg_match_all($str_RE,$str,$arr,PREG_SET_ORDER);
$inc=count($arr);
$command2='python3 console.py call IndexTable 0x1f494c56c3ad1e6738f3500d19499cd3541160ea select ';
// Include the main TCPDF library (search for installation path).
require_once('/www/wwwroot/www.zcage.com/TCPDF-main/TCPDF-main/tcpdf.php');
class MYPDF extends TCPDF {
	//Page header
	public function Header() {
		// get the current page break margin
		$bMargin = $this->getBreakMargin();
		// get current auto-page-break mode
		$auto_page_break = $this->AutoPageBreak;
		// disable auto-page-break
		$this->setAutoPageBreak(false, 0);
		// set bacground image
		$img_file = K_PATH_IMAGES.'niubi.jpeg';
		$this->Image($img_file, 0, 0, 210, 297, '', '', '', false, 300, '', false, false, 0);
		// restore auto-page-break status
		$this->setAutoPageBreak($auto_page_break, $bMargin);
		// set the starting point for the page content
		$this->setPageMark();
	}
}

// create new PDF document
$pdf = new MYPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false);

$pdf->setCreator(PDF_CREATOR);
$pdf->setAuthor('niubi team');
$pdf->setTitle('CERTIFICATION OF PLONK');
$pdf->setSubject('TCPDF Tutorial');
$pdf->setKeywords('ZK-SNARK, certification, automatic, Blockchain, niubi');

// set default header data
$pdf->setHeaderData("CERT.png", 40, "CERTIFICATION OF PLONK", PDF_HEADER_STRING, array(0,64,255), array(0,64,128));
$pdf->setFooterData(array(0,64,0), array(0,64,128));

// set header and footer fonts
$pdf->setHeaderFont(Array(PDF_FONT_NAME_MAIN, '', PDF_FONT_SIZE_MAIN));
$pdf->setFooterFont(Array(PDF_FONT_NAME_DATA, '', PDF_FONT_SIZE_DATA));

// set default monospaced font
$pdf->setDefaultMonospacedFont(PDF_FONT_MONOSPACED);

// set margins
$pdf->setMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
$pdf->setHeaderMargin(PDF_MARGIN_HEADER);
$pdf->setFooterMargin(PDF_MARGIN_FOOTER);

// set auto page breaks
$pdf->setAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);

// set image scale factor
$pdf->setImageScale(PDF_IMAGE_SCALE_RATIO);

// set some language-dependent strings (optional)
if (@file_exists(dirname(__FILE__).'/lang/eng.php')) {
	require_once(dirname(__FILE__).'/lang/eng.php');
	$pdf->setLanguageArray($l);
}

// ---------------------------------------------------------

// set default font subsetting mode
$pdf->setFontSubsetting(true);

// helvetica or times to reduce file size.
$pdf->setFont('dejavusans', '', 14, '', true);
// Add a page
// This method has several options, check the source code documentation for more information.
$pdf->AddPage();

// set text shadow effect
$pdf->setTextShadow(array('enabled'=>true, 'depth_w'=>0.2, 'depth_h'=>0.2, 'color'=>array(196,196,196), 'opacity'=>1, 'blend_mode'=>'Normal'));
$htmlb=<<<EOD
<br><br><br>
EOD;
$pdf->writeHTMLCell(0, 0, '', '', $htmlb, 0, 1, 0, true, '', true);
$html1 = <<<EOD
<h1 style="text-align:center;">Congratulations you have just created a certification of ZK-SNARK! </h1>
<br>
EOD;
$pdf->writeHTMLCell(0, 0, '', '', $html1, 0, 1, 0, true, '', true);
$info="<br><br><br><br><br><br><i>The Company being Queried: ".$Company."</i>";
$pdf->writeHTMLCell(0, 0, '', '', $info, 0, 1, 0, true, '', true);
$tbl = <<<EOF
<br><br><br>
<table border="1" cellpadding="2" cellspacing="2" align="center">
 <tr nobr="true">
  <th colspan="3">CERTIFICATIONS</th>
 </tr>
 <tr nobr="true">
  <td>Company Name</td>
  <td>Range</td>
  <td>Verification results</td>
 </tr>
EOF;
for($i=1;$i<=$inc;$i+=2)
{
    $parax='"'.$arr[$i][0].'"';
    exec($command2.$parax,$re);
    $result=$re[?];
    $str_RE2=<<<EOD
[a-z,0-9,-]+
EOD;
    preg_match_all($str_RE2,$result,$items,PREG_SET_ORDER);
    $item=$items[2][0];
    $range=$items[3][0];
    $situation=$items[5][0];
    $tbl.=<<<EOF
    <tr nobr="true">
    <td>$item</td>
    <td>$range</td>
    <td>$situation</td>
    </tr>
EOF;
}
$tbl.='</table>';
$html6=<<<EOD
<i>you can check more details about us through the following link:<br><br><a href="about.html" style="text-decoration:none;background-color:#0ABAB5;color:black;">&nbsp;<span style="color:black;">About</span><span style="color:white;"> US</span>&nbsp;</a></i>
EOD;
$pdf->writeHTML($tbl, true, false, false, false, '');
$pdf->writeHTMLCell(0, 0, '', '', $html6, 0, 1, 0, true, '', true);
$pdf->Image('TCPDF-main/TCPDF-main/examples/images/CE.png', 150, 210, 0, 0, 'PNG', '', '', true, 150, '', false, false, 1, false, false, false);
$pdf->Output('The Certificaiton of '.$Company.' company.pdf', 'I');


//============================================================+
// END OF FILE
//============================================================+
