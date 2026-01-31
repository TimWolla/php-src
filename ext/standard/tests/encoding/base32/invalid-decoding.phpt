--TEST--
Base32 invalid decoding sequences throw exceptions
--FILE--
<?php
declare(strict_types=1);

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
        var_dump(false);
    } catch (UnableToDecodeException) {
        var_dump(true);
    }
}
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
