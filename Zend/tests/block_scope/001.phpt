--TEST--
Block Scope: Basic.
--FILE--
<?php

$c = 'c';
$d = 'd';
let ($a, $b = 'B', $c, $d = 'D') {
    var_dump($a, $b, $c, $d);
}
var_dump($a, $b, $c, $d);

?>
--EXPECTF--
NULL
string(1) "B"
string(1) "c"
string(1) "D"

Warning: Undefined variable $a in %s on line %d

Warning: Undefined variable $b in %s on line %d
NULL
NULL
string(1) "c"
string(1) "d"
