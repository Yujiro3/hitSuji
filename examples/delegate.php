<?php
/**
 * 
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
php -d extension=modules/hitsuji.so -f examples/delegate.php
*/

include 'skel/bootstrap.php';

$request = hitSuji::request();
$delegate = hitSuji::delegate();

echo "______binds______\n";
$delegate->binds(array(
    'PWD'=>  array('string', 'server', 'require', '/[^\/]/'),
    'MAIL'=>  array('string', 'server', 'require', '/[^\/]/')
));
print_r($request);

echo "______parse______\n";
$delegate->parse(function ($data) {
    echo "parse\n";
    return array_map('strtolower', $data);
});
$result = $delegate->fireParse(array('key'=>'VALUE'));
var_dump($result);

echo "______quick______\n";
$delegate->quick(function ($data) {
print_r($data);
    return array_map('strtolower', $data);
});
$delegate->quick(function ($data) {
print_r($data);
    return array_map('strtoupper', $data);
});
$result = $delegate->fireQuick(array('key'=>'VALUE'));
var_dump($result);

echo "______always______\n";
$delegate->always(function ($data) {
    echo "always\n";
    print_r($data);
});
$result = $delegate->fireAlways(array('key'=>'value'));

echo "______done______\n";
$delegate->done(function ($data) {
    echo "done\n";
    print_r($data);
});
$result = $delegate->fireDone(array('key'=>'value'));

echo "______fail______\n";
$delegate->fail(function ($data, $fails) {
    echo "fail\n";
    print_r($data);
    print_r($fails);
});
$result = $delegate->fireFail(
    array('key'=>'value'),
    array('id'=>0)
);

echo "______verifyNonce______\n";
$result = $delegate->verifyNonce();
var_dump($result);

echo "______reset______\n";
$delegate->reset();

echo "______run______\n";
$delegate->binds(array(
    'PWD'=>  array('string', 'server', 'require', '/[^\/]/'),
    'MAIL'=>  array('string', 'server', 'require', '/[^\/]/')
));
$delegate->parse(function ($data) {
    echo "parse\n";
    return array_map('strtolower', $data);
});
$delegate->quick(function ($data) {
    print_r($data);
    return array_map('strtoupper', $data);
});
$delegate->always(function ($data) {
    echo "always\n";
    print_r($data);
});
$delegate->done(function ($data) {
    echo "done\n";
    print_r($data);
});

$delegate->run();
/**/



echo "\n";


echo "____________\n";

var_dump($delegate);