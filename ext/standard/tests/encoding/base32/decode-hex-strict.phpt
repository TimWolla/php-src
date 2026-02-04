--TEST--
Base32 HEX decoding vectors with strict mode
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
    ['      ', '40G2081040'],
    ['', ''],
];

foreach ($vectors as [$decoded, $encoded]) {
    try {
        echo base32_decode(data: $encoded, variant:Base32::Hex), PHP_EOL;
    } catch (Throwable $exception) {
        echo $exception::class, ": ", $exception->getMessage(), PHP_EOL;
    }
}
?>
--EXPECT--
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
fooba
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
