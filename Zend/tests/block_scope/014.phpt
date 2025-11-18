--TEST--
Block Scope: Empty statement.
--FILE--
<?php

let ($a = print('DONE'));

?>
--EXPECTF--
DONE
