--TEST--
jwt_encode function outputs JSON Web Token
--FILE--
<?php
require_once 'constants.php';

echo jwt_encode(TOKEN_CLAIMS, TOKEN_SECRET);
?>
--EXPECT--
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXUyJ9.eyJnaXRodWIiOiJAYWNlNDExIiwidHdpdHRlciI6IkBhZ2lyb0xva2kifQ.sxRKwGM5W8hirRvWXpofT1_SoRQvUnAfynWs39ZKFCY