# Xiao
 
XiaoはARM Cortex-M0で動作する軽量OSです。
NXP社のLPC1114をターゲットにしています。
μITRON4.0仕様の最低条件に対応しています。
 
# Installation

開発環境としてUbuntu 18.04 LTSを使用しています。

ツールチェインとしてGCCとGNU Binutilsを使用します。
arm-none-eabiで作成してください。
GCC 6.4.0とBinutils 2.29で動作確認しています。

また、カーネルコンフィグレータとしてRubyを用います。
Ruby 2.5で動作することを確認しています。

GNU Makeを使用してビルドします。
```bash
make
```

ファームウェアを焼く際にはlpc21ispを用います。
Versionは1.97を使用しています。

```bash
make image
```

make imageを実行すると以下のコマンドが発行されます
環境に応じて修正してください。

```bash
lpc21isp xiao.hex /dev/ttyUSB0 115200 12000
```

# サポートしているサービスコール

- CRE_TSK()
- DEF_INH()
- act_tsk()
- ext_tsk()
- get_pri()
- get_tid()
- rot_rdq()
- slp_tsk()
- wup_tsk()
- iact_tsk()
- iget_tid()
- irot_rdq()
- iwup_tsk()
 
CRE_TSK()とDEF_INH()はsystem.cfgに定義します。
割り込みハンドラIDのマクロはinterrupt_id.hに定義します。

# Note
 
タスクに割り当てるスタックは最低128Byte(0x80)を割り当てる
必要があります。

# Author
 
作成情報を列挙する
 
* Akihisa ONODA
* Xiao Project
* akihisa.onoda@osarusystem.com
 
# License

```
Copyright (c) 2013-2021 Akihisa ONODA
  
This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
```
