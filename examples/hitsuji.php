<?php
/**
 * hitsuji
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
php -d extension=modules/hitsuji.so -f examples/hitsuji.php
*/

namespace hitSuji;

include 'skel/bootstrap.php';

$dir = \hitSuji::dir('tpl');
var_dump($dir);

$pattern = \hitSuji::pattern('string');
var_dump($pattern);

$nonce = \hitSuji::makeNonce('/user/:id');
var_dump($nonce);
