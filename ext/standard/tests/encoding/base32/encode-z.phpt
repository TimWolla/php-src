--TEST--
Base32 Z-Base-32 encoding
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_encode;

$cases = [
    ['foo', 'c3zs6'],
    ['foobar', 'c3zs6aubqe'],
    ['', ''],
    ['Hello', 'jb1sa5dx'],
];

foreach ($cases as [$decoded, $encoded]) {
    var_dump(base32_encode($decoded, Base32::Z) === $encoded);
}
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
