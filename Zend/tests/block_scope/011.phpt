--TEST--
Block Scope: static variables (2).
--FILE--
<?php

function foo($a) {
    let ($a) {
        static $a;
        var_dump($a);
    }
}

foo(1);

?>
--EXPECTF--
Fatal error: Declaring static variable in let construct is disallowed in %s on line %d
