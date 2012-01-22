--TEST--
ruby_eval
--FILE--
<?php
var_dump(ruby_eval('["test"]'));
--EXPECT--
array(1) {
  [0]=>
  string(4) "test"
}

