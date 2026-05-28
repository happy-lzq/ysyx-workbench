#include <difftest.h>

uint8_t* guest_to_host(paddr_t paddr) {
    return npc_pmem + paddr - RESET_VECTOR;
}

 void build_named_log_file(char *buf, size_t buf_size,
                                        const char *path,
                                        const char *default_path,
                                        const char *suffix) {
  Assert(path,"IMG-BIN FILE PATH IS FAIL\n");

  const char *slash = strrchr(path, '/');
  const char *name = slash == NULL ? path : slash + 1;              // 获取文件名起始地址
  const char *dot = strrchr(name, '.');

  size_t dir_len = slash == NULL ? 0 : (size_t)(slash - path + 1);  // 获取目录长度包括/
  size_t base_len = (dot != NULL && dot > name) ? (size_t)(dot - name) : strlen(name);   // 获取名字长度
  // ret 返回值大于0  ret < buf_size 无溢出
  // 拼接逻辑：fix
  int ret = snprintf(buf, buf_size, "%.*s%.*s-%s",
                     (int)dir_len, path,
                     (int)base_len, name,
                     suffix);
  // 按格式化将对应的内容写入buf，且最大数据为buf_size字节大小
  // %.*s 中：.* 对应(int)的动态长度，s 对应字符串
  Assert(ret > 0 && ret < buf_size, "log path is too long: %s", path);
  // Assert(条件，输出文本) 条件成立，不输出；条件不成立，中断输出红色文本
}

