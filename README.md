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
ini_set('hitsuji.page_path', dirname(__FILE__).'/../app/pages/');

hitSuji::router([
    '' => function () {
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
    },
    '/user/:id' => [function ($id) {
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
    }, 'get'],
    '/search' => ['post', 'search.php']
]);

```

### デリゲート ###

```php
ini_set('hitsuji.template_path', dirname(__FILE__).'/../app/templates/');

$template = hitSuji::template();
$template->layout('layout.tpl');

hitSuji::delegate([
    /**
     * 直データ
     *
     * @var array
     */
    'data' => [
        'sid'=> 'sid90'
    ],
    /**
     * 入力データ定義
     *
     * @var array
     */
    'bind' => [
        'name' => ['string', 'require'],
        'uid'  => 'number',
    ],
    /**
     * パース処理
     *
     * @access public
     * @param array $data
     * @return array
     */
    'parse'=> function ($data) {
        $data['name'] = strtolower($data['name']);
        return $data;
    },
    /**
     * アクション処理
     *
     * @access public
     * @param array $data
     * @return array
     */
    'action'=> function ($data) {
        $gdb = new Groonga('./db/test.db');
        return $gdb->table('Users')
                   ->select()
                   ->matchColumns('name,location_str,description')
                   ->query($data['name'])
                   ->outputColumns('_key,name')
                   ->exec(true);
    },
    /**
     * 成功時処理
     *
     * @access public
     * @param array $values
     * @return void
     */
    'always' => function ($values) use($template) {
        $template->content('success.tpl');
        $template->assigns($values);
        $template->display();
    },
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
    },
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
]);

```
    

ライセンス
----------
Copyright &copy; 2015 Yujiro Takahashi  
Licensed under the [MIT License][MIT].  
Distributed under the [MIT License][MIT].  

[MIT]: http://www.opensource.org/licenses/mit-license.php
