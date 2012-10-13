--TEST--
ruby_version
--FILE--
<?php
if (ruby_version()) {
  print "OK";
}

--EXPECT--
OK
