--TEST--
ruby_eval
--FILE--
<?php
var_dump(ruby_eval('[1]'));
--EXPECT--
array(1) {
  [0]=>
  int(1)
}

