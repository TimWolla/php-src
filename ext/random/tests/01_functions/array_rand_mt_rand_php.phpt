--TEST--
array_rand() is not affected by MT_RAND_PHP as with PHP < 8.2
--FILE--
<?php

$array = [
    'foo',
    'bar',
    'baz',
];

mt_srand(1, MT_RAND_PHP);
function custom_array_rand() {
    global $array;
    $key = array_rand($array);
    var_dump('found key ' . $key);
    return $array[$key];
}

var_dump(
    custom_array_rand(),
    custom_array_rand(),
    custom_array_rand(),
);
?>
--EXPECTF--
Deprecated: Constant MT_RAND_PHP is deprecated since 8.3, as it uses a biased non-standard variant of Mt19937 in %s on line %d

Deprecated: The MT_RAND_PHP variant of Mt19937 is deprecated in %s on line %d
string(11) "found key 0"
string(11) "found key 1"
string(11) "found key 0"
string(3) "foo"
string(3) "bar"
string(3) "foo"
