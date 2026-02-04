--TEST--
Base16 Uppercase / Lowercase encoding tests
--FILE--
<?php

use Encoding\Base16;
use Encoding\DecodingMode;
use Encoding\UnableToDecodeException;

use function Encoding\base16_encode;
use function Encoding\base16_decode;

$original = "Hello, World!";
$encoded  = base16_encode($original);

var_dump(1 === preg_match('/^[0-9A-F]+$/', $encoded));
echo base16_decode($encoded), PHP_EOL;
$encodedLower = base16_encode($original, variant: Base16::Lower);
var_dump(1 === preg_match('/^[0-9a-f]+$/', $encodedLower));
echo base16_decode($encodedLower, variant: Base16::Lower), PHP_EOL;

?>
--EXPECT--
bool(true)
Hello, World!
bool(true)
Hello, World!
