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

function scan_paths(&$scanpaths, &$extensions)
{
    $result = array();
    $visited = array();

    $cwd = getcwd();
    $paths = $scanpaths;
    while(count($paths) > 0)
    {
        $opath = $path = array_pop($paths);
        if (strpos($opath, ".") === 0)
        {
            $path = realpath($cwd . DIRECTORY_SEPARATOR . $opath);
        }
        $path = _dereference_link($path);
        if (!$path)
        {
            echo("WARNING: the specified path $opath does not exist, skipping.\n");
            continue;
        }

        // have we visited this path before?
        if (array_key_exists($path, $visited))
        {
            continue;
        }

        $candidates = [];
        if (is_dir($path))
        {
            $visited[$path] = TRUE;

            $contents = scandir($path);
            if ($contents === FALSE)
            {
                echo("WARNING: error reading directory $path, skipping.\n");
                continue;
            }

            while(count($contents) > 0)
            {
                $fpath = array_pop($contents);

                // skip all hidden files
                if (strpos($fpath, ".") === 0)
                {
                    continue;
                }
                $opath = $path . DIRECTORY_SEPARATOR . $fpath;
                $abspath = _dereference_link(realpath($opath));
                if (!$abspath)
                {
                    echo("WARNING: the specified path $opath does not exist, skipping.\n");
                    continue;
                }
                if (is_dir($abspath))
                {
                    // have we visited this path before?
                    if (!array_key_exists($abspath, $visited))
                    {
                        $paths[] = $abspath;
                    }
                }
                else
                {
                    $candidates[] = $abspath;
                }
            }
        }
        else
        {
            $candidates[] = $path;
        }

        foreach($candidates as $candidate)
        {
            if (_is_valid_candidate($candidate, $extensions))
            {
                $result[] = $candidate;
            }
        }
    }

    // TODO:sort and remove duplicates

    return $result;
}


function _dereference_link($path)
{
    $result = $path;

    while(is_link($result))
    {
        $target = readlink($result);
        if (strpos($target, ".") === 0)
        {
            $target = realpath($result . DIRECTORY_SEPARATOR . $target);
        }
        if (!$target)
        {
            $result = NULL;
            break;
        }
        $result = $target;
    }

    return $result;
}


function _is_valid_candidate($path, &$extensions)
{
    $result = FALSE;

    // skip all tests
    if (strpos($path, "_test") === FALSE)
    {
        foreach($extensions as $extension)
        {
            $pos = strrpos($path, $extension);
            if ($pos !== FALSE && $pos == strlen($path) - strlen($extension))
            {
                $result = TRUE;
                break;
            }
        }
    }

    return $result;
}

