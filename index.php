<?php
$url = 'http://192.168.0.105:1688/services/api/messaging';
$data = array('to' => '01687602005', 'Message' => 'hi test');

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



?>


<script>
setTimeout(function(){
   window.location.reload(1);
}, 5000);
</script>