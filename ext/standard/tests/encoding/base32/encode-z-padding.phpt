--TEST--
Base32 Z-Base-32 encoding with padding
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use Encoding\PaddingMode;
use function Encoding\base32_encode;

try {
    echo base32_encode(data: 'foobar', variant: Base32::Z, paddingMode: PaddingMode::PreservePadding), PHP_EOL;
} catch (Throwable $exception) {
    echo $exception::class, ' : ', $exception->getMessage(), PHP_EOL;
}

echo base32_encode(data: 'foobar', variant: Base32::Z, paddingMode: PaddingMode::StripPadding), PHP_EOL;
?>
--EXPECT--
ValueError : Encoding\base32_encode(): Argument #3 ($paddingMode) must not be PaddingMode::PreservePadding when argument #2 ($variant) is Base32::Z
c3zs6aubqe
