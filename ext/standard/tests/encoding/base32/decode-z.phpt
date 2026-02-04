--TEST--
Base32 Z-Base-32 decoding vectors
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_decode;

$vectors = [
    ['8y', '8'],
    ['ca', 'f'],
    ['c3zo', 'fo'],
    ['c3zs6', 'foo'],
    ['c3zs6ao', 'foob'],
    ['c3zs6aub', 'fooba'],
    ['c3zs6aubqe', 'foobar'],
    ['', ''],
    ['ry', ' '],
    ['ryoy', '  '],
    ['ryony', '   '],
    ['ryonyey', '    '],
    ['ryonyeby', '     '],
    ['ryonyebyry', '      '],
];

foreach ($vectors as [$encoded, $decoded]) {
    $res = base32_decode(data: $encoded, variant: Base32::Z);
    if ('' === trim($decoded)) {
        var_dump($decoded === $res);
    } else {
        echo $res, PHP_EOL;
    }
}
?>
--EXPECT--
8
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

