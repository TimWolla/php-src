--TEST--
Block Scope: goto outgoing.
--FILE--
<?php

function foo($a) {
    var_dump($a);
    use ($a = $a + 1) {
        var_dump($a);
        goto out;
    }
 out:
    var_dump($a);
}

foo(1);

?>
--EXPECTF--
int(1)
int(2)
int(1)
