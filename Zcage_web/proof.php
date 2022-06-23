<?php
use MYPDF as GlobalMYPDF;
$Company1="APPLE";
require_once('/www/wwwroot/www.zcage.com/TCPDF-main/TCPDF-main/tcpdf.php');//tcpdf文件，在TCPDF-main文件夹里
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
$pdf = new MYPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true, 'UTF-8', false);
$secret=$_GET['val'];
$command='./prove ';
$para=$_GET['val'].' '.$_GET['min'].' '.$_GET['max'];
exec($command.$para);
exec('jq . snark.json',$array);
$pdf->setCreator(PDF_CREATOR);
$pdf->setAuthor('niubi team');
$pdf->setTitle('PROOF OF PLONK');
$pdf->setSubject('TCPDF Tutorial');
$pdf->setKeywords('ZK-SNARK, proof, automatic, Blockchain, niubi');

// set default header data
$pdf->setHeaderData("CERT.png", 40, "PROOF OF PLONK", PDF_HEADER_STRING, array(0,64,255), array(0,64,128));
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

// Set font
// dejavusans is a UTF-8 Unicode font, if you only need to
// print standard ASCII chars, you can use core fonts like
// helvetica or times to reduce file size.
$pdf->setFont('dejavusans', '', 14, '', true);

// Add a page
// This method has several options, check the source code documentation for more information.
$pdf->AddPage();

// set text shadow effect
$pdf->setTextShadow(array('enabled'=>true, 'depth_w'=>0.2, 'depth_h'=>0.2, 'color'=>array(196,196,196), 'opacity'=>1, 'blend_mode'=>'Normal'));

$min=$array[2];
$max=$array[3];
$info=<<<EOF
                <h3>PI:[$min$max]<br></h3>
EOF;
$pdf->writeHTMLCell(0, 0, '', '', $info, 0, 1, 0, true, '', true);
$info1=$array[5];
$info2=$array[6];
$info3=$array[7];
$info4=$array[8];
$info5=$array[9];
$info6=$array[10];
$info7=$array[11];
$info8=$array[12];
$info9=$array[13];
$info10=$array[14];
$info11=$array[15];
$info12=$array[16];
$info13=$array[17];
$info14=$array[18];
$info15=$array[19];

$tbl = <<<EOF
<table border="1" cellpadding="2" cellspacing="2" nobr="true">
 <tr>
  <th colspan="1" align="center">PROOFS TABLE</th>
 </tr>
 <tr>
  <td>$info1</td>
 </tr>
 <tr>
  <td>$info2</td>
 </tr>
 <tr>
  <td>$info3</td>
 </tr>
  <tr>
  <td>$info4</td>
 </tr>
  <tr>
  <td>$info5</td>
 </tr>
  <tr>
  <td>$info6</td>
 </tr>
  <tr>
  <td>$info7</td>
 </tr>
  <tr>
  <td>$info8</td>
 </tr>
  <tr>
  <td>$info9</td>
 </tr>
  <tr>
  <td>$info10</td>
 </tr>
  <tr>
  <td>$info11</td>
 </tr>
   <tr>
  <td>$info12</td>
 </tr>
   <tr>
  <td>$info13</td>
 </tr>
   <tr>
  <td>$info14</td>
 </tr>
   <tr>
  <td>$info15</td>
 </tr>

</table>
EOF;

$html6=<<<EOD
<i><a href="power_grid.php" style="text-decoration:none;background-color:#0ABAB5;color:black;">&nbsp;<span style="color:white;"> VERIFICATION</span>&nbsp;</a></i>
EOD;
$pdf->writeHTML($tbl, true, false, false, false, '');
$pdf->writeHTMLCell(0, 0, '', '', $html6, 0, 1, 0, true, '', true);

// ---------------------------------------------------------
//$pdf->Image()
//TCPDF-main\TCPDF-main\examples\images
// Close and output PDF document
// This method has several options, check the source code documentation for more information.

$pdf->Output('The Proof of '.$Company1.' company.pdf', 'I');


//============================================================+
// END OF FILE
//============================================================+
