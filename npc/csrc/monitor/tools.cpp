#include <difftest.h>

#define M_TIME_MASK (1 << 7)
#define M_MEIP_MASK (1 << 11)

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

word_t isa_query_intr(){
    // 1. 全局中断使能未开，直接返回空
    if ((npc_csr(top, CSR_MSTATUS, 0, READ) & 0x8) == 0)
        return INTR_EMPTY;

    // 2. 定时器中断：mip.MTIP 和 mie.MTIE 同时为 1
    if ((npc_csr(top, CSR_MIP, 0, READ) & M_TIME_MASK) &&
        (npc_csr(top, CSR_MIE, 0, READ) & M_TIME_MASK)) {
        // 清除 mip.MTIP 挂起位
        word_t csr_wdata = npc_csr(top, CSR_MIP, 0, READ) & ~M_TIME_MASK;
        npc_csr(top, CSR_MIP, csr_wdata, WRITE);
        return 0x80000007;  // M-mode 时钟中断号
    }

    // 3. 外部中断：mip.MEIP 和 mie.MEIE 同时为 1
    if ((npc_csr(top, CSR_MIP, 0, READ) & M_MEIP_MASK) &&
        (npc_csr(top, CSR_MIE, 0, READ) & M_MEIP_MASK)) {
        return 0x8000000b;  // Machine External Interrupt
    }

    // 4. 其他中断可以继续添加...

    return INTR_EMPTY;
}

word_t isa_raise_intr(word_t trap_cause,vaddr_t npc_pc){

}
void interrupt_check(){
    word_t trap_cause = isa_query_intr();
    if (trap_cause != INTR_EMPTY){
        word_t pc  = npc_pc(top,0,READ);
        word_t npc = isa_raise_intr(trap_cause,pc);
        npc_npc(top,npc,WRITE);
    }
    
}


