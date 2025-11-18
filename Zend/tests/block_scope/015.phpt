--TEST--
Block Scope: Ast printing.
--FILE--
<?php

assert(false && function () {
	let ($a, $b = 1) if ($b) var_dump($b);
});

?>
--EXPECTF--
Fatal error: Uncaught AssertionError: assert(false && function () {
    let ($a, $b = 1) {
        if ($b) {
            var_dump($b);
        }
    }
}) in %s:%d
Stack trace:
#0 %s(%d): assert(false, 'assert(false &&...')
#1 {main}
  thrown in %s on line %d
