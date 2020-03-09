--TEST--
jwt_encode throws an exception when it encounters an invalid algorithm
--FILE--
<?php
require_once __DIR__ . '/../constants.php';

try {
  $token = jwt_encode(TOKEN_CLAIMS, TOKEN_SECRET, 44);
} catch (ExtJwtException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Invalid algorithm detected