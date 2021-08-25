<?php
//{"employees":[{"firstName":"John","lastName":"Doe" },{"firstName":"Anna","lastName":"Smith" },{"firstName":"Peter","lastName":"Jones" }]}
$data = filter_input(INPUT_GET, 'data');
if(isset($data))
{
    updateData($data);
}
else
{
    echo json_encode(
        array(
            'result' => 0,
            'Message' => "No Data",
            'date' => date("Y-m-d h:i:sa")
        )
    );
    die();
}

function updateData($data)
{
    if ($data == "ON")
    {
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
                'Type' => "Command",
                'Time' => date("Y-m-d h:i:sa"),
            )
        );
        //echo "data: {\"result\": 0,\"Devices\":[{$JSON}]}\n\n";
        echo "data: {$JSON}\n\n";
        flush();
        die();
    }
    else if ($data == "OFF")
    {
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
                'Button' => "OFF",
                'Type' => "Command",
                'Time' => date("Y-m-d h:i:sa"),
            )
        );
        //echo "data: {\"result\": 0,\"Devices\":[{$JSON}]}\n\n";
        echo "data: {$JSON}\n\n";
        flush();
        die();
    }
    else
    {
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
                'Button' => "Invalid",
                'Type' => "Command",
                'Time' => date("Y-m-d h:i:sa"),
            )
        );
        //echo "data: {\"result\": 0,\"Devices\":[{$JSON}]}\n\n";
        echo "data: {$JSON}\n\n";
        flush();
        die();
    }
}


?>