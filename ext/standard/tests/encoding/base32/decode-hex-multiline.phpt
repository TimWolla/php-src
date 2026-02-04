--TEST--
Base32 HEX decoding multiline and whitespace handling
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_decode;

$input = <<<BASE
89GMSPRL
D4======
BASE;

echo base32_decode($input, Base32::Hex), PHP_EOL;
?>
--EXPECT--
Bangui
