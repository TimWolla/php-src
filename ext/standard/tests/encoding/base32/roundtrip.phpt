--TEST--
Base32 encode/decode round-trip
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_encode;
use function Encoding\base32_decode;

$values = [
    '',
    '10',
    'test130',
    'test',
    '8',
    '0',
    '=',
    'foobar',
];

foreach ($values as $value) {
    var_dump(base32_decode(base32_encode($value)) === $value);
    var_dump(base32_decode(base32_encode($value, Base32::Hex), Base32::Hex) === $value);
    var_dump(base32_decode(base32_encode($value, Base32::Crockford), Base32::Crockford) === $value);
    var_dump(base32_decode(base32_encode($value, Base32::Z), Base32::Z) === $value);
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
bool(true)
bool(true)
bool(true)
bool(true)
