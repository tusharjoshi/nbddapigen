<?php

// TODO:LICENSE


function nbddapigen_writer($outdir, &$info)
{

    $api_info = _prepare_api_info($info);

    ob_start();
    include("templates/apidoc.xml");
    $content = ob_get_contents();
    ob_end_clean();

    $spath = $outdir . DIRECTORY_SEPARATOR . str_replace(":", DIRECTORY_SEPARATOR, $info["section"]);
    if (!file_exists($spath))
    {
        mkdir($spath, 0755, TRUE);
    }
    $fpath = $spath . DIRECTORY_SEPARATOR . $info["name"] . ".xml";
    echo("INFO: writing $fpath\n");
    file_put_contents($fpath, $content);
}


function _prepare_api_info(&$api_info)
{
    $result = $api_info;

    $result["fn_signature_esc"] = htmlspecialchars($result["fn_signature"]);

    if ($result["doc_short"])
    {
        $result["doc_short"] = htmlspecialchars($result["doc_short"]);
        $result["doc_short"] = _newline2br($result["doc_short"]);
    }

    if ($result["doc_long"])
    {
        $result["doc_long"] = htmlspecialchars($result["doc_long"]);
        $result["doc_long"] = _newline2br($result["doc_long"]);
    }

    if ($result["doc_param"])
    {
        for ($index = 0; $index < count($result["doc_param"]); $index++)
        {
            $param =& $result["doc_param"][$index];
            $param["type"] = htmlspecialchars($param["type"]);
            $param["name"] = htmlspecialchars($param["name"]);
            $param["comment"] = _newline2br($param["comment"]);
        }
    }

    return $result;
}


function _newline2br($comment)
{
    return str_replace("\n", "<br/>\n", htmlspecialchars($comment));
}

