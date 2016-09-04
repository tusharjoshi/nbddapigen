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

function nbddapigen_writer($outdir, &$api_info) {
    ob_start();
    include("templates/apidoc.xml");
    $content = ob_get_contents();
    ob_end_clean();

    $spath = $outdir . DIRECTORY_SEPARATOR . str_replace(":", DIRECTORY_SEPARATOR, $api_info["section"]);
    if (!file_exists($spath)) {
        mkdir($spath, 0755, TRUE);
    }
    $fpath = $spath . DIRECTORY_SEPARATOR . $api_info["name"] . ".xml";
    echo("INFO: writing $fpath\n");
    file_put_contents($fpath, $content);
}
