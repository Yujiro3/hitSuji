<?php
/**
 * フロントコントローラー
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
include dirname(__FILE__).'/../bootstrap.php';

hitSuji::router(
)->always(
  'always.php'
)->on(
  'get', 
  '/user/:id', 
  'user.php'
)->run();

/**
 * メモリ使用量用
 */
function convert($size) {
    $unit=array('b','kb','mb','gb','tb','pb');
    return @round($size / pow(1024, ($i=floor(log($size, 1024)))), 2).' '.$unit[$i];
}

echo "<script>\n".
     "console.log('".convert(memory_get_usage()).
     "','".convert(memory_get_usage(true))."');\n".
     "</script>\n";
