--TEST--
Base16 Forgiving decoding tests
--FILE--
<?php

use Encoding\Base16;
use Encoding\DecodingMode;
use Encoding\UnableToDecodeException;

use function Encoding\base16_encode;
use function Encoding\base16_decode;

$encoded = "48656C6C6F2c20576F726C6421";
$original = "Hello, World!";

echo base16_decode(strtolower($encoded), decodingMode: DecodingMode::Forgiving), PHP_EOL;
echo base16_decode(strtoupper($encoded), variant: Base16::Lower, decodingMode: DecodingMode::Forgiving), PHP_EOL;

?>
--EXPECT--
Hello, World!
Hello, World!
