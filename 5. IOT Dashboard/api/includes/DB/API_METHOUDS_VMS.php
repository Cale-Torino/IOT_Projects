<?php

class API_METHOUDS extends API_DB {

    public function getCommandResult($data){
        $stmt = $this->connect()->prepare("INSERT INTO devstatlog (id, devicestat, created_at) VALUES (null, '".$data."',CURRENT_TIMESTAMP)");
        if ($stmt->execute())
        {
            //$log  = "Data:".$data.PHP_EOL.'Date:'.date("F j, Y, g:i a").PHP_EOL."------------------------->".PHP_EOL;
            //file_put_contents('./getCommandResult_'.date("n.j.Y").'.log', $log, FILE_APPEND);
            //file_put_contents('./getCommandResult.log', $log, FILE_APPEND);
            $array = '{"result": 0,"getCommandResult": inserted}';
            return $array;
        }else{
            echo json_encode(
                array(
                    'result'  => 3,
                    'message' =>'getCommandResult is invalid!'
                )
            );
            die(); 
        }
    }

    //select *from devstatlog ORDER BY id DESC LIMIT 1;
    //SELECT * FROM `devstatlog` WHERE id=(SELECT MAX(id) FROM `devstatlog`);
    public function getLastCommand(){
        $stmt = $this->connect()->query('SELECT * FROM `devstatlog` WHERE id=(SELECT MAX(id) FROM `devstatlog`);');
        if ($stmt->rowCount()) {
        //ID Account Name Password UpdateTime
        $array='';
        while($row =$stmt->fetch()){
            $array .= '{"id": "'.$row['id'].'","devicestat": "'.$row['devicestat'].'","created_at": "'.$row['created_at'].'"},';
        }
        $array = substr($array, 0, -1);
        //return '['.$array.']';
        return $array;
    }else{
        echo json_encode(
            array(
                'result'  => 6,
                'message' =>'getLastCommand not found'
            )
        );
        die(); 
    }
    }
}


?>