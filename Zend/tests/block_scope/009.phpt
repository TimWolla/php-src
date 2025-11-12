--TEST--
Block Scope: break out.
--FILE--
<?php

function foo($a) {
    var_dump($a);
    do {
        use ($a = $a + 1) {
            var_dump($a);
            break;
        }
    } while (false);
    var_dump($a);
}

foo(1);

?>
--EXPECTF--
int(1)
int(2)
int(1)
