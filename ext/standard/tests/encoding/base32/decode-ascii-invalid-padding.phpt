--TEST--
Base32 ASCII decoding vectors with invalid padding mode
--FILE--
<?php
declare(strict_types=1);

use Encoding\DecodingMode;
use Encoding\UnableToDecodeException;
use function Encoding\base32_decode;

$vectors = [
    ['f', 'MY'],
    ['fo', 'MZXQ'],
    ['foo', 'MZXW6'],
    ['foob', 'MZXW6YQ'],
    ['fooba', 'MZXW6YTB'],
    ['foobar', 'MZXW6YTBOI'],
    [' ', 'EA'],
    ['  ', 'EAQA'],
    ['   ', 'EAQCA'],
    ['    ', 'EAQCAIA'],
    ['     ', 'EAQCAIBA'],
    ['      ', 'EAQCAIBAEA'],
    ['', ''],
];

foreach ($vectors as [$decoded, $encoded]) {
    try {
        $result = base32_decode(data: $encoded, decodingMode: DecodingMode::Forgiving);
        echo $result, ' ', $decoded, PHP_EOL;
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
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
Encoding\UnableToDecodeException: Invalid character
