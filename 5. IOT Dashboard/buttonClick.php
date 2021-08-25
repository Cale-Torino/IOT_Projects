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
            'result' => 1,
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
        echo json_encode(
            array(
                'result' => 0,
                'Message' => "Device Switched On",
                'date' => date("Y-m-d h:i:sa")
            )
        );
        die();
    }
    else if ($data == "OFF")
    {
        echo json_encode(
            array(
                'result' => 0,
                'Message' => "Device Switched Off",
                'date' => date("Y-m-d h:i:sa")
            )
        );
        die();
    }
    else
    {
        echo json_encode(
            array(
                'result' => 1,
                'Message' => "Wrong Data",
                'date' => date("Y-m-d h:i:sa")
            )
        );
        die();
    }
}

?>