# ModnarCompilerTools

&#160; &#160; &#160; &#160; 关于“编译”的实用工具！

## GrammarParser

&#160; &#160; &#160; &#160; 文法解析工具，用此工具来判断你的文法文件是否是相应文法。

&#160; &#160; &#160; &#160; 目前支持判断的类型有`SLR(1)`和`LR(1)`类型两种。

### 用法说明

&#160; &#160; &#160; &#160; 使用编译后生成的目标文件`analyze`来检查你的文法。

&#160; &#160; &#160; &#160; 你可以这样使用：

```bash
$ ./analyze -all ../res/txt/grammar.txt
```

&#160; &#160; &#160; &#160; 用此命令来检查grammar.txt是否符合`SLR(1)`和`LR(1)`文法定义。

&#160; &#160; &#160; &#160; 运行结果展示：

![GrammarParser execution output](../res/img/output.png)

&#160; &#160; &#160; &#160; 命令参数说明：

``` 
Usage: $ analyze -[option] filepath [-nil='null']
```

- option: 必要参数，指定检查范围，可选值包括`-slr`、`-lr`和`-all`。其中`-all`是指包括所有可能的选项。

- nil='null': 可选参数，指定文法中的“空”用什么字符/字符串来表示。例如编译实验中常用的以“$”来作为该标记。当然，你的文法中可能直接使用“empty”来作为“空”标记，只需要加上选项`-nil='empty'`即可，就像这样：

```bash
$ ./analyze -all ../res/txt/grammar.txt -nil='empty'
```

### 安装

&#160; &#160; &#160; &#160; 下载并编译(附赠Makefile)，得到`analyze`可执行文件，将其作为命令行可执行程序并运行即可。当然，你也可以将其移动到`/usr/local/bin/`下(Unix/Linux下且确保你的权限足够)，以使其更加方便使用。

- SSH:

```bash
$ git clone git@github.com:Modnars/Compiler.git
$ cd Compiler/Tools/src/
$ make
```

- HTTPS:

```bash
$ git clone https://github.com/Modnars/Compiler.git
$ cd Compiler/Tools/src/
$ make
```

## 声明

> Copyright (c) 2020 Modnar. All rights reserved.
