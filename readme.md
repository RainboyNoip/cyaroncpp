## 目的

[luogu-dev/cyaron](https://github.com/luogu-dev/cyaron)的cpp实现

注意：代码只在linux下测试过，windows不知道能不能使用。

## 安装

在终端下粘贴下面的命令，运行

```bash
sh -c "$(curl -fsSL https://ghproxy.com/https://github.com/RainboyNoip/cyaroncpp/blob/master/install.sh)"
```

## hello world

```cpp
// helloword.cpp
#include <cyaroncpp/cyaron.hpp>
#include <string>
#include <iostream>

int main(){
    std::vector<std::string> v{"hello world!","hello world."};
    //随机从v里选一个string输出
    std::cout << cyaron::String::random(v) << std::endl;
    return 0;
}
```

编译：cyaron使用了c++17标签，是`noilinux 2.0`支持的最高标准，编译命令如下

```bash
g++ -std=c++17 -o helloword helloword.cpp
```

## 随机数生成 RND


注意下面的代码默认使用了 ``using namespace cyaron``

```c++
//RND 是一个全局变量，用于生成随机数

//生成一个long long 范围内的随机整数
long long a1 = RND();   

//生成一个[-10,10]范围内的随机整数
int a2 = RND(-10,10); 

//生成一个[0,1.0]范围内的随机double型浮点数
double a3 = RND.random() 
```

## 字符串

```c++

//生成一个随机单词，单词长度为5，从 ALPHABET_SMALL 里挑选字符
std::string a1 = String::random(5);

//生成一个随机单词，单词长度为5，从字符串"abcdefg"里挑选字符
std::string a1 = String::random(5,"abcdefg");

//生成一个随机单词，单词长度为[3,8]之间，从字符"abcdefg"里挑选
std::string a1 = String::random(3,8,"abcdefg");

//从v里随机返回一个字符串
std::vector<std::string> v{"123","456","hello world"};
auto ret = String::random(v);

```

`cyaron`里如下的全局变量
```c++
const std::string ALPHABET_SMALL = "abcdefghijklmnopqrstuvwxyz";
const std::string ALPHABET_CAPITAL = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string ALPHABET = ALPHABET_SMALL + ALPHABET_CAPITAL;
```

**random_sentence**：随机生成句子
```c++

// 随机生成一个长度为5个单词的句子，第一个单词首字母大写，
// 使用空格分隔单词，每个单词的长度在[3，8]之间
// 每个单词从 ALPHABET_SMALL 中挑选
// 句子的结尾字符为 [!.] 中的随机一个
String::random_sentence(5);

// 随机生成一个单词数量为[3,8]之间的句子，第一个单词首字母不大写，
// 使用" ,_"之一分隔单词，每个单词的长度在[3,6]之间
// 每个单词从 "abcde" 中挑选
// 句子的结尾字符为 [!.] 中的随机一个
String::random_sentence( 3,8
    sentence_terminators   = ".!", //名字结尾
    first_letter_uppercase = false, //首字母不大写
    word_separators        = " ,-", //单词间隔
    charset                = "abcde", //单词使用的字符集合
    word_length_range      = {3,6}  // 每个单词的长度
);

```
注意:`sentence_terminators,first_letter_uppercase,word_separators,charset,word_length_range`是类似于Python函数中的**具名参数**，用户不用关心cyaroncpp是如何实现的，只用知道如下：

- 可以只写其中的一些
- 可以不用关心参数顺序

所以也可以像下面这样用

```c++
auto a1 = String::random_sentence(3,8);
auto a2 = String::random_sentence(3,8,charset = "abcded!");
auto a3 = String::random_sentence(3,8, word_length_range={3,6} ,charset = "abcded!");

std::cout << a1 << '\n'
          << a2 << '\n'
          << a3 << '\n';
```

TODO: String::random_paragraph,String::random_regular

## 图

```c++
Graph g1(10); //创建一个有10个点图，默认为无向图

g1.add_edge(1,3); //添加一条边 权值为0
g1.add_edge(1,2,10);//添加一条边 权值为10

//创建一个有10个点图，有向图
Graph g2(10,false);

//使用c++11风格的遍历g1的边
for(const auto &e : g1){
    // e.u 边的起点
    // e.v 边的终点
    // e.w 边的权值
}

int a1 = g1.edges_size(); //得到边的数量
int a2 = g1.point_count();//得到点的数量

//在类graph里定义了两个函数，如下
graph::set_with_head(bool with = false) //设定输出时带有n，m 默认输出
graph::set_with_weight(bool with = false)//设定输出时带有 w 边权 默认输出

//io见下面
io.input_write(g1); //输出这个图，受到set_with_head,set_with_weight的影响
//一行一条边

std::cout << g1;
io << g1;
```

不过在大多数情况下您不需要手动建图，我们为您准备了大量模板，用法如下：

```c++
//创建n个点，m条边的无向图，可以有自环，可以有重复边,边的权为{3,8}之间
Graph g1 = Graph::graph(n,m); 
auto g2 = Graph::graph(n,m  //n个点，m条边
    directed = true,    //有向图
    self_loop = false,  // 不能有自环
    repeated_edges = false, // 不能有重复边
    weight_limit = {1,3}    // 边的权为[1,3]之间
);

//TODO
// Graph::graph_connected // 保证连通

auto tree = Graph::tree(n); //创建n个点树
auto tree2 = Graph::tree(n, //创建n个点树
    chain = 0.4,    // 40%的节点呈现链状
    flower = 0.35 //35%的节点呈现菊花图状,剩余25%的节点随机加入
); 

// 生成一棵n个节点的随机二叉树
auto binary_tree = Graph::binary_tree(n) 
//生成一棵n个节点的二叉树，其中节点有40%的概率是左儿子，35%的概率是右儿子，25%的概率被随机选择
auto binary_tree = Graph::binary_tree(n, left= 0.4, right = 0.35) 


TODO
DAG
UDAG
```


## input/output

IO类可以方便的创建一组测试数据，构造方法如下

```c++
IO io; //从标准输出里输出数据
IO io("data1.in","data1.out") //data1.in做为输入文件，data1.out作为输出文件
IO io(1);
IO io("data",1,".in",".out");
IO io("test",1);
```

```c++
IO io("data1.in","data1.out") //data1.in做为输入文件，data1.out作为输出文件
io.input_write(1,2,3); //向输入文件里写入 "1 2 3"
io << 1 << 2 << 3;     //向输入文件里写入 "123"
io.output_write(1,2,3);//向输入文件里写入 "1 2 3"

std::vector<int> v{1,2,3,4};
io.input_write(1,2,v,4); //这样也可以，会把v里的元素一个一个输出

io.input_writeln(1,2,3); //带换行
io.output_writeln(1,2,3); //带换行
//调用当前目录下的std程序，把输入文件的内容通过stdin送入，获得stdout的内容生成输出
io.output_gen("./std");
```

## 对拍器 Compare

```c++
// 默认比较器为NOIP风格，忽略最后空行和行尾空格
// 以std.out为标准，对比1.out和2.out的正确性
Compare::output(std="std.out","1.out","2.out");

IO std_io(1);
std_io.output_writeln(1, 2, 3) // 往std_io的output里写入一些东西
// 以std_io这个IO对象中的output为标准，对拍1.out和2.out，
Compare::output(std=std_io,"1.out", "2.out" )

TODO Compare::program
```

