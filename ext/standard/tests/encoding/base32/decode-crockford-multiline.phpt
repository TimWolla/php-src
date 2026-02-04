--TEST--
Base32 Crockford decoding multiline and whitespace handling
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_decode;

$input = <<<BASE
8 9G PWSV  ND
4
BASE;

echo base32_decode($input, Base32::Crockford), PHP_EOL;
?>
--EXPECT--
Bangui
