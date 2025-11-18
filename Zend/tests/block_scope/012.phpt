--TEST--
Block Scope: global variables (1).
--FILE--
<?php

function foo() {
    global $a;
    let ($a) {
        var_dump($a);
        $a = '456';
        var_dump($a);
    }
    var_dump($a);
}

$a = '123';
foo();
var_dump($a);

?>
--EXPECT--
string(3) "123"
string(3) "456"
string(3) "123"
string(3) "123"
