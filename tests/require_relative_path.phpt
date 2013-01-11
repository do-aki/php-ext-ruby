--TEST--
ruby_require
--FILE--
<?php

chdir(__DIR__);
ruby_require('./require.rb');

echo ruby_eval('loaded_function()');
--EXPECT--
ok
