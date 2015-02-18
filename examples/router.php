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
hitSuji::router(
)->always(
  'simple.php'
)->on(
  'get',
  'usr/:id',
  'simple.php'
)->run('get','usr/123');
/**/
hitSuji::router(
)->always(
  function () {
    echo "default\n";
  }
)->on(
  'get',
  'usr/:id',
  function ($id) {
    var_dump($id);
    echo "usr:{$id}\n";
  }
)->run('get','usr/123');
/**/