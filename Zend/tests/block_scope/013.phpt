--TEST--
Block Scope: global variables (2).
--FILE--
<?php

function foo($a) {
    let ($a) {
        global $a;
        var_dump($a);
    }
}

foo(1);

?>
--EXPECTF--
Fatal error: Declaring global variable in let construct is disallowed in %s on line %d
