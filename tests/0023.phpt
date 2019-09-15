--TEST--
jwt_encode encodes multi-dimensional array data
--FILE--
<?php
require_once 'constants.php';

echo gettype(
    jwt_encode(
        TOKEN_SECRET, 
        JWT_ALGO_HS256,
        TOKEN_CLAIMS_MULTIDIM 
    )
);
?>
--EXPECT--
string