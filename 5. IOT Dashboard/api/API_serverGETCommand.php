<?php
include_once('includes/DB/API_DB_VMS.php');
include_once('includes/DB/API_METHOUDS_VMS.php');
//{"employees":[{"firstName":"John","lastName":"Doe" },{"firstName":"Anna","lastName":"Smith" },{"firstName":"Peter","lastName":"Jones" }]}
//http://localhost/ESP32_IoT_Shield_Dashboard/api/API_serverGETCommand.php?data=apikey1234
$data = filter_input(INPUT_GET, 'data', FILTER_SANITIZE_URL);
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
    if ($data == "apikey1234")
    {
        getData($data);
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
        echo $JSON;
        flush();
        die();
    }
}



function getData($data){
$ob = new API_METHOUDS;
echo $ob->getLastCommand($data);
}


?>