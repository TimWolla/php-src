--TEST--
Base32 ASCII decoding vectors with forgiving mode
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
    $res = base32_decode(data: $encoded, decodingMode: DecodingMode::Forgiving);
    if ('' === trim($decoded)) {
        var_dump($decoded === $res);
    } else {
        echo $res, PHP_EOL;
    }}
?>
--EXPECT--
f
fo
foo
foob
fooba
foobar
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
