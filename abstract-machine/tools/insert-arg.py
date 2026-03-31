#!/usr/bin/env python3
# 用于将makefile脚本参数（mainargs）嵌入到编译生成的二进制文件中，替换掉一个预定义的占位符（placeholder）
# 当二进制文件运行时，main 函数的 args 参数就能接收到实际的参数字符串。
from sys import argv

bin = argv[1]
max_len = int(argv[2])
placeholder = argv[3]          # 占位符字符串
mainargs = argv[4]             # 占位符实际参数内容

if len(mainargs) >= max_len:
    print("Error: mainargs should not be longer than {0} bytes\n".format(max_len))
    exit(1)
print("mainargs={0}".format(mainargs))

fp = open(bin, 'r+b')
data = fp.read()
idx = data.find(str.encode(placeholder))   # 在bin文件数据内查找占位符字符串内容，并返回其第一次出现的字节起始地址idx
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
