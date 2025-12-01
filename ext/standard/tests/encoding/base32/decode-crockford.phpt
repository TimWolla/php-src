--TEST--
Base32 Crockford decoding
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_decode;

$cases = [
    ['foo', 'CSQPY'],
    ['foobar', 'CSQPYRK1'],
    ['', ''],
    ['Hello', '91JPRV3F'],
];

foreach ($cases as [$decoded, $encoded]) {
    var_dump(base32_decode($encoded, Base32::Crockford) === $decoded);
}
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
