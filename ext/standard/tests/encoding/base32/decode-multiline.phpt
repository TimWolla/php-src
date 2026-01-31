--TEST--
Base32 decoding multiline input
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_decode;

$input = <<<BASE
89GMSPRL
D4======
BASE;

var_dump(base32_decode($input, Base32::Hex) === 'Bangui');
?>
--EXPECT--
bool(true)
