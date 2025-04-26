--TEST--
Block Scope: Order of destruction.
--FILE--
<?php

class D {
    public function __construct(
        public string $name,
    ) {
        echo "Constructing: ", $this->name, PHP_EOL;
    }

    public function __destruct() {
        echo "Destructing: ", $this->name, PHP_EOL;
    }
}

use ($a = new D('a'), $b = new D('b'), $c = new D('c')) {
    var_dump($a, $b, $c);
}

?>
--EXPECTF--
Constructing: a
Constructing: b
Constructing: c
object(D)#%d (1) {
  ["name"]=>
  string(1) "a"
}
object(D)#%d (1) {
  ["name"]=>
  string(1) "b"
}
object(D)#%d (1) {
  ["name"]=>
  string(1) "c"
}
Destructing: c
Destructing: b
Destructing: a
