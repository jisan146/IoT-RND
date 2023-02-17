<?php
error_reporting(0);
session_start();
$_SESSION["a"] =$_SESSION["a"]+1;
$_SESSION["phone"];
$_SESSION["sms"];
function get_sms() 
	{
	
		$url = 'http://www.srbuildersbd.com:8087/Default.aspx';
		$data = array('p' => 'Samsung123459S', 'Message' => 'hi test');

		// use key 'http' even if you send the request to https://...
		$options = array(
						'http' => array(
       
											'method'  => 'POST',
											'content' => http_build_query($data)
										)
						);
		$context  = stream_context_create($options);
		$result = file_get_contents($url, false, $context);
		if ($result === FALSE) { /* Handle error */ }


		$phone=substr($result,0,11);
		
		$sms=substr($result,11);
		
		
		$_SESSION["phone"]=$phone;
		$_SESSION["sms"]=$sms;
	}
	

function send_sms() 
	{
	
		$url = 'http://192.168.0.105:1688/services/api/messaging';
		$data = array('to' => $_SESSION["phone"], 'Message' => $_SESSION["sms"]);

		// use key 'http' even if you send the request to https://...
		$options = array(
							'http' => array(
       
											'method'  => 'POST',
											'content' => http_build_query($data)
											)
						);
		$context  = stream_context_create($options);
		$result = file_get_contents($url, false, $context);
		if ($result === FALSE) { /* Handle error */ }
		echo $result;
		
	}



if ($_SESSION["a"] <10)
						{
							get_sms();send_sms();
						}
						else if ($_SESSION["a"]>30)
						{
							$_SESSION["a"]=0;
						}
?>


<script>
setTimeout(function(){
   window.location.reload(1);
}, 3000);
</script>

