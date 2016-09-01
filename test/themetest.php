<?php


$source = file_get_contents("./test/fixtures/aggregator.theme.inc");
$source1 = file_get_contents("./test/fixtures/api.theme.inc");
$source = str_replace(array("\r\n", "\r"), array("\n", "\n"), $source);
$source1 = str_replace(array("\r\n", "\r"), array("\n", "\n"), $source1);

chdir("./build/src/nbddapigen");
require("./parser.inc");

$api_info = nbddapigen_parser($source, "aggregator");

var_dump($api_info);

$api_info = nbddapigen_parser($source1, "api");

var_dump($api_info);

