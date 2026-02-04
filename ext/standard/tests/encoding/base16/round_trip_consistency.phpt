--TEST--
Base16 Round-trip consistency tests
--FILE--
<?php

use Encoding\Base16;
use Encoding\DecodingMode;
use Encoding\UnableToDecodeException;

use function Encoding\base16_encode;
use function Encoding\base16_decode;

$inputs = [
    "",
    "a",
    "abc",
    "The quick brown fox jumps over the lazy dog",
    "\0\1\2\xff",
];

foreach ($inputs as $original) {
    $encoded = base16_encode($original);
    $decoded = base16_decode($encoded);

    if ($decoded !== $original) {
        echo "Round-trip failed for: " . $original . PHP_EOL;
        continue;
    }

    var_dump(true);
}

$bytes = '';
for ($i = 0; $i < 256; $i++) {
    $bytes .= chr($i);
}

var_dump(base16_decode(base16_encode($bytes)) === $bytes);

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
