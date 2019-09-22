--TEST--
jwt_decode throws ExtJwtException when token expires
--FILE--
<?php
require_once 'constants.php';

try {
    $token = jwt_encode(TOKEN_SECRET, JWT_ALGO_HS256, [
        'iat' => 1568703793,
        'exp' => 1568703798
    ]);
    sleep(10);
    var_dump(jwt_decode($token, TOKEN_SECRET, JWT_ALGO_HS256));
} catch (ExtJwtException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
token verification failed: token expired