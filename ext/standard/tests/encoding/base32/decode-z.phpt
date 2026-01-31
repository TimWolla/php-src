--TEST--
Base32 Z-Base-32 decoding
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_decode;

$cases = [
    ['foo', 'c3zs6'],
    ['foobar', 'c3zs6aub'],
    ['', ''],
    ['Hello', 'jb1sa5dx'],
];

foreach ($cases as [$decoded, $encoded]) {
    var_dump(base32_decode($encoded, Base32::Z) === $decoded);
}
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
