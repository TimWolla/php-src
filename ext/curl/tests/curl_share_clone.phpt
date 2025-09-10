--TEST--
Attached share handles are not preserved when cloning
--EXTENSIONS--
curl
--FILE--
<?php

$sh = curl_share_init();

$ch1 = curl_init();
curl_setopt($ch1, CURLOPT_URL, 'file://' . __DIR__ . '/curl_testdata1.txt');
curl_setopt($ch1, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch1, CURLOPT_SHARE, $sh);

$ch2 = clone $ch1;
$ch3 = curl_copy_handle($ch2);

// Make sure nothing bad handles if the share handle is unset early.
unset($sh);

var_dump($ch1, $ch2, $ch3);

?>
--EXPECTF--
object(CurlHandle)#%d (1) {
  ["share":"CurlHandle":private]=>
  object(CurlShareHandle)#%d (0) {
  }
}
object(CurlHandle)#%d (1) {
  ["share":"CurlHandle":private]=>
  NULL
}
object(CurlHandle)#%d (1) {
  ["share":"CurlHandle":private]=>
  NULL
}
