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

require("help.inc");
require("version.inc");
require("generator.inc");

function nbddapigen_cli()
{
    $result = 0;

    $options = _parse_options();

    if ($options == NULL)
    {
        nbddapigen_help();
        $result = 1;
    }
    else if (array_key_exists("help", $options))
    {
        nbddapigen_help();
    }
    else if (array_key_exists("version", $options))
    {
        nbddapigen_version();
    }
    else
    {
        $message = NULL;
        if (!_validate_options($options, $message, $result))
        {
            echo($message);
            nbddapigen_help();
        }
        else
        {
            try
            {
                nbddapigen_generator($options);
            }
            catch (Exception $ex)
            {
                nbddapigen_help();
                throw $ex;
            }
        }
    }

    exit($result);
}

function _parse_options()
{
    global $argv;

    $result = array(
        "exturl"     => "http://api.drupal.org/api/search",
        "extensions" => array(".api.php", ".theme.inc", ".module"),
        "outdir"     => "./nbddapigenout"
    );

    $opts = $argv;
    array_shift($opts);
    while (count($opts) > 0)
    {
        $opt   = array_shift($opts);
        $value = NULL;
        // are we done parsing options yet?
        if (strpos($opt, "-") !== 0)
        {
            array_unshift($opts, $opt);
            break;
        }
        if (strpos($opt, "=") !== FALSE)
        {
            $parts = explode("=", $opt);
            $opt   = $parts[0];
            $value = $parts[1];
        }
        switch ($opt)
        {
            case "-h":
            case "--help":
                $result["help"]      = TRUE;
                break;
            case "-f":
            case "--force":
                $result["force"]     = TRUE;
                break;
            case "--version":
                $result["version"]   = TRUE;
                break;
            case "-o":
                $value               = array_shift($opts);
            // fall through
            case "--outdir":
                $result["outdir"]    = $value;
                break;
            case "-d":
                $value               = array_shift($opts);
            // fall through
            case "--drupal":
                $result["drupal"]    = $value;
                break;
            case "-u":
                $value               = array_shift($opts);
            // fall through
            case "--exturl":
                $result["exturl"]    = $value;
                break;
            case "-x":
                $value               = array_shift($opts);
            // fall through
            case "--extension":
                $result["extension"] = explode(",", $value);
                break;
            default:
                echo("Error: unknown option " . $opt . "\n\n");
                return NULL;
        }
    }

    if (count($opts) > 0)
    {
        $result["scanpaths"] = array();
        while (count($opts) > 0)
        {
            $result["scanpaths"][] = array_shift($opts);
        }
    }

    return $result;
}

function _validate_options(&$options, &$message, &$code)
{
    $result = TRUE;
    if (!array_key_exists("scanpaths", $options) || count($options["scanpaths"])
            == 0)
    {
        $message = "ERROR: at least one scan path is required.\n\n";
        $code    = 1;
        $result  = FALSE;
    }
    else if (!array_key_exists("drupal", $options) || array_key_exists("drupal",
                                                                       $options)
            && strrpos($options["drupal"], ".x") === FALSE)
    {
        $message = "ERROR: drupal version required, e.g. 8.2.x.\n\n";
        $code    = 2;
        $result  = FALSE;
    }
    return $result;
}
