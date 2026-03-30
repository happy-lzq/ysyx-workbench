#!/usr/bin/env python3
# 用于将命令行参数（mainargs）嵌入到编译生成的二进制文件中，替换掉一个预定义的占位符（placeholder）
# 当二进制文件运行时，main 函数的 args 参数就能接收到实际的参数字符串。
from sys import argv

bin = argv[1]
max_len = int(argv[2])
placeholder = argv[3]
mainargs = argv[4]

if len(mainargs) >= max_len:
    print("Error: mainargs should not be longer than {0} bytes\n".format(max_len))
    exit(1)
print("mainargs={0}".format(mainargs))

fp = open(bin, 'r+b')
data = fp.read()
idx = data.find(str.encode(placeholder))
if idx == -1:
    print("Error: placeholder not found!\n")
    exit(1)
fp.seek(idx)
mainargs_pad = str.encode(mainargs) + ((max_len - len(mainargs)) * str.encode("\0"))
if len(mainargs_pad) != max_len:
    print("Error: len(mainargs_pad) != max_len\n")
    exit(1)
fp.write(mainargs_pad)
fp.close()
