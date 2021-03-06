--TEST--
jwt_encode signs JWTs with algorithms prefixed with PS
--FILE--
<?php
require_once __DIR__ . '/../functions.php';

$encode = partial('jwt_encode', TOKEN_CLAIMS, RSA_PRIVATE_KEY);

$tokens   = [
  'ps256' => $encode(JWT_ALGO_PS256), 
  'ps384' => $encode(JWT_ALGO_PS384),
  'ps512' => $encode(JWT_ALGO_PS512)
];

$isValid  = array_reduce($tokens, function ($acc, $token) {
  $acc = preg_match('/([\w\d]*)(.){1}([\w\d\W]*)/', $token) ? true : false;

  return $acc;
}, false);

echo $isValid ? 'true' : 'false';
?>
--EXPECT--
true