<?php

// vim: expandtab:ts=4:sw=4
/*
 * Copyright 2016 Coldrye
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

require("fs.inc");
require("parser.inc");
require("writer.inc");

function nbddapigen_generator(&$options)
{
    $result = 0;

    $scanpaths  = $options["scanpaths"];
    $extensions = $options["extensions"];
    $outdir     = $options["outdir"];
    $exturl     = $options["exturl"];
    $drupal     = $options["drupal"];

    $files = scan_paths($scanpaths, $extensions, $result);

    foreach ($files as $file)
    {
        echo("INFO: processing $file.\n");
        $api_infos = nbddapigen_parser(
                file_get_contents($file), _strip_extension(basename($file)));
        foreach ($api_infos as $api_info)
        {
            $api_info = _prepare_api_info($api_info);
            nbddapigen_writer($outdir, $exturl, $drupal, $api_info);
        }
    }

    return $result;
}

function _strip_extension($file)
{
    return explode(".", $file)[0];
}

function _prepare_api_info(&$api_info)
{
    $result = $api_info;

    $result["fn_signature_esc"] = htmlspecialchars($result["fn_signature"]);

    if ($result["doc_short"])
    {
        $result["doc_short"] = htmlspecialchars($result["doc_short"]);
        $result["doc_short"] = _strip_newline($result["doc_short"]);
    }

    if ($result["doc_long"])
    {
        //$result["doc_long"] = htmlspecialchars($result["doc_long"]);
        $result["doc_long"] = _strip_newline($result["doc_long"]);
    }

    if ($result["doc_param"])
    {
        for ($index = 0; $index < count($result["doc_param"]); $index++)
        {
            $param         = & $result["doc_param"][$index];
            $param["type"] = htmlspecialchars($param["type"]);
            $param["name"] = htmlspecialchars($param["name"]);
            //$param["comment"] = _newline2br($param["comment"]);
        }
    }

    return $result;
}

function _newline2br($comment)
{
    return str_replace("\n", "<br/>\n", htmlspecialchars($comment));
}

function _strip_newline($comment)
{
    //return str_replace("\n", " ", htmlspecialchars($comment));
    return str_replace("\n", " ", $comment);
}
