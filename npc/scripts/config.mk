# npc/scripts/config.mk
Q            := @
KCONFIG_PATH := $(NPC_HOME)/tools/kconfig
FIXDEP_PATH  := $(NPC_HOME)/tools/fixdep
Kconfig      := $(NPC_HOME)/Kconfig

CONF   := $(KCONFIG_PATH)/build/conf
MCONF  := $(KCONFIG_PATH)/build/mconf
FIXDEP := $(FIXDEP_PATH)/build/fixdep
# npc/tools/kconfig/build/mconf   # TUI 菜单界面
# npc/tools/kconfig/build/conf    # 配置同步/生成器
# npc/tools/fixdep/build/fixdep   # 配置依赖追踪工具

$(CONF):
	$(Q)$(MAKE) -s -C $(KCONFIG_PATH) NAME=conf

$(MCONF):
	$(Q)$(MAKE) -s -C $(KCONFIG_PATH) NAME=mconf

$(FIXDEP):
	$(Q)$(MAKE) -s -C $(FIXDEP_PATH)

menuconfig: $(MCONF) $(CONF) $(FIXDEP)
	$(Q)$(MCONF) $(Kconfig)
	$(Q)$(CONF) -s --syncconfig $(Kconfig)

%defconfig: $(CONF) $(FIXDEP)
	$(Q)$< -s --defconfig=configs/$@ $(Kconfig)
	$(Q)$< -s --syncconfig $(Kconfig)

.PHONY: menuconfig defconfig