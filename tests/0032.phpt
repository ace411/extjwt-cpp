--TEST--
jwt_decode throws ExtJwtException when a parameter is empty
--FILE--
<?php
try {
    var_dump(jwt_decode('', '', JWT_ALGO_HS256));
} catch (ExtJwtException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
invalid token supplied