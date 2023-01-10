# DSA  Assignment 2: Shogi Implementation

## 實現功能

* 棋駒移動
* 棋駒昇變
* 駒台及持駒打入
* 棋譜輸出
* 輸出一般網站可使用的棋譜

## 執行方式

```bash
make && ./obj/main	
```

## 使用方式
### 基本操作
系統將會提示輸入先手方之棋步，請依照以下規則輸入：
`[初始座標（直行，橫列）] [結束座標（直行，橫列）] [棋駒羅馬簡寫]`。
座標之`(1,1)`位於棋盤先手方之右上角。
範例如下：

```
77 76 fu
33 34 fu
88 22 kaku
31 22 gin 
00 55 kaku //持駒打入，初始座標為00
```

若棋駒可昇變，將會提示
```
`<<成りますか？>>(naru, narazu)`
```
請輸入`naru`以昇變或`narazu`以保持為原棋駒。
若要悔棋，可以輸入
```
revert
```
；若要退出，請輸入
```
quit
```
。退出之後，此程式會紀錄該次棋局之棋譜，存放於
```
./kifu.sfenlist
```
存放，待回放使用。
### 單行sfen棋譜讀入
請執行以下指令
```
$ ./main --readsfen ${SFENFILE_LOCATION}	     
```
以使用sfen格式繼續中斷的棋局，並繼續遊玩。
您可以使用 [Lishogi](https://lishogi.org/analysis/) 以生成一個新盤面。

### 棋譜瀏覽
本棋譜瀏覽功能僅限讀入本程式之`sfenlist` 檔案。請使用
```
$ ./main --replay ${SFENLIST_FILE_LOCATION}	     
```
並使用`f, b` 前後瀏覽。退出時按下 `q` 即可。 