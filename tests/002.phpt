--TEST--
ruby_eval
--FILE--
<?php
ruby_eval(
<<<'EOC'
  def hello_ruby_on_php
    "HELLO Ruby on PHP!!!"
  end
EOC
);

echo ruby_eval('hello_ruby_on_php()');
--EXPECT--
HELLO Ruby on PHP!!!
