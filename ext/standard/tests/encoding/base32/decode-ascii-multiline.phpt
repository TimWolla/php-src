--TEST--
Base32 ASCII decoding multiline and whitespace handling
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_decode;

$input = <<<BASE
IJQ W4Z
3VNE=
=====
BASE;

echo base32_decode($input, Base32::Ascii), PHP_EOL;
?>
--EXPECT--
Bangui
