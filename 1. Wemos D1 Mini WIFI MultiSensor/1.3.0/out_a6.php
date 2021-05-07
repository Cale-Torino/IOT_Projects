<!DOCTYPE html>
<html>
<body>

<?php
$name = "data_".date("n.j.Y").".log";
$myfile = fopen($name, "r") or die("Unable to open file!");
echo fread($myfile,filesize($name))."<br>";
fclose($myfile);
?>

</body>
</html>