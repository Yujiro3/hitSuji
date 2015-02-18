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

hitSuji::router([
  ''          => 'always.php',
  '/user/:id' => ['get', 'user.php']
]);

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
