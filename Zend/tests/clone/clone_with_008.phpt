--TEST--
Clone with error cases
--FILE--
<?php

readonly class Clazz {
	public function __construct(
		public public(set) string $a,
		public public(set) string $b,
	) { }
}

$c = new Clazz('default', 'default');

var_dump(clone($c, a: "updated A"));

?>
--EXPECTF--
Fatal error: Uncaught Error: Cannot modify readonly property Clazz::$a in %s:%d
Stack trace:
#0 %s(%d): clone(Object(Clazz), a: 'updated A')
#1 {main}
  thrown in %s on line %d
