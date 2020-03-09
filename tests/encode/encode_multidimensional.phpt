--TEST--
jwt_encode encodes multi-dimensional array data
--FILE--
<?php
require_once __DIR__ . '/../constants.php';

$token = jwt_encode(TOKEN_CLAIMS_MULTIDIM, TOKEN_SECRET);

echo $token;
?>
--EXPECT--
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXUyJ9.eyJsb2NhdGlvbiI6ImthbXBhbGEiLCJzb2NpYWwiOiJ7XCJnaXRodWJcIjpcIkBhY2U0MTFcIixcInR3aXR0ZXJcIjpcIkBhZ2lyb0xva2lcIn0ifQ.sHFxpicUZ7IfgXk6Q32bgyJLDT3bL-fqAdG0qU027gI