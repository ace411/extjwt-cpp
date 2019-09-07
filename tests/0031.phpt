--TEST--
jwt_decode throws JwtException when invalid algorithm is detected
--FILE--
<?php
require_once 'constants.php';

try {
    $token = jwt_encode(TOKEN_SECRET, JWT_ALGO_HS384, TOKEN_CLAIMS);

    var_dump(jwt_decode($token, TOKEN_SECRET, 44));
} catch (Exception $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
Invalid algorithm detected