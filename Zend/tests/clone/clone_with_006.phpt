--TEST--
Clone with error cases
--FILE--
<?php

$x = new stdClass();

try {
	var_dump(clone($x, ...1));
} catch (Throwable $e) {
	echo $e::class, ": ", $e->getMessage(), PHP_EOL;
}

?>
--EXPECT--
TypeError: Only arrays can be unpacked for clone, int given
