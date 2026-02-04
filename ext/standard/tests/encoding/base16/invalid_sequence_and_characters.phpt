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
    echo base16_decode("ZZ"), ' ; "ZZ" must not be decoded', PHP_EOL;
} catch (Throwable $exception) {
    echo $exception::class, ' : ', $exception->getMessage(), PHP_EOL;
}

try {
    echo base16_decode("ABC"), ' ; "ABC" must not be decoded', PHP_EOL;
} catch (Throwable $exception) {
    echo $exception::class, ' : ', $exception->getMessage(), PHP_EOL;
}

?>
--EXPECT--
Encoding\UnableToDecodeException : Invalid character
Encoding\UnableToDecodeException : Invalid length
