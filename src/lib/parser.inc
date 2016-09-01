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

require("vendor/grammar_parser/list.inc");
require("vendor/grammar_parser/object.inc");
require("vendor/grammar_parser/parser.inc");
require("vendor/grammar_parser/reader.inc");
require("vendor/grammar_parser/writer.inc");
require("vendor/grammar_parser/editor.inc");


/**
 * Parses the specified source and returns an array of api info data structures.
 *
 * API Info Data Structure
 *
 * section: theme (theme hooks) or api (api hooks) . ":" . $subsection
 * name: name of the api call (lest the prefix)
 * fn_name: name of the function
 * fn_signature: string signature of the function
 * doc_short: short summary
 * doc_long: long documentation when available (
 * doc_param: array of param info data structures when available
 *
 * Param Info Data Structure
 *
 * type: type information when available
 * name: the parameter name
 * comment: the comment when available
 *
 * @param string $source
 *   the source to be parsed, which must begin with a leading <?php PI.
 * @param string $subsection
 *   the subsection that was determined by the generator from the filename
 */
function nbddapigen_parser($source, $subsection)
{
    $result = array();
    $editor = NULL;
    $reader = NULL;
    $nodes = NULL;
    $current = NULL;

    $editor = PGPEditor::getInstance();
    $reader = $editor->getReader();

    $reader->reset();
    $reader->setSnippet($source);
    $reader->buildGrammar();

    $nodes = $reader->getStatements();
    $current = $nodes->first();
    while($current != NULL)
    {
        $statement = $current->data;
        $type = is_object($statement) ? $statement->type : $statement["type"];
        if ($type === T_FUNCTION)
        {
            $api_info = _parse_api_info($editor, $statement, $subsection);
            if ($api_info)
            {
                $result[] = $api_info;
            }
        }
        $current = $current->next;
    }

    return $result;
}


function _parse_api_info(&$editor, &$statement, $subsection)
{
    $result = NULL;

    $section = NULL;

    $RE_HOOK = "/^hook_(.+)$/";
    $RE_THEME = "/^template_preprocess_(.+)$/";
    $SECTION_API = "hooks";
    $SECTION_THEME = "themes";

    $fn_name = $editor->statementOperandToText($statement);
    $matches = array();
    if (preg_match($RE_HOOK, $fn_name, $matches))
    {
        $section = $SECTION_API;
    }
    else if (preg_match($RE_THEME, $fn_name, $matches))
    {
        $section = $SECTION_THEME;
    }

    if ($section)
    {
        $name = $matches[1];
        $fn_signature = $editor->functionGetSignature($statement);
        $comment = $editor->commentToString($statement->comment);
        if (strlen($comment) > 0)
        {
            $lines = explode("\n", $comment);
            $doc_short = _parse_comment($lines, $offset, TRUE);
            $doc_long = _parse_comment($lines, $offset);
            $doc_param = _parse_annotations($lines, $offset);
        }
        $result = _create_new_api_info(
            $section . ":" . $subsection, $name, $fn_name, $fn_signature,
            $doc_short, $doc_long, $doc_param
        );
    }

    return $result;
}


function _create_new_api_info(
    $section, $name, $fn_name, $fn_signature,
    $doc_short, $doc_long, $doc_param
)
{
    return array(
        "section" => $section,
        "name" => $name,
        "fn_name" => $fn_name,
        "fn_signature" => $fn_signature,
        "doc_short" => $doc_short,
        "doc_long" => $doc_long,
        "doc_param" => $doc_param
    );
}


function _parse_comment(&$lines, &$offset, $single = FALSE)
{
    $tmp = [];

    $count = count($lines);
    while($offset < $count)
    {
        $offset++;
        if ($offset == $count)
        {
            break;
        }
        $line = trim($lines[$offset]);

        if (strpos($line, "@") === 0)
        {
            // we stop at first param annotation
            $offset--;
            break;
        }
        else if (strlen($line) == 0)
        {
            if (count($tmp) == 0)
            {
                // skip leading empty lines
                continue;
            }
            else if ($single)
            {
                // stop before second paragraph when parsing
                // single paragraph
                break;
            }
        }
        $tmp[] = $line;
        if ($single && strpos($line, ".") === strlen($line) - 1)
        {
            // break after first paragraph when parsing
            // single paragraph
            break;
        }
    }

    return trim(implode("\n", $tmp));
}


function _parse_annotations(&$lines, &$offset)
{
    $result = [];

    $count = count($lines);
    while($offset < $count)
    {
        $offset++;
        if ($offset == $count)
        {
            break;
        }
        $line = trim($lines[$offset]);

        if (strpos($line, "@param") === 0)
        {
            $result[] = _create_new_param_info(
                $line, _parse_comment($lines, $offset)
            );
        }
        else if (strpos($line, "@") === 0)
        {
            // skip unsupported annotations
            _parse_comment($lines, $offset);
        }
    }

    return $result;
}


function _create_new_param_info($line, $comment)
{
    $parts = explode(" ", substr($line, strlen("@param ")));
    $type = "";
    $name = $parts[0];
    if (count($parts) > 1)
    {
        $type = $parts[0];
        $name = $parts[1];
    }

    return array(
        "type" => $type,
        "name" => $name,
        "comment" => $comment
    );
}

