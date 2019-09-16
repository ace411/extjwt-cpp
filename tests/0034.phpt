--TEST--
jwt_decode decodes array with non-string values intact
--FILE--
<?php
require_once 'constants.php';

$jwt = jwt_encode(TOKEN_SECRET, JWT_ALGO_HS256, [
    'hasInsta'  => false,
    'cpp_ver'   => 11,
    'pi'        => 3.141
]);

var_dump(jwt_decode($jwt, TOKEN_SECRET, JWT_ALGO_HS256));
?>
--EXPECT--
array(3) {
  ["cpp_ver"]=>
  int(11)
  ["hasInsta"]=>
  bool(false)
  ["pi"]=>
  float(3.141)
}