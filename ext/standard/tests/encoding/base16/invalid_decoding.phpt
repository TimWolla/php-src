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
    echo base16_decode("abc"), ' ; "abc" must not be decoded', PHP_EOL;
} catch (Throwable $exception) {
    echo $exception::class, ' : ', $exception->getMessage(), PHP_EOL;
}

try {
    echo base16_decode("gh"), ' ; "gh" must not be encoded', PHP_EOL;
} catch (UnableToDecodeException) {
    echo $exception::class, ' : ', $exception->getMessage(), PHP_EOL;
}

?>
--EXPECT--
Encoding\UnableToDecodeException : Invalid character
Encoding\UnableToDecodeException : Invalid character
