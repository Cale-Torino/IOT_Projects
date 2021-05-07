<?php

// {
//     "Result":"0",
//     "UID":"450b87a4cc154179a5e1d6e3b1e31ec5",
//     "Device_Modal":"NODEMCU-CH340",
//     "Device_Type":"NodeMCU_Lua",
//     "Device_Catagory":"ESP8266_Dev_Board",
//     "Time":"2021-2-11-00:00:12",
//     "Temp":"-127.00",
//     "LDR":"1",
//     "Volts":"-0.34",
//     "WiFi_Stations":[
//        {
//           "SSID":"inst",
//           "CH":"1",
//           "ENC":"WPA-WPA2-PSK",
//           "RSSI":"-58",
//           "PER":"84",
//           "MAC":"F0:B4:D2:08:BB:46"
//        },
//        {
//           "SSID":"EZVIZ",
//           "CH":"5",
//           "ENC":"WPA2-PSK",
//           "RSSI":"-67",
//           "PER":"66",
//           "MAC":"1C:67:58:44:4B:CB"
//        },
//        {
//           "SSID":"instl",
//           "CH":"6",
//           "ENC":"WPA-WPA2-PSK",
//           "RSSI":"-94",
//           "PER":"12",
//           "MAC":"84:D8:1B:C7:24:16"
//        }
//     ],
//     "Mac_Address":"10.0.0.8",
//     "On_Time":"410230119"
//  }

$API_KEY = filter_input(INPUT_GET, 'API_KEY', FILTER_SANITIZE_URL);
$USERNAME = filter_input(INPUT_GET, 'USERNAME', FILTER_SANITIZE_URL);
$DATA = filter_input(INPUT_GET, 'DATA', FILTER_SANITIZE_URL);
//http://127.0.0.1/a6/in_a6.php?API_KEY=asdflrtjT&USERNAME=a6in&DATA=testing_data
//http://64.42.181.26/a6/in_a6.php?API_KEY=asdflrtjT&USERNAME=a6in&DATA=testing_data
if($USERNAME =='a6in' && $API_KEY=='asdflrtjT'){
    A6_IN($DATA);die();
}else{
    echo json_encode(
        array(
            'result'  => 1,
            'message' =>'api key is invalid!'
        )
    );
    die();
}

function A6_IN($DATA){
    if($DATA)
    {
    $BYTES = mb_strlen($DATA, '8bit');

    // $decoder = json_decode($DATA);
    // $Result = $decoder->Result;
    // $UID = $decoder->UID;
    // $Device_Modal = $decoder->Device_Modal;
    // $Device_Type = $decoder->Device_Type;
    // $Device_Catagory = $decoder->Device_Catagory;
    // $Time = $decoder->Time;
    // $Temp = $decoder->Temp;
    // $LDR = $decoder->LDR;
    // $Volts = $decoder->Volts;
    // $Mac_Address = $decoder->Mac_Address;
    // $On_Time = $decoder->On_Time;
    // $WiFi = $decoder->WiFi_Stations;
    // foreach($WiFi as $key => $WiFi_Stations)
    // {
    //     $SSID = $WiFi_Stations->SSID;
    //     $CH = $WiFi_Stations->CH;
    //     $ENC = $WiFi_Stations->ENC;
    //     $RSSI = $WiFi_Stations->RSSI;
    //     $PER = $WiFi_Stations->PER;
    //     $MAC = $WiFi_Stations->MAC;

    //     echo 'Test='.$SSID.'<br>'.$CH.'<br>'.$ENC.'<br>'.$RSSI.'<br>'.$PER.'<br>'.$MAC.'<br>';
    // }


    //Login attempts Log
     $log  = "Data:".$DATA.PHP_EOL.'Date:'.date("F j, Y, g:i a").PHP_EOL.'Bytes:'.$BYTES.PHP_EOL."------------------------->".PHP_EOL;
     file_put_contents('./data_'.date("n.j.Y").'.log', $log, FILE_APPEND);

     echo json_encode(
        array(
            'result'  => 0,
            'message' =>'data inserted successfully',
            'data' =>''.$DATA.'',
            'bytes' =>''.$BYTES.'',
            'time' =>''.date("F-j-Y_g-i_a").''
        )
    );

    die();
    }
    else
    {
        echo json_encode(
            array(
                'result'  => 2,
                'message' =>'no data inserted'
            )
        );
        die();
    }
    
    }
?>