<?php

if(!empty($_POST['action']) && !empty($_POST['input'])) {
    $action = $_POST['action'];
    $input = $_POST['input'];
    if ($action === "filesize") {
        echo filesize($input);
    } else if ($action === "mime_content_type") {
        echo mime_content_type($input);
    } else if ($action === "realpath") {
        echo realpath($input);
    } else {
        phpinfo();
    }
} else {
    echo "Welcome!";
}
