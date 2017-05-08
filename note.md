# "30日でできる! OS自作入門" 読書メモ

## 作業メモ
### Day1
* QEMUから実行することにしたが、画面出力については以下のようなqemuのオプションを試したがうまく行かず、libvirtを通してSPICEで画面出力を実施するようにしてうまく行った
    - `-vga std`
    - `-nographic`
    - `-serial stdio`
* 参考
    - [Linux環境で本書を読み進めるヒント](http://d.hatena.ne.jp/big-eyed-hamster/20081229/1230562654)
    - [FAT12のバイナリフォーマットについて](http://www.dfists.ua.es/~gil/FAT12Description.pdf)
### Day2
* 参考
    - [Linux環境で本書を読み進めるヒント](http://d.hatena.ne.jp/big-eyed-hamster/20090106/1231182085)
    - [NASMのORGについて](http://www.nasm.us/doc/nasmdoc7.html)
    - [OpCode逆引き表](http://ref.x86asm.net/coder32.html)
### Day3
* 0x8200 から 18 * 2 * 10 sector = 0x08200 - 0x34fff に第2セクタからの中身をロード ( 0x08000 - 0x081ff にはあとで第1セクタの中身をロード )
    - メモリマップ上空いているとことして選定
* Debug は QEMU の Monitor が使いやすい
    - QEMUのGUIからCtrl + Alt + 2
    - GDBで接続する方法もある
        + `-S`, `-gdb tcp::1234` などのオプション付きでQEMU実行
        + gdbから `target remote localhost:1234` のようにする
    - `info register` あるいは `print $eax` などでレジスタの内容を確認したり，`x /64x 0x7c00` のようにしてメモリ上の情報を確認できる
* 参考
    - [BIOS interrupt call](https://en.wikipedia.org/wiki/BIOS_interrupt_call)
    - [x86 Assembly Commands List](https://en.wikibooks.org/wiki/X86_Assembly/Control_Flow)
    - [QEMU Monitor](https://en.wikibooks.org/wiki/QEMU/Monitor)
    - [Debug Linux kernel on QEMU from GDB](https://www.hiroom2.com/2014/01/15/qemu%E4%B8%8A%E3%81%AElinux%E3%82%AB%E3%83%BC%E3%83%8D%E3%83%AB%E3%82%92gdb%E3%81%A7%E3%83%87%E3%83%90%E3%83%83%E3%82%B0%E3%81%99%E3%82%8B/)

## ノート
### アセンブリ
* アセンブリ言語にもNASM, MASM, GASなど複数あり、構文も違う
    - NASMではアドレスオペランドに `[]` を使い、定数やレジスタにプレフィクスはいらない
    - GASではレジスタに `%` を,定数に `$` をプレフィクスとして付け、アドレスオペランドは `()` を使う
    - ラベルはNASMではそのままオペランドに指定してアドレスとして解釈され, GASでは `$` プレフィクスによりアドレスとして解釈できる( `$` なしではアドレスにある内容を示す)(jmpはどちらもラベルをそのまま指定でよい)
* ORG directive
    - NASM: プログラムの開始位置を指定 (=section内でのアドレス参照のオフセットを定める)
    - MASM: object file内の書き込み位置指定
* MOV: データの転送（代入）
    - NASMでは第1オペランドが転送先, 第2オペランドが転送元でGASはその逆
* INT: ソフトウェア割り込み
    - BIOSに定義された関数を呼び出せる
* HLT: 外部割り込みがあるまでCPUを停止する
* ADD: 加算
    - 指定されたレジスタに指定された値を加算する
    - GASではaddl=符号なし, adda=符号ありの加算となる
* x86のレジスタ
    - 名前と意味
        + AX(AH,AL): accumulator
        + CX(CH,CL): counter
        + DX(DH,DL): data
        + BX(BH,BL): base
        + SP: stack pointer
        + BP: base pointer
        + SI: source index
        + DI: destination index
        + ES: esxtrasegment
        + CS: code segment
        + SS: stack segment
        + DS: data segment
        + FS
        + GS
    - bit
        + 8bit: AH, AL, ...
        + 16bit: AX, ..., SP, ...
        + 32bit: EAX, ..., ESP, ...
        + 64bit: RAX, ..., RSP, ...
* segment registerには即値をMOVできない
    - x86ではROMからsegment registerにデータ転送するパスがないため、汎用レジスタを介する必要がある

### ブートプロセス
* ドライブの第1セクタ(512byte)に書き込まれたプログラム = IPL を実行
    - ブートセクタ最後2byte 0x55 0xAA かでIPLが置かれているか判断
    - IPLはメモリ上 0x00007c00 - 0x00007dff にロードすることが決まっている
    - 教材のOSはブートセクタ中最初の80byteがFAT12のヘッダ
* ブート中に画面に文字を出力したりディスクを読み込むのはBIOS interrupt callのおかげ
    - real address mode (16bitモード)でのみ動く

### x86 CPU
* real mode (real address mode)
    - 8086互換の動作モード
    - GASでは `.code16` の指定でreal modeになる

### リンカ
* asmの中でラベルのアドレスを代入する部分など, 実際のアドレスはリンカによって解決される
    - `ld hoge.o -T linker_script hoge.bin`
    - バイナリをどうマッピングするかはリンカスクリプトで定義する
    - `as` の出力バイナリを `readelf -s` すればシンボルとアドレスが分かる

## 疑問
* IPLからディスプレイに画面出力される部分の仕組み
    - HDMIやDVIで接続したディスプレイは文字情報でなくレンダリング後の情報を受け取るはず
    - キャラクタから実際のビットマップにレンダリングしているのは一体どの部分なのか
    - BIOS画面などでもTUIを出せているのでかなりハードウェアに近いレイヤであるとは思う
    - *INTで呼び出している, BIOSに実装されたコードが実現している？*
* リンカの詳細
    - アセンブル時に解決していないアドレスは0埋めになっているようだが、リンカはシンボルに対応するアドレスをどこに差しこめばよいか(シンボルがどこでつかわれているか)をどうやって判断しているのか？
    - リンク時にアドレスを解決するのでは, メモリ上のどこにロードされるかわからないのではないか？
        + IPLについては, BIOSが0x00007c00にロードすると決まっている？
* BIOSについて
    - BIOSはどうやって作っているのか？M/Bのメーカーが利用しているチップセットにあわせて組み込み開発に近いことをしているのか？
    - UEFIの場合は何がどう変わったのか？
* デバッグテクニック関連
    - QEMU MonitorでCtrl+PageUpなどでもスクロールバック出来ない
