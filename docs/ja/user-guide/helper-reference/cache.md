---
title: キャッシュ
page_id: "080.035"
---

## キャッシュ

Webアプリケーションでは、ユーザからのリクエストのたびに、データベースへの問い合わせとテンプレートのレンダリングなどの処理が行われ、HTMLデータが生成されています。多くは大したオーバヘッドはかかりませんが、大量のリクエストをさばく大規模なサイトでは無視できないかもしれません。たとえ小規模のサイトであっても、数秒かかるような処理が何度も実行されるのは避けたいでしょう。このような場合では、処理されたデータをキャッシュしておき、同じリクエストがあった時に再利用することが有効な手段になりえます。キャッシュすることでオーバヘッドの軽減につながり、素早いレスポンスを期待できます。

ただ、キャッシュデータを再利用することで古いデータが表示されつづけてしまうという危険性もはらんでいます。
データが更新された際に、その関連するキャッシュデータをどのように扱うかを考える必要があります。キャッシュされたデータを直ちに更新しなければならないのか、それともタイムアウトになるまで古いデータが表示されつづけても問題がないのか。データの性質によって実装を考える必要があります。

また、ユーザ毎に異なるページがあるサイト、例えばプライベートな情報を表示するページでキャッシュを利用する際は注意が必要です。

ページデータをキャッシュする場合には、ユーザ毎に異なるキー文字列でデータをキャッシュすることになります。キー文字列が一意に設定されないと、他のユーザのページが表示されてしまうかもしれません。ただ、このようなケースではキャッシュは全てのユーザに共通なデータのみに絞る方が安全で効果的かもしれませんね。


## キャッシュを有効にする

application.ini ファイルで Cache.SettingsFile のコメントアウト(#)を外し、キャッシュバックエンドのパラメータ 'Cache.Backend'に値を設定します。
```
Cache.SettingsFile=cache.ini

Cache.Backend=sqlite
```
この例では SQLite を設定しました。他に使えるバックエンドとして MongoDB や Redis を設定することができます。


次に、cache.ini を編集して、データベースへの接続情報を設定します。
デフォルトでは次のようになっています。必要に応じて編集してください。
```
[sqlite]
DatabaseName=tmp/cachedb
HostName=
Port=
UserName=
Password=
ConnectOptions=
PostOpenStatements=PRAGMA journal_mode=WAL; PRAGMA busy_timeout=5000; PRAGMA synchronous=NORMAL; VACUUM;
```

バックエンドに MongoDB や Redis を設定した場合は、該当する箇所に接続情報を設定してください。


## ページのキャッシュ

生成されたHTMLデータをキャッシュすることできます。

HTMLデータを生成するには render()関数に対しアクション名を指定（省略も可能）していましたが、ページキャッシュでは renderFromCache(..) にキーと保存する時間を指定することで行います。
キャッシュされたHTMLデータを送信するには、renderFromCache(..) を使います。

例として "index"ビューのHTMLデータを"index"というキーで10秒間キャッシュし、かつキャッシュされたHTMLデータをするには次のようにします。
```
    if (! renderFromCache("index")) {
          :
          :   // get data..
          :
        renderAndStoreInCache("index", 10, "index");
    }
```

indexアクションで実行されるならば 第３引数の"index"は省略できます。
詳しくは [APIリファレンス](http://api-reference.treefrogframework.org/classTActionController.html){:target="_blank"}を参照してください。


## データのキャッシュ

キャッシュしたいのはページだけとは限りません。バイナリやテキストをキャッシュしたいケースもあります。
TCacheクラスを使い、データを保存、読出、削除することができます。

```
  Tf::cache()->set("key", "value", 10);
    :
    :
  auto data = Tf::cache()->get("key");
    :
```

他のメソッドについては [APIリファレンス](http://api-reference.treefrogframework.org/classTCache.html){:target="_blank"}を参照してください。