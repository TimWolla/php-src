--TEST--
Block Scope: break out.
--FILE--
<?php

function let_in_do($a) {
    var_dump($a);
    do {
        let ($a = $a + 1) {
            var_dump($a);
            break;
        }
    } while (false);
    var_dump($a);
}

let_in_do(1);

function do_in_let($a) {
    var_dump($a);
    let ($a = $a + 1) do {
        var_dump($a);
        break;
    } while (false);
    var_dump($a);
}

do_in_let(3);

?>
--EXPECTF--
int(1)
int(2)
int(1)
int(3)
int(4)
int(3)
