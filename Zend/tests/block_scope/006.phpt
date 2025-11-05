--TEST--
Block Scope: References.
--FILE--
<?php

$array = [1, 2, 3];

use ($value) foreach ($array as &$value) {
  $value *= 2;
}

$value = 99;

var_dump($array);

?>
--EXPECTF--
array(3) {
  [0]=>
  int(2)
  [1]=>
  int(4)
  [2]=>
  int(6)
}
