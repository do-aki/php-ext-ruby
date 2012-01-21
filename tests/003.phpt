--TEST--
ruby_eval
--FILE--
<?php
var_dump(ruby_eval('true'));
var_dump(ruby_eval('false'));
var_dump(ruby_eval('nil'));
var_dump(ruby_eval('1'));
var_dump(ruby_eval('1.0'));
var_dump(ruby_eval('"hoge"'));
--EXPECT--
bool(true)
bool(false)
NULL
int(1)
float(1)
string(4) "hoge"

