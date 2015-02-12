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

include 'skel/bootstrap.php';

$request = hitSuji::request();

$request->verifies(array(
    'PWD'=>  array('string', 'server', 'require', '/[^\/]/'),
    'MAIL'=>  array('string', 'server', 'require', '/[^\/]/')
));

$request->verify('PWD', array('string', 'server', 'require', '/[^\/]/'));


$result = $request->number('123');
var_dump($result);

$result = $request->string('search@yahoo.co.jp');
var_dump($result);


$result = $request->email('search@yahoo.co.jp');
var_dump($result);

$result = $request->url('http://yahoo.co.jp/search');
var_dump($result);

$result = $request->date('2015-02-10');
var_dump($result);

$result = $request->datetime('2015-02-10 44:44:44');
var_dump($result);

$result = $request->regex('1', '/[^\/]/');
var_dump($result);

$value = $request->value('PWD', 'request');
echo "value::";
var_dump($value);

print_r($request);
$request->reset();
print_r($request);
