--TEST--
Base32 ASCII decoding vectors with invalid data
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
    ['foobar', 'MZXW6YTBOI'],
    [' ', 'EA'],
    ['  ', 'EAQA'],
    ['   ', 'EAQCA'],
    ['    ', 'EAQCAIA'],
    ['      ', 'EAQCAIBAEA'],
];

foreach ($vectors as [$decoded, $encoded]) {
    try {
        $result = base32_decode(data: $encoded, decodingMode: DecodingMode::Strict);
        echo $result, ' must not be decoded from `', $encoded, '`', PHP_EOL;
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
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
Encoding\UnableToDecodeException: Missing padding
