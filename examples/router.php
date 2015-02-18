<?php
/**
 * router
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
php -d extension=modules/hitsuji.so -f examples/router.php
*/

ini_set('hitsuji.page_path', dirname(__FILE__).'/skel/app/pages/');

/**/
hitSuji::router([
    ''        => 'simple.php',
    'usr/:id' => [
        'get',
        'simple.php'
    ]
]);

hitSuji::router([
    ''        => 'simple.php',
    'usr/:id' => [
        function ($id) {
            var_dump($id);
            echo "usr:{$id}\n";
        }, 'get'
    ]
]);

/**/