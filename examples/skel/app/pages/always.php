<?php
/**
 * デフォルトページ
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

$template = hitSuji::view();
$template->layout('layout.tpl');

hitSuji::delegate([
    /**
     * デフォルト処理
     *
     * @access public
     * @return void
     */
    'always' => function ($values) use($template) {
        $template->content('always.tpl');
        $template->display();
    }
]);