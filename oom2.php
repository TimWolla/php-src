<?php
try {
	$v = json_decode('[' . \str_repeat(
		'{"a":1},',
		16*1024
	) . '{"a":1}]');

	//var_dump($v);

	var_dump(memory_get_usage(), memory_get_peak_usage());
} catch (Error $e) {
	echo $e, PHP_EOL;
	var_dump(memory_get_usage(), memory_get_peak_usage());
}
