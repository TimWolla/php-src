--TEST--
Base16 Invalid characters & odd length tests
--FILE--
<?php

use Encoding\Base16;
use Encoding\DecodingMode;
use Encoding\UnableToDecodeException;

use function Encoding\base16_encode;
use function Encoding\base16_decode;

try {
    base16_decode("ZZ");
    var_dump(false);
} catch (UnableToDecodeException) {
    var_dump(true);
}

try {
    base16_decode("ABC");
    var_dump(false);
} catch (UnableToDecodeException) {
    var_dump(true);
}

?>
--EXPECT--
bool(true)
bool(true)
