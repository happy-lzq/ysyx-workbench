#include <dlfcn.h>
#include <difftest-def.h>  
#include <memory/paddr.h>

void (*ref_difftest_memcpy)(paddr_t addr, void *buf, size_t n, bool direction) = NULL;
void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL;
void (*ref_difftest_raise_intr)(uint64_t NO) = NULL;

void init_difftest(const char *ref_so_file){
    void *handle = dlopen(ref_so_file, RTLD_LAZY);
    ref_difftest_memcpy     = dlsym(handle, "difftest_memcpy");
    ref_difftest_regcpy     = dlsym(handle, "difftest_regcpy");
    ref_difftest_exec       = dlsym(handle, "difftest_exec");
    ref_difftest_raise_intr = dlsym(handle, "difftest_raise_intr");
    
    void (*ref_difftest_init)(int) = dlsym(handle, "difftest_init");
    ref_difftest_init(1234);
    
}