--TEST--
Base32 ASCII decoding vectors with invalid padding mode
--FILE--
<?php
declare(strict_types=1);

use Encoding\DecodingMode;
use Encoding\UnableToDecodeException;
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

foreach ($vectors as [, $encoded]) {
    try {
        base32_decode(data: $encoded, decodingMode: DecodingMode::Strict);
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
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
