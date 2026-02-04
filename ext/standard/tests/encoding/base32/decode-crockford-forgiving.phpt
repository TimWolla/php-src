--TEST--
Base32 Crockford decoding with invalid data
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use Encoding\DecodingMode;
use function Encoding\base32_decode;

$cases = [
    ['foo', 'CSQPY===='],
    ['foobar', 'CSQPYRK1E8==='],
    ['', '==='],
    ['Hello', '91JPRV3F===='],
];

foreach ($cases as [$decoded, $encoded]) {
    try {
        echo base32_decode($encoded, variant: Base32::Crockford, decodingMode: DecodingMode::Forgiving), '; `'.$encoded.'` must throw instead of being decoded.', PHP_EOL;
    } catch (Throwable $exception) {
        echo $exception::class, ": ", $exception->getMessage(), PHP_EOL;
    }
}
?>
--EXPECT--
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
