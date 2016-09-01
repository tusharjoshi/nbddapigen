<?php


$source = file_get_contents("./test/fixtures/core.api.php");
$source = str_replace(array("\r\n", "\r"), array("\n", "\n"), $source);

chdir("./build/src/nbddapigen");
require("./parser.inc");

$api_info = nbddapigen_parser($source, "core");

var_dump($api_info);

