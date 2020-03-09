--TEST--
extension throws an exception upon detection of token expiry
--FILE--
<?php
require_once __DIR__ . '/../constants.php';

try {
  $token = jwt_encode(array_merge(TOKEN_CLAIMS, [
    'iat' => time(),
    'exp' => time() + 5
  ]), TOKEN_SECRET);

  sleep(6);

  print_r(jwt_decode($token, TOKEN_SECRET));
} catch (ExtJwtException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Token verification failed: token expired