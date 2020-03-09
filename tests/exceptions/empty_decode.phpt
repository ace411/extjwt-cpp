--TEST--
jwt_decode throws exception whenever empty parameters are detected
--FILE--
<?php
try {
  $claims = jwt_decode('', '');
} catch (ExtJwtException $exp) {
  echo $exp->getMessage();
}
?>
--EXPECT--
Invalid token supplied