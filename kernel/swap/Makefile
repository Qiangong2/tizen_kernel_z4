#
# Makefile for the SWAP
#
extra_cflags := -I$(M) -I$(M)/kprobe/arch/$(LINKNAME)/ -I$(M)/uprobe/arch/$(LINKNAME)/
extra_cflags += $(MCFLAGS)
export extra_cflags

obj-$(CONFIG_SWAP_DA) += \
         master/ \
         buffer/ \
         ksyms/ \
         driver/ \
         writer/ \
         kprobe/ \
         uprobe/ \
         us_manager/ \
         ks_features/ \
         loader/ \
         sampler/ \
         energy/ \
         parser/ \
         retprobe/ \
         webprobe/ \
         preload/ \
         fbiprobe/ \
         wsp/ \
         nsp/ \
         task_ctx/ \
         uihv/ \
         got_patcher/

