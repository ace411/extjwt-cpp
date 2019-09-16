--TEST--
jwt_decode decodes multi-dimensional array data
--FILE--
<?php
require_once 'constants.php';

$jwt = jwt_encode(TOKEN_SECRET, JWT_ALGO_HS256, TOKEN_CLAIMS_MULTIDIM);

echo gettype(jwt_decode($jwt, TOKEN_SECRET, JWT_ALGO_HS256));
?>
--EXPECT--
array