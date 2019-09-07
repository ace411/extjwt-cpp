--TEST--
jwt_decode throws ExtJwtException when decode error is encountered
--FILE--
<?php
require_once 'constants.php';

try {
    var_dump(jwt_decode('abc123', TOKEN_SECRET, JWT_ALGO_HS256));
} catch (ExtJwtException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
invalid token supplied