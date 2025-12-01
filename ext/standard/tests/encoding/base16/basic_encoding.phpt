--TEST--
Base16 Encode / Decode basic strings
--FILE--
<?php

use Encoding\Base16;
use Encoding\DecodingMode;
use Encoding\UnableToDecodeException;

use function Encoding\base16_encode;
use function Encoding\base16_decode;

var_dump(base16_encode("Hello") === "48656C6C6F");
var_dump(base16_decode("48656C6C6F") === "Hello");

var_dump(base16_encode("Hello", Base16::Lower) === "48656c6c6f");
var_dump(base16_decode("48656c6c6f", Base16::Lower) === "Hello");

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
