<?php
/**
 * 
 *
 * PHP versions 5.3.x
 *
 *      hitSuji : ひつじ <https://github.com/Yujiro3/hitsuji>
 *      Copyright (c) 2011-2015 sheeps.me All Rights Reserved.
 *
 * @package         hitSuji
 * @copyright       Copyright (c) 2011-2015 sheeps.me
 * @author          Yujiro Takahashi <yujiro3@gmail.com>
 * @filesource
 */

/*
php -d extension=modules/hitsuji.so -f examples/delegate.php
*/

hitSuji::delegate([
    'data' => [
        'sid'=> 'sid90'
    ],
    'bind' => [
        'PWD' => ['string', 'require', 'server'],
        'MAIL'=> ['server'],
    ],
    'parse'=> function ($data) {
        echo "parse\n";
        print_r($data);
        return $data;
    },
    'action'=> function ($data) {
        echo "action\n";
        print_r($data);
        return [true, $data];
    },
    'always' => function ($data) {
        echo "always\n";
        print_r($data);
    },
    'done' => function ($data) {
        echo "done\n";
        print_r($data);
    },
    'fail' => function ($data) {
        echo "fail\n";
        print_r($data);
    }
]);

