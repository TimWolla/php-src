--TEST--
Block Scope: References.
--FILE--
<?php

$array = [1, 2, 3];

let ($value) foreach ($array as &$value) {
    $value *= 2;
}

$value = 99;

var_dump($array);

$a = '1';
$b = &$a;
let ($b) {
    $b = '2';
}
var_dump($a, $b);
$b = '3';
var_dump($a, $b);

?>
--EXPECT--
array(3) {
  [0]=>
  int(2)
  [1]=>
  int(4)
  [2]=>
  int(6)
}
string(1) "1"
string(1) "1"
string(1) "3"
string(1) "3"
