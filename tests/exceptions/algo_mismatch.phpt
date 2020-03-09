--TEST--
jwt_decode throws an exception when algorithm mismatch is encountered
--FILE--
<?php
require_once __DIR__ . '/../constants.php';

try {
  $token = jwt_encode(TOKEN_CLAIMS, TOKEN_SECRET);

  print_r(jwt_decode($token, TOKEN_SECRET, JWT_ALGO_HS512));
} catch (ExtJwtException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Token verification failed: wrong algorithm