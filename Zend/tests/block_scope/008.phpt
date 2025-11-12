--TEST--
Block Scope: goto incoming.
--FILE--
<?php

function foo($a) {
    var_dump($a);
    goto in;
    use ($a = $a + 1) {
 in:
        var_dump($a);
    }
}

foo(1);

?>
--EXPECTF--
Fatal error: 'goto' into use construct is disallowed in %s on line %d
