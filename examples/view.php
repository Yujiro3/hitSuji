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

ini_set('hitsuji.template_path', dirname(__FILE__).'/skel/app/templates/');

$template = hitSuji::template();

$template->layout('layout.tpl');
$template->content('always.tpl');
$template->assigns(array('search'=>'yahoo'));
$template->assign('id', '123');
$template->display();
