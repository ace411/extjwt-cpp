--TEST--
jwt_encode throws ExtJwtException when a parameter is empty
--FILE--
<?php
require 'constants.php';

try {
    echo jwt_encode('', JWT_ALGO_HS256, TOKEN_CLAIMS);
} catch (ExtJwtException $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
Argument(s) cannot be empty