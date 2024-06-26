--TEST--
iterable type#001
--FILE--
<?php

function test(iterable $iterable) {
    var_dump($iterable);
}

function gen() {
    yield 1;
    yield 2;
    yield 3;
};

test([1, 2, 3]);
test(gen());
test(new ArrayIterator([1, 2, 3]));

try {
    test(1);
} catch (Throwable $e) {
    echo $e->getMessage();
}
?>
--EXPECTF--
array(3) {
  [0]=>
  int(1)
  [1]=>
  int(2)
  [2]=>
  int(3)
}
object(Generator)#%d (1) {
  ["function"]=>
  string(3) "gen"
}
object(ArrayIterator)#1 (1) {
  ["storage":"ArrayIterator":private]=>
  array(3) {
    [0]=>
    int(1)
    [1]=>
    int(2)
    [2]=>
    int(3)
  }
}
test(): Argument #1 ($iterable) must be of type Traversable|array, int given, called in %s on line %d
