// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcore_top.h for the primary calling header

#include "Vcore_top__pch.h"
#include "Vcore_top_core_top.h"

VL_ATTR_COLD void Vcore_top_core_top___ctor_var_reset(Vcore_top_core_top* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+      Vcore_top_core_top___ctor_var_reset\n"); );
    Vcore_top__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18209466448985614591ull);
    vlSelf->debug_pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4804012649788666537ull);
    vlSelf->__PVT__rs1_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 7583249219640028756ull);
    vlSelf->__PVT__rs2_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 16827201314045723821ull);
    vlSelf->__PVT__rd_addr = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 7950012703377089919ull);
    vlSelf->__PVT__mem_read = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13326595402784547387ull);
    vlSelf->__PVT__mem_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11468839172742666876ull);
    vlSelf->__PVT__reg_write = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12827810274228629793ull);
    vlSelf->__PVT__rs1_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11750827288182224484ull);
    vlSelf->__PVT__rs2_rdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 17559880426433436776ull);
    vlSelf->__PVT__pc_sel = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 13338003956286421371ull);
    vlSelf->__PVT__jump_jalr = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6863117063337051265ull);
    vlSelf->__PVT__imm_jal = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4790689783502139995ull);
    vlSelf->__PVT__pc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4211327832146562899ull);
    vlSelf->__PVT__alu_op = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 8715831498092569784ull);
    vlSelf->__PVT__alu_src_a = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5042220309442743189ull);
    vlSelf->__PVT__alu_src_b = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 15720408231426215117ull);
    vlSelf->__PVT__br_type = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 5106123903061439567ull);
    vlSelf->__PVT__lsu_type = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 575966077902606244ull);
    vlSelf->__PVT__reg_wdata_src = VL_SCOPED_RAND_RESET_I(2, __VscopeHash, 8087835073572350899ull);
    vlSelf->__PVT__mem_rdata_raw = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16549395328379310893ull);
    vlSelf->__PVT__u_if_stage__DOT__pc_next = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 15805956212574889517ull);
    for (int __Vi0 = 0; __Vi0 < 65536; ++__Vi0) {
        vlSelf->__PVT__u_if_stage__DOT__imem[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2844471439109414916ull);
    }
    vlSelf->__PVT__u_id_stage__DOT__opcode = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 14377490955449504451ull);
    vlSelf->__PVT__u_id_stage__DOT__funct3 = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 4823391736290649031ull);
    vlSelf->__PVT__u_id_stage__DOT__funct7 = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 385761609146573415ull);
    vlSelf->u_id_stage__DOT__imm_gen_pic__DOT____VdfgRegularize_hebd20cbd_0_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15179502037063025716ull);
    vlSelf->__PVT__u_ex_stage__DOT__src1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16263495455497675834ull);
    vlSelf->__PVT__u_ex_stage__DOT__src2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16504586124116396867ull);
    for (int __Vi0 = 0; __Vi0 < 65536; ++__Vi0) {
        vlSelf->__PVT__u_mem_stage__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10587610104552300845ull);
    }
    vlSelf->__PVT__u_mem_stage__DOT__mem_wmask = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 1554319670473424423ull);
    vlSelf->__PVT__u_mem_stage__DOT__mem_wdata = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6434199343738395092ull);
    vlSelf->__PVT__u_mem_stage__DOT__lsu_pic__DOT__byte_selected = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8710062778087775714ull);
    vlSelf->__PVT__u_mem_stage__DOT__lsu_pic__DOT__half_selected = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 10257239297811676575ull);
    vlSelf->__VdfgRegularize_h3877affd_0_0 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2821362542291781701ull);
    vlSelf->__VdlyVal__u_mem_stage__DOT__mem__v0 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 12755631218110998195ull);
    vlSelf->__VdlyDim0__u_mem_stage__DOT__mem__v0 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 2211828534816366238ull);
    vlSelf->__VdlySet__u_mem_stage__DOT__mem__v0 = 0;
    vlSelf->__VdlyVal__u_mem_stage__DOT__mem__v1 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2662819122831301238ull);
    vlSelf->__VdlyDim0__u_mem_stage__DOT__mem__v1 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 16160039326627985420ull);
    vlSelf->__VdlySet__u_mem_stage__DOT__mem__v1 = 0;
    vlSelf->__VdlyVal__u_mem_stage__DOT__mem__v2 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 14086834389155263063ull);
    vlSelf->__VdlyDim0__u_mem_stage__DOT__mem__v2 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 15107835647734242994ull);
    vlSelf->__VdlySet__u_mem_stage__DOT__mem__v2 = 0;
    vlSelf->__VdlyVal__u_mem_stage__DOT__mem__v3 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 14781698979574984710ull);
    vlSelf->__VdlyDim0__u_mem_stage__DOT__mem__v3 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 17498038502413767604ull);
    vlSelf->__VdlySet__u_mem_stage__DOT__mem__v3 = 0;
}
