--TEST--
stream_context_get_params()
--FILE--
<?php

$ctx = stream_context_create();
var_dump(stream_context_get_params($ctx));

var_dump(stream_context_set_option($ctx, "foo","bar","baz"));
var_dump(stream_context_get_params($ctx));

function stream_notification_callback() {}
var_dump(stream_context_set_params($ctx, array("notification" => "stream_notification_callback")));
var_dump(stream_context_get_params($ctx));

class MyStream {
    public static function notification_callback() {}
}
var_dump(stream_context_set_params($ctx, array("notification" => ["MyStream", "notification_callback"])));
var_dump(stream_context_get_params($ctx));

var_dump(stream_context_get_params($ctx));
var_dump(stream_context_get_options($ctx));
var_dump(stream_context_get_params($ctx));
var_dump(stream_context_get_options($ctx));

?>
--EXPECT--
array(1) {
  ["options"]=>
  array(0) {
  }
}
bool(true)
array(1) {
  ["options"]=>
  array(1) {
    ["foo"]=>
    array(1) {
      ["bar"]=>
      string(3) "baz"
    }
  }
}
bool(true)
array(2) {
  ["notification"]=>
  string(28) "stream_notification_callback"
  ["options"]=>
  array(1) {
    ["foo"]=>
    array(1) {
      ["bar"]=>
      string(3) "baz"
    }
  }
}
bool(true)
array(2) {
  ["notification"]=>
  array(2) {
    [0]=>
    string(8) "MyStream"
    [1]=>
    string(21) "notification_callback"
  }
  ["options"]=>
  array(1) {
    ["foo"]=>
    array(1) {
      ["bar"]=>
      string(3) "baz"
    }
  }
}
array(2) {
  ["notification"]=>
  array(2) {
    [0]=>
    string(8) "MyStream"
    [1]=>
    string(21) "notification_callback"
  }
  ["options"]=>
  array(1) {
    ["foo"]=>
    array(1) {
      ["bar"]=>
      string(3) "baz"
    }
  }
}
array(1) {
  ["foo"]=>
  array(1) {
    ["bar"]=>
    string(3) "baz"
  }
}
array(2) {
  ["notification"]=>
  array(2) {
    [0]=>
    string(8) "MyStream"
    [1]=>
    string(21) "notification_callback"
  }
  ["options"]=>
  array(1) {
    ["foo"]=>
    array(1) {
      ["bar"]=>
      string(3) "baz"
    }
  }
}
array(1) {
  ["foo"]=>
  array(1) {
    ["bar"]=>
    string(3) "baz"
  }
}
