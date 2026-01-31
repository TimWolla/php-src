--TEST--
Base32 ASCII decoding vectors with strip padding
--FILE--
<?php
declare(strict_types=1);

use Encoding\DecodingMode;
use function Encoding\base32_decode;

$vectors = [
    ['f', 'MY'],
    ['fo', 'MZXQ'],
    ['foo', 'MZXW6'],
    ['foob', 'MZXW6YQ'],
    ['fooba', 'MZXW6YTB'],
    ['foobar', 'MZXW6YTBOI'],
    [' ', 'EA'],
    ['  ', 'EAQA'],
    ['   ', 'EAQCA'],
    ['    ', 'EAQCAIA'],
    ['     ', 'EAQCAIBA'],
    ['      ', 'EAQCAIBAEA'],
    ['', ''],
];

foreach ($vectors as [$decoded, $encoded]) {
    var_dump(base32_decode(data: $encoded, decodingMode: DecodingMode::Forgiving) === $decoded);
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
