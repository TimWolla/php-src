--TEST--
Base32 HEX encoding vectors
--FILE--
<?php

use Encoding\Base32;
use function Encoding\base32_encode;

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
    [
        "\x1e\x0c\x41\x97\x59\x09\xe2\xca\x2e\x87\xe1\x0b\x44\x81\xe6\xff"
      . "\x1f\x32\x4d\xcf\xdc\x82\x30\xe6\x88\x23\x72\x25\x1f\xcd\x25\xfb",
        '3O6435QP17HCKBK7S45K90F6VSFJ4JEFRI131PK84DP2A7UD4NTG===='
    ],
];

foreach ($vectors as [$decoded, $encoded]) {
    var_dump(base32_encode($decoded, Base32::Hex) === $encoded);
}
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
bool(true)
