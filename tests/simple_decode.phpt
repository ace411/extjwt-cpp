--TEST--
jwt_decode outputs JWT claims as hashtable
--FILE--
<?php
require_once 'constants.php';

$token  = jwt_encode(TOKEN_CLAIMS, TOKEN_SECRET);
$claims = jwt_decode($token, TOKEN_SECRET); 

print_r($claims);
?>
--EXPECT--
Array
(
    [github] => @ace411
    [twitter] => @agiroLoki
)