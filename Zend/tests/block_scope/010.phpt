--TEST--
Block Scope: static variables (1).
--FILE--
<?php

function foo($a) {
    static $a;
    use ($a) {
        var_dump($a);
    }
}

foo(1);

?>
--EXPECTF--
Fatal error: Cannot block scope static variable $a in %s on line %d
