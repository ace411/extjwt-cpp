--TEST--
jwt_encode function outputs JSON Web Token
--FILE--
<?php
require_once 'constants.php';

echo gettype(jwt_encode(TOKEN_SECRET, JWT_ALGO_HS256, TOKEN_CLAIMS));
?>
--EXPECT--
string