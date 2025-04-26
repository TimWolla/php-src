--TEST--
Block scope does not use try unless necessary
--INI--
opcache.enable=1
opcache.enable_cli=1
opcache.optimization_level=0
opcache.opt_debug_level=0x10000
--EXTENSIONS--
opcache
--FILE--
<?php
function foo() {
    $c = 'c';
    $d = 'd';
    use ($a, $b = 'B', $c, $d = 'D') {
        var_dump($a, $b, $c, $d);
        throw new \Exception();
    }
    var_dump($a, $b, $c, $d);
}
try {
    foo();
} catch (\Exception $e) {
}
?>
--EXPECTF--
$_main:
     ; (lines=5, args=0, vars=1, tmps=1)
     ; (before optimizer)
     ; %s
0000 INIT_FCALL 0 336 string("foo")
0001 DO_UCALL
0002 JMP 0004
0003 CV0($e) = CATCH string("Exception")
0004 RETURN int(1)
EXCEPTION TABLE:
     0000, 0003, -, -

foo:
     ; (lines=32, args=0, vars=4, tmps=12)
     ; (before optimizer)
     ; %s
0000 ASSIGN CV0($c) string("c")
0001 ASSIGN CV1($d) string("d")
0002 T6 = COALESCE CV2($a) 0005
0003 T7 = ASSIGN CV2($a) null
0004 T6 = QM_ASSIGN T7
0005 FREE T6
0006 ASSIGN CV3($b) string("B")
0007 T9 = COALESCE CV0($c) 0010
0008 T10 = ASSIGN CV0($c) null
0009 T9 = QM_ASSIGN T10
0010 FREE T9
0011 ASSIGN CV1($d) string("D")
0012 INIT_FCALL 4 144 string("var_dump")
0013 SEND_VAR CV2($a) 1
0014 SEND_VAR CV3($b) 2
0015 SEND_VAR CV0($c) 3
0016 SEND_VAR CV1($d) 4
0017 DO_ICALL
0018 V13 = NEW 0 string("Exception")
0019 DO_FCALL
0020 THROW V13
0021 UNSET_CV CV1($d)
0022 UNSET_CV CV0($c)
0023 UNSET_CV CV3($b)
0024 UNSET_CV CV2($a)
0025 INIT_FCALL 4 144 string("var_dump")
0026 SEND_VAR CV2($a) 1
0027 SEND_VAR CV3($b) 2
0028 SEND_VAR CV0($c) 3
0029 SEND_VAR CV1($d) 4
0030 DO_ICALL
0031 RETURN null
LIVE RANGES:
     13: 0019 - 0020 (new)
NULL
string(1) "B"
string(1) "c"
string(1) "D"
