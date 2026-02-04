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

foreach ($values as $data) {
    foreach (Base32::cases() as $variant) {
        $encoded = base32_encode(data: $data, variant: $variant);
        $decoded = base32_decode(data: $encoded, variant: $variant);
        if ($decoded !== $data) {
            echo "Round-trip failed for: " . $data . PHP_EOL;
            continue;
        }

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
