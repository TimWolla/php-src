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

var_dump(
    base16_decode(
        strtolower($encoded),
        decodingMode: DecodingMode::Forgiving
    ) === $original
);

var_dump(
    base16_decode(
        strtoupper($encoded),
        variant: Base16::Lower,
        decodingMode: DecodingMode::Forgiving
    ) === $original
);

?>
--EXPECT--
bool(true)
bool(true)
