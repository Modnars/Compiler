# Grammar Parser

GrammarParser 是一个通用文法解析工具，其设计原理严格采用《编译原理》中说明的内容，目前已经完整支持解析 LR(1) 类型文法，LR(1) 文法也是诸多上下文无关文法中最为复杂的文法，其复杂程度主要表现在解析生成的中间状态集庞大、数量众多，而支持这样的文法解析的好处在于，对于较为复杂的文法，本工具都能更好、更普适、更准确地进行支持。对于其他类型的文法，设计文法时需要更多的考虑与回避（比如 LL(1) 类型语法需要文法中不能存在左递归生成式），因此优先实现一种最为强大的语法解析类型，可以极大地方便后续流程的设计与实现。后续，依然会拓展实现对 SLR(1)、LALR 类型文法的支持，他们在用于对一些文法进行分析时，状态集数量会明显降低，但同时支持的文法也有一定的限制，后续实现时会展开说明。

## 使用说明

```text
Usage: GrammarParser --file GRAMMAR_FILE [OPTION]
  -f, --file     Specify the source grammar file.
  -h, --help     Show the text and exit.
  -i, --input    Redirect the input stream with input file. If not set, the input stream is stdin.
  -o, --output   Output the ActionTable (in LR) and the PredictionTable (in LL) to file.
                 Output file types: Graphviz(.dot, `-o dot`), CSV(.csv, `-o csv`) and Golang(.go, `-o go`).
  -v, --version  Show the version information and exit.
  -V, --verbose  Enable the verbose mode.
  --ll1          Parse the Grammar File with LL(1).
  --slr          Parse the Grammar File with SLR(1).
  --lr1          Parse the Grammar File with LR(1).
```

比如，执行命令 `./GrammarParser -f test/grammar-file/excelconfc.txt --lr1` 即可按照 LR(1) 语法文件 `excelconfc.txt` 来进行语法解析。执行过程主要包括：计算文法中非终结符的 FIRST 集、生成文法对应的 LR(0) 项集合、计算 LR(1) 项集合、计算每个 LR(1) 项集合的闭包、计算 LR(1) 项集合闭包间的转移函数、填充 LR(1) 文法动作表（ActionTable，包括移入、规约动作）。其中填充动作表用于检查是否存在 移入/移入 冲突、移入/规约 冲突，如果不存在冲突项，就认为文法本身可以按照 LR(1) 文法来进行分析，也就是说这是一个 LR(1) 文法。

## 说明

对于一种语法的复杂程度而言，几种常见的语法分析类型所能支持的分析能力是不同的。一般而言，`LL(1)` 的分析能力最弱，这表示如果希望语法能够被 `LL(1)` 文法进行分析，往往需要语法本身满足一定的条件——比如不能包含左递归，这对于语法来说条件更为苛刻，甚至在这样的条件下，一些串无法按照语法生成出来，因此说 `LL(1)` 文法所能支持的分析能力，是最弱的。按照这样的分析能力强弱来排序，那么几种常见的分析文法之间的分析能力强弱顺序就是 `LL(1)` < `SLR(1)` < `LALR` < `LR(1)`。

因此，对于一个语法而言，其如果能够被 `LL(1)` 文法执行语法分析，那么这个语法也能被其余的三种文法进行语法分析。值得一提的是，`LL(1)` 中的第一个 `L` 表示从左向右扫描输入，第二个 `L` 表示产生最左推导，而 `1` 则表示在每一步中只需要向前看一个输入符号来决定语法分析动作。`LR` 也就是从左向右扫描输入的同时，按照产生最右推导（也称为规范推导）的方式来进行分析。对于 `LL` 文法，其分析过程是自顶向下的，即从起始推导式开始，不断“预测”接下来的输入串，从而实现接收；而 `LR` 文法，其分析过程式自底向上的，分析过程中不断尝试最右规约，从而最终将输入串规约成起始推导表达式，实现对于输入串的接收。

