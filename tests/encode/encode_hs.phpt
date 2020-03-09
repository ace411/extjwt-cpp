--TEST--
jwt_encode signs JWTs with algorithms prefixed with HS
--FILE--
<?php
require_once __DIR__ . '/../functions.php';

$encode = partial('jwt_encode', TOKEN_CLAIMS, TOKEN_SECRET);

$tokens = [
  'hs256' => $encode(JWT_ALGO_HS256), 
  'hs384' => $encode(JWT_ALGO_HS384),
  'hs512' => $encode(JWT_ALGO_HS512)
];

print_r($tokens);
?>
--EXPECT--
Array
(
    [hs256] => eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXUyJ9.eyJnaXRodWIiOiJAYWNlNDExIiwidHdpdHRlciI6IkBhZ2lyb0xva2kifQ.sxRKwGM5W8hirRvWXpofT1_SoRQvUnAfynWs39ZKFCY
    [hs384] => eyJhbGciOiJIUzM4NCIsInR5cCI6IkpXUyJ9.eyJnaXRodWIiOiJAYWNlNDExIiwidHdpdHRlciI6IkBhZ2lyb0xva2kifQ.FQipugiod7bLPCQ757BusDNv50BQZ8LsyBHvXo3X-U43ppjQmsMwV3-qFdnrKvM9
    [hs512] => eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXUyJ9.eyJnaXRodWIiOiJAYWNlNDExIiwidHdpdHRlciI6IkBhZ2lyb0xva2kifQ.-MY1yEGluzqggyoqjAkT0_OxIKfoAun4s1mll0fXyHAlv6FuEqWkrqpC3fwEjI8ri4WBEI83IdJBtBCV2PBW5g
)