--TEST--
jwt_decode decodes token with multi-dimensional array data
--FILE--
<?php
require_once __DIR__ . '/../constants.php';

$token = jwt_encode(TOKEN_CLAIMS_MULTIDIM, TOKEN_SECRET);

print_r(jwt_decode($token, TOKEN_SECRET));
?>
--EXPECT--
Array
(
    [location] => kampala
    [social] => Array
        (
            [github] => @ace411
            [twitter] => @agiroLoki
        )

)