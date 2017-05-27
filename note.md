# "30日でできる! OS自作入門" 読書メモ

## 作業メモ
### Day1
* ~~QEMUから実行することにしたが、画面出力については以下のようなqemuのオプションを試したがうまく行かず、libvirtを通してSPICEで画面出力を実施するようにしてうまく行った~~
  (追記: gtkフラグ有効でqemuをビルドすればコマンドラインからの実行でもSPICE経由での出力が出来た)
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
* 本ではディスクイメージ 0x4200 以降にファイルが書かれるとあるが、自分の環境では0x4400以降に書き込まれるようだった
* 3-9における手順は、Cから直接objファイル生成->リンカスクリプトで.textセクション抽出->catで結合 とした
* bootpack.cの呼び出しはリンカスクリプトの工夫で簡素化 [参考](http://d.hatena.ne.jp/papamitra/20060511/1147361738)
    - 本ではbootpack.hrbと、bootpack.cから作ったバイナリをBOOTPACKラベル以降に順につなぎ、.dataセクションについては独自にスタック領域の隣にロードして呼び出している(リンクの際にもその前提)
    - リンカスクリプトによりbootpack.cがロードされるアドレスを基準にアドレスを解決するようにして、.dataセクションもそのままバイナリに封入
* 参考
    - [BIOS interrupt call](https://en.wikipedia.org/wiki/BIOS_interrupt_call)
    - [x86 Assembly Commands List](https://en.wikibooks.org/wiki/X86_Assembly)
    - [QEMU Monitor](https://en.wikibooks.org/wiki/QEMU/Monitor)
    - [Debug Linux kernel on QEMU from GDB](https://www.hiroom2.com/2014/01/15/qemu%E4%B8%8A%E3%81%AElinux%E3%82%AB%E3%83%BC%E3%83%8D%E3%83%AB%E3%82%92gdb%E3%81%A7%E3%83%87%E3%83%90%E3%83%83%E3%82%B0%E3%81%99%E3%82%8B/)
    - [Comparison of GAS and NASM](https://www.ibm.com/developerworks/library/l-gas-nasm/)
    - [ELFについて](http://softwaretechnique.jp/OS_Development/Tips/ELF/elf01.html)
### Day4

## ノート
### アセンブリ
* アセンブリ言語にもNASM, MASM, GASなど複数あり、構文も違う
    - NASMではアドレスオペランドに `[]` を使い、定数やレジスタにプレフィクスはいらない
    - GASではレジスタに `%` を,定数に `$` をプレフィクスとして付け、アドレスオペランドは `()` を使う
    - ラベルはNASMではそのままオペランドに指定してアドレスとして解釈され, GASでは `$` プレフィクスによりアドレスを即値として解釈できる( `$` なしではアドレスにある内容を示す)(jmpはどちらもラベルをそのまま指定でよい)
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
* OUT: レジスタの内容を指定されたI/Oポートに出力する
* CLI: ハードウェア割り込み実行の禁止
* CALL: サブルーチン呼び出し
    - 次の命令アドレスをスタックにプッシュして指定したアドレスにジャンプ
* JMP: 指定アドレスにジャンプ
    - NASMの場合、ロングジャンプは `jmp section:offset`
    - GASの場合、ロングジャンプは `ljmp $section, $offset`
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
* byte directiveなどでは即値しか取り得ないのでGASでも `$` 指定不要
* C言語から呼び出す場合
    - 4(%esp), 8(%esp), 12(%esp), ... に1,2,3,... 番目の引数が入る
    - C言語の動作に影響させないためEAX, ECX, EDXしか変更してはいけない
* 逆アセンブル例: `objdump -D -Matt,i386,addr16,data16 -b binary -m i386 binary-without-header.img`
    - 16-,32-bit modeなどはaddr16, data16をいじる

### ブートプロセス
* 最初はBIOSがドライブの第1セクタ(512byte)に書き込まれたプログラム = IPL を実行
    - ブートセクタ最後2byte 0x55 0xAA かでIPLが置かれているか判断
    - IPLはメモリ上 0x00007c00 - 0x00007dff にロードすることが決まっている
    - 教材のOSはブートセクタ中最初の80byteがFAT12のヘッダ
    - サイズが限定されるので、ドライブの内容をメモリにロードして実行（ジャンプ）するのが役目
* ブート中に画面に文字を出力したりディスクを読み込むのはBIOS interrupt callのおかげ
    - real address mode (16bitモード)でのみ動く
* IPLでブートドライブの内容(OSのイメージ含む)をメモリ上にロードし、OSのイメージの実行へジャンプ
* IPLからジャンプして実行されるプログラムでは、主にreal modeからprotected modeなどに移行しつつ処理をCなどで書かれたOSの中心的な部分にうつす
    - 移行の際には本来Cで考えられるような呼び出し規約をasmで実装する必要がある

### x86 CPU
* real mode (real address mode)
    - 8086互換の動作モード
    - GASでは `.code16` の指定でreal modeの想定でアセンブルする
    - 32-bit modeに移行する際気をつけること
        + `.code32` directiveを適切に利用する
* PIC(Programmable Interrupt Controller)
    - エンコーダICに近く、割り込み線からの入力を割り込み番号にして送信
    - 割り込みにマスクを掛けたりもする

### リンカ
* コンパイル後のリンクする前のファイル=object file
    - リロケーション情報・シンボル情報・デバッグ情報などのメタデータを含んでいる
    - フォーマットには種類があり、LinuxのデフォルトはELF
        + ELFはobject fileだけでなく、実行ファイルやshared object fileにも利用される
        + 以下の部分から構成される
            * ELF header
                - object fileのタイプやアーキテクチャの情報などを持つ
                - entry pointとなるアドレスも記載されている
                - 他の部分の開始位置やエントリ数も持っている
            * program header table
                - object fileではoptional
            * section/segment(複数)
                - 命令コード, データ, シンボル情報, リロケーション情報などはそれぞれsectionに収められる
                - 特殊なsectionとして、.bss, .data, .rodata, .text, .rel.text, ...
            * section header table
                - 実行ファイルではoptional
                - object file内のすべてのsectionに対応するそれぞれのsection headerを格納する
                - section headerはsection名(正確にはsection名のセクション文字列テーブル.shstrtab内でのインデックス)やsectionのサイズなどを持っている
    - text segment:
* asmの中でラベルのアドレスを代入する部分など, 実際のアドレスはリンカによって解決される
    - `ld hoge.o -T linker_script hoge.bin`
    - バイナリをどうマッピングするかはリンカスクリプトで定義する
    - `as` の出力バイナリを `readelf -s` すればシンボルとアドレスが分かる
    - アドレス解決の際に利用されるアドレス（例えば.dataセクションがどのあ
    ドレスに存在するかなど）は、Virtual Memory Address(VMA)と呼ぶ
    - 出力されたファイル内のアドレスはLoad Memory Address(LMA)
    - LMAはAT属性により指定可能

## 疑問
* IPLからディスプレイに画面出力される部分の仕組み
    - HDMIやDVIで接続したディスプレイは文字情報でなくレンダリング後の情報を受け取るはず
    - キャラクタから実際のビットマップにレンダリングしているのは一体どの部分なのか
    - BIOS画面などでもTUIを出せているのでかなりハードウェアに近いレイヤであるとは思う
    - *INTで呼び出している, BIOSに実装されたコードが実現している？*
* リンカの詳細
    - アセンブル時に解決していないアドレスは0埋めになっているようだが、リンカはシンボルに対応するアドレスをどこに差しこめばよいか(シンボルがどこでつかわれているか)をどうやって判断しているのか？
        + *object fileの.rel.textセクション内に使われている場所に関する情報がある*
    - リンク時にアドレスを解決するのでは, メモリ上のどこにロードされるかわからないのではないか？
        + IPLについては, BIOSが0x00007c00にロードすると決まっている？
* BIOSについて
    - BIOSはどうやって作っているのか？M/Bのメーカーが利用しているチップセットにあわせて組み込み開発に近いことをしているのか？
    - UEFIの場合は何がどう変わったのか？
* デバッグテクニック関連
    - QEMU MonitorでCtrl+PageUpなどでもスクロールバック出来ない
    - virt-managerから作成したVMで実施するとディスプレイ初期化(INT 0x10)まで到達しない上、QEMU Monitorに接続できず、gtkオプション有効のQEMUコマンドをCLIから実施した場合はそれらがうまく行く
        + 内部的に渡すコマンドラインオプションの違いが影響している？？？
* 3日目9章の100行
    - なぜスタックの場所が0x00310000なのか
    - GDTの意味、内容
