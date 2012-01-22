Ruby on PHP (PHP extension)
=============

Ruby を PHP 上で動かすための PHP 拡張。


Introduction
-----------
現在は、文字列として ruby のコードを受け取って実行する ruby_eval 関数しかありません。

Scientific Linux release 6 (x86_64) 上で開発しています。

ライブラリパス等が決め打ちなので、標準の ruby-devel パッケージが入っている必要があります。
(autoconf よくわからないの)

###sample code

    <?php
    ruby_eval(
    <<<'EOC'
      def hello
        "HELLO Ruby on PHP!!"
      end
    EOC
    );
 
    echo ruby_eval('hello()'); // say HELLO Ruby on PHP!!

### Slide (Tokyu Ruby Kaigi 04)
http://www.slideshare.net/do_aki/20111029-rubyon-php

Installation
-----------
    phpize
    ./configure
    make test
    sudo make install

and add follow line to php.ini

    extension=ruby.so

