--TEST--
Clone with basic
--FILE--
<?php

$x = new stdClass();

$foo = 'FOO';
$bar = 'BAR';
$array = [
	'baz' => 'BAZ',
	'arra' => [1, 2, 3],
];

var_dump(clone $x);
var_dump(clone($x));
var_dump(clone($x, foo: $foo, bar: $bar));
var_dump(clone($x, ...$array));
var_dump(clone($x, ...["obj" => $x]));

?>
--EXPECTF--
object(stdClass)#%d (0) {
}
object(stdClass)#%d (0) {
}
object(stdClass)#%d (2) {
  ["foo"]=>
  string(3) "FOO"
  ["bar"]=>
  string(3) "BAR"
}
object(stdClass)#%d (2) {
  ["baz"]=>
  string(3) "BAZ"
  ["arra"]=>
  array(3) {
    [0]=>
    int(1)
    [1]=>
    int(2)
    [2]=>
    int(3)
  }
}
object(stdClass)#%d (1) {
  ["obj"]=>
  object(stdClass)#%d (0) {
  }
}
