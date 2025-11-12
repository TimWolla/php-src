--TEST--
Block Scope: static variables (2).
--FILE--
<?php

function foo($a) {
    use ($a) {
        static $a;
        var_dump($a);
    }
}

foo(1);

?>
--EXPECTF--
Fatal error: Declaring static variable in use construct is disallowed in %s on line %d
