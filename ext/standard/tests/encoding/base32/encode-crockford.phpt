--TEST--
Base32 Crockford encoding
--FILE--
<?php
declare(strict_types=1);

use Encoding\Base32;
use function Encoding\base32_encode;

$vectors = [
    '8',
    'f',
    'fo',
    'foo',
    'foob',
    'fooba',
    'foobar',
    '',
    ' ',
    '  ',
    '   ',
    '    ',
    '     ',
    '      ',
    "\x1e\x0c\x41\x97\x59\x09\xe2\xca\x2e\x87\xe1\x0b\x44\x81\xe6\xff"
  . "\x1f\x32\x4d\xcf\xdc\x82\x30\xe6\x88\x23\x72\x25\x1f\xcd\x25\xfb",
];

foreach ($vectors as $data) {
    echo base32_encode(data: $data, variant: Base32::Crockford), PHP_EOL;
}
?>
--EXPECT--
70
CR
CSQG
CSQPY
CSQPYRG
CSQPYRK1
CSQPYRK1E8

40
40G0
40G20
40G2080
40G20810
40G2081040
3R6435TS17HCMBM7W45M90F6ZWFK4KEFVJ131SM84DS2A7YD4QXG
