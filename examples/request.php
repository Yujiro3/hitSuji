<?php
/**
 * request
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
php -d extension=modules/hitsuji.so -f examples/request.php
*/

$request = hitSuji::request([
    'PWD'=>  array('string', 'server', 'require', '/[^\/]/'),
    'MAIL'=>  array('string', 'server', 'require', '/[^\/]/')
]);

var_dump($request);

