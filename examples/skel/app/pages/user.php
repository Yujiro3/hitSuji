<?php
/**
 * ユーザーページ
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
    'bind' => [
        'id' => ['number', 'route'],
    ],
    /**
     * パース処理
     *
     * @access public
     * @param array $data
     * @return array
     */
    'parse' => function ($data) {
        return array_map('intval', $data);
    },
    /**
     * アクション処理
     *
     * @access public
     * @param array $data
     * @return array
     */
    'action' => function ($data) {
        /* DB関連処理など */
        // return [$data];

        /* 出力の分岐を明確にする場合 */
        return [true, $data];
    },
    /**
     * デフォルト処理
     *
     * @access public
     * @return void
     */
    'always' => function ($values) use($template) {
        $template->content('user/always.tpl');
        $template->assign('id', $values['id']);
        $template->assign('nonce', hitSuji::makeNonce('/user/:id'));
        $template->display();
    },
    /**
     * 成功時処理
     *
     * @access public
     * @return void
     */
    function ($values) use($template) {
        $template->content('user/success.tpl');
        $template->assign('id', $values['id']);
        $template->display();
    },
    /**
     * 失敗時処理
     *
     * @access public
     * @return void
     */
    'done' => function ($values, $checks) use($template) {
        $template->content('user/fail.tpl');
        $template->assign('id', $values['id']);
        $template->assign('error', 'エラーです。');
        $template->display();
    }
]);