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
php -d extension=modules/hitsuji.so -f examples/quick.php
*/

hitSuji::quick([
    'data' => ['id'=> 3],
    'parse'=> function ($data) {
        echo "parse\n";
        return $data;
    },
    'action'=> function ($data) {
        echo "action\n";
        return [true, $data];
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
