--TEST--
Base32 invalid decoding sequences throw exceptions
--FILE--
<?php

use Encoding\Base32;
use Encoding\UnableToDecodeException;
use function Encoding\base32_decode;

$cases = [
    ['MZXQ====', Base32::Hex],
    ['90890808', Base32::Ascii],
    ['MzxQ====', Base32::Ascii],
    ['A=ACA===', Base32::Ascii],
    ['A======', Base32::Ascii],
    ['A', Base32::Ascii],
];

foreach ($cases as [$sequence, $alphabet]) {
    try {
        base32_decode($sequence, $alphabet);
    } catch (Throwable $exception) {
        echo $exception::class, ": ", $exception->getMessage(), PHP_EOL;
    }
}
?>
--EXPECT--
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid padding
Encoding\UnableToDecodeException: Invalid character
