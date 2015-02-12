<?php
/**
 * View
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
php -d extension=modules/hitsuji.so -f examples/view.php
*/

include 'skel/bootstrap.php';

$view = hitSuji::view();

$view->layout('layout.tpl');
$view->content('always.tpl');
$view->assigns(array('search'=>'yahoo'));
$view->assign('id', '123');
$view->display();
