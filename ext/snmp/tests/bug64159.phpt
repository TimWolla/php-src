--TEST--
Bug #64159: Truncated snmpget
--CREDITS--
Boris Lytochkin
--EXTENSIONS--
snmp
--SKIPIF--
<?php
require_once(__DIR__.'/skipif.inc');
?>
--ENV--
MIBS=noneXistent
--FILE--
<?php
require_once(__DIR__.'/snmp_include.inc');

snmp_set_quick_print(false);
snmp_set_valueretrieval(SNMP_VALUE_LIBRARY);

$ext_output = snmpget($hostname, $community, '.1.3.6.1.4.1.2021.8.1.101.1');
$md5 = md5($ext_output);
if ($md5 === "ab8283f948419b2d24d22f44a80b17d3") {
    echo "okay\n";
} else {
    echo $ext_output;
}
?>
--EXPECTF--
MIB search path: %s
Cannot find module (noneXistent): At line %d in (%s)
okay
