--TEST--
Base32 ASCII encoding vectors
--FILE--
<?php

use Encoding\Base32;
use function Encoding\base32_encode;

$vectors = [
    ['f', 'MY======'],
    ['fo', 'MZXQ===='],
    ['foo', 'MZXW6==='],
    ['foob', 'MZXW6YQ='],
    ['fooba', 'MZXW6YTB'],
    ['foobar', 'MZXW6YTBOI======'],
    [' ', 'EA======'],
    ['  ', 'EAQA===='],
    ['   ', 'EAQCA==='],
    ['    ', 'EAQCAIA='],
    ['     ', 'EAQCAIBA'],
    ['      ', 'EAQCAIBAEA======'],
    ['', ''],
    [
        "\x1e\x0c\x41\x97\x59\x09\xe2\xca\x2e\x87\xe1\x0b\x44\x81\xe6\xff"
      . "\x1f\x32\x4d\xcf\xdc\x82\x30\xe6\x88\x23\x72\x25\x1f\xcd\x25\xfb",
        'DYGEDF2ZBHRMULUH4EFUJAPG74PTETOP3SBDBZUIENZCKH6NEX5Q===='
    ],
    ['8', 'HA======'],
];

foreach ($vectors as [$decoded, $encoded]) {
    var_dump(base32_encode($decoded) === $encoded);
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
