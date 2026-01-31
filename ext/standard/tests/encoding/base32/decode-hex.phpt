--TEST--
Base32 HEX decoding vectors
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_decode;

$vectors = [
    ['f', 'CO======'],
    ['fo', 'CPNG===='],
    ['foo', 'CPNMU==='],
    ['foob', 'CPNMUOG='],
    ['fooba', 'CPNMUOJ1'],
    ['foobar', 'CPNMUOJ1E8======'],
    [' ', '40======'],
    ['  ', '40G0===='],
    ['   ', '40G20==='],
    ['    ', '40G2080='],
    ['     ', '40G20810'],
    ['      ', '40G2081040======'],
    ['', ''],
];

foreach ($vectors as [$decoded, $encoded]) {
    var_dump(base32_decode($encoded, Base32::Hex) === $decoded);
}

var_dump(base32_decode('         ', Base32::Hex) === '');
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
