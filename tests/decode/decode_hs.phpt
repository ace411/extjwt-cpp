--TEST--
jwt_decode decodes JWTs signed with algorithms prefixed with HS
--FILE--
<?php
require_once __DIR__ . '/../functions.php';

$encode = partial('jwt_encode', TOKEN_CLAIMS, TOKEN_SECRET);

$tokens = [
	$encode(JWT_ALGO_HS256), 
  $encode(JWT_ALGO_HS384),
  $encode(JWT_ALGO_HS512)
];

$acc    = [];
$algos  = [JWT_ALGO_HS256, JWT_ALGO_HS384, JWT_ALGO_HS512];

for ($idx = 0; $idx < count($tokens); $idx += 1) {
	$acc[] = jwt_decode($tokens[$idx], TOKEN_SECRET, $algos[$idx]);
}

print_r(array_merge(...$acc));
?>
--EXPECT--
Array
(
    [github] => @ace411
    [twitter] => @agiroLoki
)