--TEST--
jwt_encode throws JwtException when invalid algorithm is detected
--FILE--
<?php
require_once 'constants.php';

try {
    echo jwt_encode(TOKEN_SECRET, 22, TOKEN_CLAIMS);
} catch (Exception $exp) {
    echo $exp->getMessage();
}
?>
--EXPECT--
Invalid algorithm detected