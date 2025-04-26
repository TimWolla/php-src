--TEST--
Block Scope: Using used variables.
--FILE--
<?php

$a = 'a';

use ($a, $b = strtoupper($a), $c = $b . $a) {
    var_dump($a, $b, $c);
}

?>
--EXPECT--
string(1) "a"
string(1) "A"
string(2) "Aa"
