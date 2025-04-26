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
    try {
        $c = 'c';
        $d = 'd';
        use ($a, $b = 'B', $c, $d = 'D') {
            var_dump($a, $b, $c, $d);
            throw new \Exception();
        }
    } catch (\Exception $e) {
        var_dump($a, $b, $c, $d);
    }
}
foo();
?>
--EXPECTF--
$_main:
     ; (lines=3, args=0, vars=0, tmps=1)
     ; (before optimizer)
     ; %s
0000 INIT_FCALL 0 368 string("foo")
0001 DO_UCALL
0002 RETURN int(1)

foo:
     ; (lines=37, args=0, vars=5, tmps=13)
     ; (before optimizer)
     ; %s
0000 ASSIGN CV0($c) string("c")
0001 ASSIGN CV1($d) string("d")
0002 T8 = COALESCE CV2($a) 0005
0003 T9 = ASSIGN CV2($a) null
0004 T8 = QM_ASSIGN T9
0005 FREE T8
0006 ASSIGN CV3($b) string("B")
0007 T11 = COALESCE CV0($c) 0010
0008 T12 = ASSIGN CV0($c) null
0009 T11 = QM_ASSIGN T12
0010 FREE T11
0011 ASSIGN CV1($d) string("D")
0012 INIT_FCALL 4 144 string("var_dump")
0013 SEND_VAR CV2($a) 1
0014 SEND_VAR CV3($b) 2
0015 SEND_VAR CV0($c) 3
0016 SEND_VAR CV1($d) 4
0017 DO_ICALL
0018 V15 = NEW 0 string("Exception")
0019 DO_FCALL
0020 THROW V15
0021 T7 = FAST_CALL 0023
0022 JMP 0028
0023 UNSET_CV CV1($d)
0024 UNSET_CV CV0($c)
0025 UNSET_CV CV3($b)
0026 UNSET_CV CV2($a)
0027 FAST_RET T7 try-catch(0)
0028 JMP 0036
0029 CV4($e) = CATCH string("Exception")
0030 INIT_FCALL 4 144 string("var_dump")
0031 SEND_VAR CV2($a) 1
0032 SEND_VAR CV3($b) 2
0033 SEND_VAR CV0($c) 3
0034 SEND_VAR CV1($d) 4
0035 DO_ICALL
0036 RETURN null
LIVE RANGES:
     15: 0019 - 0020 (new)
EXCEPTION TABLE:
     0000, 0029, -, -
     0002, -, 0023, 0027
NULL
string(1) "B"
string(1) "c"
string(1) "D"

Warning: Undefined variable $a in %s on line %d

Warning: Undefined variable $b in %s on line %d

Warning: Undefined variable $c in %s on line %d

Warning: Undefined variable $d in %s on line %d
NULL
NULL
NULL
NULL
