--TEST--
Base32 Crockford encoding
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_encode;

$cases = [
    ['foo', 'CSQPY'],
    ['foobar', 'CSQPYRK1E8'],
    ['', ''],
    ['Hello', '91JPRV3F'],
];

foreach ($cases as [$decoded, $encoded]) {
    var_dump(base32_encode($decoded, Base32::Crockford) === $encoded);
}
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
