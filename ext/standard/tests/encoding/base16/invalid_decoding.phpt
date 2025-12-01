--TEST--
Base16 Invalid decoding cases tests
--FILE--
<?php

use Encoding\Base16;
use Encoding\DecodingMode;
use Encoding\UnableToDecodeException;

use function Encoding\base16_encode;
use function Encoding\base16_decode;

try {
    base16_decode("abc");
    var_dump(false);
} catch (UnableToDecodeException) {
    var_dump(true);
}

try {
    base16_decode("gh");
    var_dump(false);
} catch (UnableToDecodeException) {
    var_dump(true);
}

?>
--EXPECT--
bool(true)
bool(true)
