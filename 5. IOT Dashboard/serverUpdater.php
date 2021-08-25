<?php
//{"employees":[{"firstName":"John","lastName":"Doe" },{"firstName":"Anna","lastName":"Smith" },{"firstName":"Peter","lastName":"Jones" }]}
header('Content-Type: text/event-stream');
header('Cache-Control: no-cache');
header('Connection: keep-alive');

//$time = date('r');
//echo "data: The server time is: {$time}\n\n";
//echo "data: 1\n\n";
$JSON = json_encode(
    array(
        'Id' => 68,
        'Device' => "ESP8266",
        'Button' => "On",
        'Status' => "Online",
        'Time' => date("Y-m-d h:i:sa"),
    )
);
echo "data: {\"result\": 0,\"Devices\":[{$JSON}]}\n\n";
flush();
die();
?>