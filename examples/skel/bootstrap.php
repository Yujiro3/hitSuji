<?php
/**
 * Bootstrap
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

namespace hitSuji;

/**
 * Bootstrapクラス
 *
 * @package         hitSuji
 * @subpackage      Bootstrap
 * @author          Yujiro Takahashi <yujiro3@gmail.com>
 */
class Bootstrap {
    /**
     * ライフタイムサイクル:8時間
     * @const integer
     */
    const LIFETIME = 28800;

    /**
     * 暗号等に使われる種
     * @const string
     */
    const SEED = 'asdfj5246485902rweld;';

    /**
     * 情報
     * @const array
     */
    public $base_dir;

    /**
     * ディレクトリ
     * @const array
     */
    public $dirs;

    /**
     * 正規表現パターン
     * @const array
     */
    public $patterns;

    /**
     * コンストラクタ
     *
     * @access public
     * @return void
     */
    public function __construct() {
        /* 各種ディレクトリ設定 */
        $this->base_dir = dirname(__FILE__).'/';                // ベース
        $this->dirs = array(
            'base'     => $this->base_dir,                      // ベース
            'app'      => $this->base_dir.'app/',               // アプリケーション
            'lib'      => $this->base_dir.'app/lib/',           // ライブラリ
            'tpl'      => $this->base_dir.'app/templates/',     // ビュー
            'page'     => $this->base_dir.'app/pages/',         // ページ
            'log'      => $this->base_dir.'app/logs/',          // ロギング
            'document' => $this->base_dir.'www/',               // ドキュメント
        );
        $this->patterns = array(
            'string'   => '/[^\/]/i',
            'email'    => '/^[^0-9]+[a-zA-Z0-9_]+([.][a-zA-Z0-9_]+)*[@][a-zA-Z0-9_]+([.][a-zA-Z0-9_]+)*[.][a-zA-Z]{2,4}$/',
            'date'     => '/[0-9]{4}\-?[0-9]{2}\-?[0-9]{2}/',
            'datetime' => '/[0-9]{4}\-?[0-9]{2}\-?[0-9]{2}\s?[0-9]{2}:?[0-9]{2}\:?[0-9]{2}/',
        );

        /* エラー設定 */
        ini_set('error_reporting',        E_ALL);
        ini_set('display_errors',         true);
        ini_set('display_startup_errors', true);
        ini_set('error_log', $this->base_dir.'app/logs/error.log');

        ini_set('date.timezone', 'Asia/Tokyo');
    }

    /**
     * ワンタイムパスワードの取得
     *
     * @access public
     * @param  string $seed
     * @return string Nonceデータ
     */
    public static function nonce($seed) {
        /**
         * 18-01
         * 02-09
         * 10-17
         */
        $now   = strtotime('now');
        $timer = ceil($now / self::LIFETIME);
        
        $plain  = $timer.'key:'.self::SEED.':seed:'.$seed;
        $hashed = md5($plain);

        return substr($hashed, -14, 10);
    }

    /**
     * 引数を返す
     *
     * @access public
     * @return string
     */
    public static function get($key) {
        return $key;
    }

    /**
     * ディレクトリの取得
     *
     * @access public
     * @return string
     */
    public function dir($key) {
        return $this->dirs[$key];
    }

    /**
     * 正規表現パターンの取得
     *
     * @access public
     * @return string
     */
    public function pattern($key) {
        return $this->patterns[$key];
    }
}

\hitSuji::bootstrap();
