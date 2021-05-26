## MD4 C语言实现

注意：该程序具有字节序依赖，只有在小端字节序的机器上(如x86、amd64)才能得到正确的结果。

### 编译运行
```
gcc -o main main.c md4.c
./main
```

### 测试结果
```
MD4("The quick brown fox jumps over the lazy dog")
= 1bee69a46ba811185c194762abaeae90
MD4("The quick brown fox jumps over the lazy cog")
= b86e130ce7028da59e672d56ad0113df
```