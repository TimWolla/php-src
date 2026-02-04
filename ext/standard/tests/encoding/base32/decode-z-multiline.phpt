--TEST--
Base32 Z-Base-32 decoding multiline and whitespace handling
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_decode;

$input = <<<BASE
ejo sh35 ip
r
BASE;

echo base32_decode($input, Base32::Z), PHP_EOL;
?>
--EXPECT--
Bangui
