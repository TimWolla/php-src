--TEST--
Base16 decoding multiline input
--FILE--
<?php
declare(strict_types=1);

use Encoding\DecodingMode;
use function Encoding\base16_decode;

$input = <<<HEX
48 65
6C 6C
6F
HEX;

echo base16_decode($input, decodingMode: DecodingMode::Forgiving), PHP_EOL;
?>
--EXPECT--
Hello
