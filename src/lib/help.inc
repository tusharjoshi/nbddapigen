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


function nbddapigen_help()
{
    global $argv;

    nbddapigen_version();
    echo("usage: " . basename($argv[0]) . " [OPTION]\n");
    echo(" -h, --help                  ");
        echo("usage information\n");
    echo("     --version               ");
        echo("version information\n");
    echo(" -f, --force                 ");
        echo("force overwrite existing files\n");
    echo(" -o, --outdir <path>         ");
        echo("specifies the output path to which all generated api ");
        echo("documentation will be written to. (defaults to ./nbddapiout)\n");
    echo(" -x, --ext <extension>,...   ");
        echo("specifies the file name extensions to be considered. ");
        echo("(defaults to .api.php,.theme.inc,.module)\n");
//    echo(" -s, --section <section>     ");
//        echo("specifies the section into which the generated documentation ");
//        echo("should be placed. (overrides the default which is determined ");
//        echo("from the file path.)\n");
    echo(" -p <scan-path> ...          ");
        echo("specifies the paths to be scanned for available api ");
        echo("documentation\n");
}

