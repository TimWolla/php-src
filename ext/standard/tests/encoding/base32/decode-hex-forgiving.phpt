--TEST--
Base32 HEX decoding vectors with forgiving mode
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use Encoding\DecodingMode;
use function Encoding\base32_decode;

$vectors = [
    ['f', 'CO'],
    ['fo', 'CPNG'],
    ['foo', 'CPNMU'],
    ['foob', 'CPNMUOG'],
    ['fooba', 'CPNMUOJ1'],
    ['foobar', 'CPNMUOJ1E8'],
    [' ', '40'],
    ['  ', '40G0'],
    ['   ', '40G20'],
    ['    ', '40G2080'],
    ['     ', '40G20810'],
    ['      ', '40G2081040'],
    ['', ''],
];

foreach ($vectors as [$decoded, $encoded]) {
    $res = base32_decode(data: $encoded, variant: Base32::Hex, decodingMode: DecodingMode::Forgiving);
    if ('' === trim($decoded)) {
        var_dump($decoded === $res);
    } else {
        echo $res, PHP_EOL;
    }
}
var_dump(base32_decode('         ', Base32::Hex) === '');
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
bool(true)
