--TEST--
Base16 Ignored whitespace tests
--FILE--
<?php

use Encoding\Base16;
use Encoding\DecodingMode;

use function Encoding\base16_encode;
use function Encoding\base16_decode;

$encoded = "48 65\n6C\t6C\r6F 2C 20 57 6F 72 6C 64 21";

echo base16_decode($encoded, decodingMode: DecodingMode::Forgiving), PHP_EOL;
echo base16_decode($encoded, decodingMode: DecodingMode::Strict), PHP_EOL;

?>
--EXPECT--
Hello, World!
Hello, World!
