--TEST--
jwt_encode throws exception whenever empty parameters are detected
--FILE--
<?php
try {
  $token = jwt_encode([], '');
} catch (ExtJwtException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Argument(s) cannot be empty