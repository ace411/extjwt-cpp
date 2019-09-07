--TEST--
jwt_decode outputs JWT claims as hashtable
--FILE--
<?php
require_once 'constants.php';

$token  = jwt_encode(TOKEN_SECRET, JWT_ALGO_HS256, TOKEN_CLAIMS);
$claims = jwt_decode($token, TOKEN_SECRET, JWT_ALGO_HS256); 

echo gettype($claims);
?>
--EXPECT--
array