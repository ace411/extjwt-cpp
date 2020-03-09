--TEST--
Check if extension extjwt is loaded
--SKIPIF--
<?php
if (!extension_loaded('extjwt_cpp')) {
    echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension extjwt is available';
?>
--EXPECT--
The extension extjwt is available