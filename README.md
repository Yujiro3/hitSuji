PHPフレームワーク
======================
Web framework delivered as a C-extension for PHP 

利用方法
------

### hitSujiのインストール ###
    
    $ git clone https://github.com/Yujiro3/hitSuji.git
    $ cd ./hitSuji
    $ phpize
    $ ./configure
    $ make
    $ sudo -s
    # make install
    # cd /etc/php5/mods-available
    # echo extension=hitsuji.so > hitsuji.ini
    # cd /etc/php5/conf.d
    # ln -s ../mods-available/hitsuji.ini ./30-hitsuji.ini
    
    
### ルーター ###

```php
<?php
include dirname(__FILE__).'/bootstrap.php';

hitSuji::router(
)->always(function () {
    echo <<< HTML
<html>
<head>
  <title>always</title>
</head>
<body>
  hello
</body>
</html>
HTML;
})->on('get', '/user/:id', function ($id) {
    echo <<< HTML
<html>
<head>
  <title>test</title>
</head>
<body>
  {$id}
</body>
</html>
HTML;
})->on('post', '/search', 'search.php'
)->run();

```

### デリゲート ###

```php
$template = hitSuji::view();
$template->layout('layout.tpl');

hitSuji::delegate(
)->binds(
    array(
        'name' => array('string', 'require'),
        'uid'  => 'number',
    )
)->parse(
    /**
     * パース処理
     *
     * @access public
     * @param array $data
     * @return array
     */
    function ($data) {
        $data['name'] = strtolower($data['name']);
        return $data;
    }
)->action(
    /**
     * アクション処理
     *
     * @access public
     * @param array $data
     * @return array
     */
    function ($data) {
        $gdb = new Groonga('./db/test.db');
        return $gdb->table('Users')
                   ->select()
                   ->matchColumns('name,location_str,description')
                   ->query($data['name'])
                   ->outputColumns('_key,name')
                   ->exec(true);
    }
)->always(
    /**
     * デフォルト処理
     *
     * @access public
     * @param array $values
     * @return void
     */
    function ($values) use($template) {
        $template->content('default.tpl');
        $template->display();
    }
)->done(
    /**
     * 成功時処理
     *
     * @access public
     * @param array $values
     * @return void
     */
    function ($values) use($template) {
        $template->content('success.tpl');
        $template->assigns($values);
        $template->display();
    }
)->fail(
    /**
     * 失敗時処理
     *
     * @access public
     * @param array $values
     * @param array $checked
     * @return void
     */
    function ($values, $checked) use($template) {
        $template->content('error.tpl');
        $template->assign('error', 'エラーです。');
        $template->display();
    }
)->run();
```
    

ライセンス
----------
Copyright &copy; 2014 Yujiro Takahashi  
Licensed under the [MIT License][MIT].  
Distributed under the [MIT License][MIT].  

[MIT]: http://www.opensource.org/licenses/mit-license.php
