--TEST--
Base16 Encode / Decode basic strings
--FILE--
<?php

use Encoding\Base16;
use Encoding\DecodingMode;
use Encoding\UnableToDecodeException;

use function Encoding\base16_encode;
use function Encoding\base16_decode;

echo base16_encode("Hello"), PHP_EOL;
echo base16_decode("48656C6C6F"), PHP_EOL;
echo base16_encode("Hello", Base16::Lower), PHP_EOL;
echo base16_decode("48656c6c6f", Base16::Lower), PHP_EOL;

?>
--EXPECT--
48656C6C6F
Hello
48656c6c6f
Hello