对于 `LR` 文法，其引入了 Item 项的概念，通过区分规约过程中的不同读入状态，将项划分成了项集。在项集之间按照处于指定状态时面对的串来进行项集状态的跳转，而如何计算判断面对的串及执行的动作，就是语法解析过程中的重点。`SLR(1)` 文法将每个非终结符号的 FOLLOW 集中的符号做为“面对的串”，因而其构造起来最为简洁方便，且项集数量（对应分析自动机的状态数量）少，因而构造所需的存储空间最小（因为项集中的项的相似程度最大），但缺点就是无论 FOLLOW 集中某个符号是否可能成为“面对的串”，都会被认为是需要执行规约动作，因而往往会导致 `规约/规约` 冲突或 `移入/规约` 冲突。

为了解决这样的“针对性”不足的问题，`LR(1)` 文法将这个“面对的串”抽象成了“展望符”（lookahead）。对于一般语法而言，往往只需要向前看一个符号（一个展望符）就足够确定需要执行移入还是规约动作了，因而 `LR(1)` 的分析能力是非常强的。但为了区分面对不同展望符执行不同的动作，`LR(1)` 不仅在原有的 `SLR(1)` 项（实际上就是 `LR(0)` 项）的基础上增加了对于展望符的存储记录，还使得原来的项集被按照展望符进一步划分了，从而每个复杂语法的 `LR(1)` 项目集数量往往都较为庞大、每个项目集中的项的数量较为繁多。对于项目集中的项的数量，GrammarParser 将展望符存储成为一个集合，在不失功能表达性的同时，让流程更为精简直观。对于这样的每个 `LR(1)` 项目，输出时格式类似于 `[S -> A ·B C, a/b/c]`。其中，`·` 表示分析过程中间的状态，即分析到 `B` 这一非终结符；`a/b/c` 就是这个 `LR(1)` 项的展望符。按照 `LR(1)` 的定义，其实际上相当于三个标准的 `LR(1)` 项。实际上可以看到，展望符本质上是 FOLLOW 集的一个子集，且往往对于几乎所有状态而言，它是 FOLLOW 集的一个真子集。

为了解决 `LR(1)` 状态数量庞大的问题，进一步引出了 `LALR` （向前看-LR）技术。这个方法经常在实践中使用，因为这种方法得到的分析表比规范 LR 分析表小很多，而且大部分常见的程序设计语言构造都可以方便地使用一个 LALR 文法表示。对于 SLR 文法，这一点也基本成立，只是仍然存在少量构造不能够方便地使用 SLR 技术来处理。同前面对于 LR(1) 的原理叙述一般，可以看到 LALR 状态的“面对的串”较 LR(1) 扩大了，它本质上是对那些拥有相同核心项目的 LR(1) 状态集合的 LR(1) 项目的“面对的串”（展望符）取了并集。因此结果就是它的状态数量骤减，等同于那些只拥有核心的项集，因而它的状态集数量和 SLR 是严格相等的，因为他们包含的项目集都是那些核心的项集。而 LALR 对于“面对的串”的拓展却不是“盲目”的，它选取了那些对于 LR(1) 项目有效的“面对的串”，因而保证了 LALR 分析时的正确性。当然，此时 LALR 的“面对的串”，依然是 FOLLOW 集的一个子集。如果将某个项集的“面对的串”进行一个并集计算，用 `LOOKAHEAD()` 来表示这个并集运算，那么这几种 LR 分析技术的这个并集之间的关系就是：`LOOKAHEAD(SLR(1))` ⊇ `LOOKAHEAD(LALR)` ⊇ `LOOKAHEAD(LR(1))`

对于 LOOKAHEAD(SLR(1)) 而言，其实质上就等同于 FOLLOW 运算，即 `LOOKAHEAD(SLR(1)) = FOLLOW(SLR(1))`。

由于 `LALR` 较 `LR(1)` 而言，其单个状态下的“展望符”集合中终结符数量更多，因而更容易触发执行规约动作。对于可能存在的规约冲突等问题，`LALR` 相较于 `LR(1)` 也会更延迟检测出来。因为 `LR(1)` 的状态足够细分，导致自动机执行到某个状态时，如果遇到的终结符不能执行分析动作，就立即报错；而 `LALR` 可能还需要执行几步分析后才能检测出这个错误。

