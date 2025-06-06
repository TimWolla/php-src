--TEST--
enchant_dict_suggest() function
--CREDITS--
marcosptf - <marcosptf@yahoo.com.br>
--EXTENSIONS--
enchant
--SKIPIF--
<?php
if (!enchant_broker_list_dicts(enchant_broker_init())) {die("skip no dictionary installed on this machine! \n");}
?>
--FILE--
<?php
$broker = enchant_broker_init();
$dicts = enchant_broker_list_dicts($broker);
$sugs = "soong";

if (is_object($broker)) {
    echo("OK\n");
    $requestDict = enchant_broker_request_dict($broker, $dicts[0]['lang_tag']);

    if ($requestDict) {
        echo("OK\n");
        $dictSuggest = enchant_dict_suggest($requestDict,$sugs);

        if (is_array($dictSuggest)) {
            echo("OK\n");
        } else {
            echo("dict suggest failed\n");
        }

    } else {
    echo("broker request dict failed\n");

    }

} else {
    echo("broker is not a resource; failed;\n");
}
?>
--EXPECT--
OK
OK
OK
