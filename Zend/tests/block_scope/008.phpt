--TEST--
Block Scope: goto incoming.
--FILE--
<?php

function foo($a) {
    var_dump($a);
    goto in;
    let ($a = $a + 1) {
 in:
        var_dump($a);
    }
}

foo(1);

?>
--EXPECTF--
Fatal error: 'goto' into let construct is disallowed in %s on line %d
