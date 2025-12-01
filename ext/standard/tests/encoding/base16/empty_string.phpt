--TEST--
Base16 Empty string handling tests
--FILE--
<?php

use Encoding\Base16;
use Encoding\DecodingMode;
use Encoding\UnableToDecodeException;

use function Encoding\base16_encode;
use function Encoding\base16_decode;

var_dump(base16_encode("") === "");
var_dump(base16_decode("") === "");

?>
--EXPECT--
bool(true)
bool(true)