如果对语法分析器的大小做一下比较，一个文法的 SLR 和 LALR 分析表总是具有相同数量的状态，对于像 C 这样的语言来说，通常有几百个状态。对于同样大小的语言，规范 LR 分析表通常有几千个状态。因此，构造 SLR 和 LALR 分析表要比构造规范 LR 分析表更容易，而且更经济。

## 应用

[`excelconfc`](https://github.com/Modnars/excelconfc) 是一个以此工具生成的语法分析动作表为基础的 Excel 配置表解析导出配置文件的工具。其采用的解析文法类型为 LALR(1) 文法，具体文法为 excelconf.mini.txt（可参考测试样例）。

## 测试

为了更为直观地感受文法复杂程度，以及不同分析技术的分析能力，对于提供的几种测试文法，其测试结果如下：

| 文法 \ 分析技术 | LL(1) | SLR(1) | LALR(1) | LR(1) | 备注 |
| :-: | :-: | :-: | :-: | :-: | :-: |
| 4.1 | × | ✓ | ✓ | ✓ | 包含左递归文法的运算表达式文法 |
| 4.2 | ✓ | ✓ | ✓ | ✓ | 消除左递归文法改造后的运算表达式文法 |
| 4.3 | × | × | × | × | 存在二义性的运算表达式文法 |
| 4.7 | × | × | × | × | 存在二义性的运算表达式文法 |
| 4.13 | ✓ | ✓ | ✓ | ✓ | |
| 4.18 | × | ✓ | ✓ | ✓ | |
| 4.23 | × | × | × | × | 存在二义性的 if, else 语法 |
| 4.24 | × | × | × | × | 提取左公因子后的存在二义性的 if, else 语法 |
| 4.28 | ✓ | ✓ | ✓ | ✓ | 消除左递归文法改造后的运算表达式文法，与文法 4.2 相同 |
| 4.49 | × | × | ✓ | ✓ | 左值、右值的赋值文法 |
| 4.55 | ✓ | ✓ | ✓ | ✓ | LR(1) 示例文法 |
| 4.67 | × | × | × | × | 存在二义性的 if, else 语法 |
| Example.4.58 | × | × | × | ✓ | 例 4.58 文法 |
| excelconfc | × | ✓ | ✓ | ✓ | 导表工具 [`excelconfc`](https://github.com/Modnars/excelconfc) 文法 |
| excelconfc.rr | × | ✓ | ✓ | ✓ | 导表工具 [`excelconfc`](https://github.com/Modnars/excelconfc) 右递归文法 |
| excelconfc.mini | × | ✓ | ✓ | ✓ | 导表工具 [`excelconfc`](https://github.com/Modnars/excelconfc) 精简文法 |
| program | × | ✓ | ✓ | ✓ | 自制语言语法 |
| Practice.4.2.1 | × | ✓ | ✓ | ✓ | 练习 4.2.1 文法的增广文法 |
| Practice.4.2.2.1 | × | ✓ | ✓ | ✓ | 练习 4.2.2 (1) 文法的增广文法 |
| Practice.4.2.2.2 | ✓ | ✓ | ✓ | ✓ | 练习 4.2.2 (2) 文法的增广文法 |
| Practice.4.2.2.3 | × | × | × | × | 练习 4.2.2 (3) 文法的增广文法 |
| Practice.4.2.2.4 | × | × | × | × | 练习 4.2.2 (4) 文法的增广文法 |
| Practice.4.2.2.5 | × | ✓ | ✓ | ✓ | 练习 4.2.2 (5) 文法的增广文法 |
| Practice.4.2.2.6 | × | × | × | × | 练习 4.2.2 (6) 文法的增广文法 |
| Practice.4.2.2.7 | × | ✓ | ✓ | ✓ | 练习 4.2.2 (7) 文法的增广文法 |
| Practice.4.6.5 | ✓ | × | ✓ | ✓ | 练习 4.6.5 文法的增广文法 |
| Practice.4.6.6 | × | ✓ | ✓ | ✓ | 练习 4.6.6 文法的增广文法 |
| Practice.4.7.4 | × | × | ✓ | ✓ | 练习 4.7.4 文法的增广文法 |
| Practice.4.7.5 | × | × | × | ✓ | 练习 4.7.5 文法的增广文法 |

## 示例

- 文法 4.49

```text
S' -> S
S -> L = R
S -> R
L -> * R
L -> id
R -> L
```

文法 4.49 本身不是一个 SLR(1) 文法，但它是一个 LR(1) 文法。其 LR(1) 关键分析信息如下：

```text
FIRST SET:
L: * id
R: * id
S: * id
S': * id

STATE CLOSURE:

I0:
[S' -> ·S, #]
[S -> ·L = R, #]
[S -> ·R, #]
[L -> ·* R, =]
[L -> ·id, =]
[R -> ·L, #]
[L -> ·* R, #]
[L -> ·id, #]

I1:
[L -> ·* R, =]
[L -> ·id, =]
[R -> ·L, #]
[L -> ·* R, #]
[L -> ·id, #]
[L -> * ·R, =]
[L -> * ·R, #]
[R -> ·L, =]

I2:
[S -> L ·= R, #]
[R -> L·, #]

I3:
[S -> R·, #]

I4:
[S' -> S·, #]

I5:
[L -> id·, =]
[L -> id·, #]

I6:
[R -> L·, #]
[R -> L·, =]

I7:
[L -> * R·, =]
[L -> * R·, #]

I8:
[R -> ·L, #]
[L -> ·* R, #]
[L -> ·id, #]
[S -> L = ·R, #]

I9:
[R -> ·L, #]
[L -> ·* R, #]
[L -> ·id, #]
[L -> * ·R, #]

I10:
[R -> L·, #]

I11:
[S -> L = R·, #]

I12:
[L -> id·, #]

I13:
[L -> * R·, #]

ACTION TABLE:
0:	*: 1	L: 2	R: 3	S: 4	id: 5
1:	*: 1	L: 6	R: 7	id: 5
2:	#: -6	=: 8
3:	#: -3
4:	#: -1
5:	#: -5	=: -5
6:	#: -6	=: -6
7:	#: -4	=: -4
8:	*: 9	L: 10	R: 11	id: 12
9:	*: 9	L: 10	R: 13	id: 12
10:	#: -6
11:	#: -2
12:	#: -5
13:	#: -4
```

- 文法 4.55

```text
S' -> S
S -> C C
C -> c C
C -> d
```

文法 4.55 本身是一个 LR(1) 文法，同时它也是一个 SLR(1) 文法。简言之，SLR(1) 文法一定是 LR(1) 文法，反之则未必。其 LR(1) 关键信息分析如下：

```text
FIRST SET:
C: c d
S: c d
S': c d

STATE CLOSURE:

I0:
[S' -> ·S, #]
[S -> ·C C, #]
[C -> ·c C, c/d]
[C -> ·d, c/d]

I1:
[S -> C ·C, #]
[C -> ·c C, #]
[C -> ·d, #]

I2:
[S' -> S·, #]

I3:
[C -> ·c C, c/d]
[C -> ·d, c/d]
[C -> c ·C, c/d]

I4:
[C -> d·, c/d]

I5:
[S -> C C·, #]

I6:
[C -> c ·C, #]
[C -> ·c C, #]
[C -> ·d, #]

I7:
[C -> d·, #]

I8:
[C -> c C·, c/d]

I9:
[C -> c C·, #]

ACTION TABLE:
0:	C: 1	S: 2	c: 3	d: 4
1:	C: 5	c: 6	d: 7
2:	#: -1
3:	C: 8	c: 3	d: 4
4:	c: -4	d: -4
5:	#: -2
6:	C: 9	c: 6	d: 7
7:	#: -4
8:	c: -3	d: -3
9:	#: -3
```
