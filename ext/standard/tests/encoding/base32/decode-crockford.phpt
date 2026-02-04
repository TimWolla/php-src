--TEST--
Base32 Crockford decoding vectors
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_decode;

$vectors = [
    ['CR', 'f'],
    ['CSQG', 'fo'],
    ['CSQPY', 'foo'],
    ['CSQPYRG', 'foob'],
    ['CSQPYRK1', 'fooba'],
    ['CSQPYRK1E8', 'foobar'],
    ['', ''],
    ['40', ' '],
    ['40G0', '  '],
    ['40G20', '   '],
    ['40G2080', '    '],
    ['40G20810', '     '],
    ['40G2081040', '      '],
];

foreach ($vectors as [$encoded, $decoded]) {
    $res = base32_decode(data: $encoded, variant: Base32::Crockford);
    if ('' === trim($decoded)) {
        var_dump($decoded === $res);
    } else {
        echo $res, PHP_EOL;
    }
}
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
