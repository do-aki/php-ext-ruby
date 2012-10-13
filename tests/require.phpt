--TEST--
ruby_require
--FILE--
<?php
$file = dirname(__FILE__) . '/require.rb';
ruby_require($file);

echo ruby_eval('loaded_function()');
--EXPECT--
ok
