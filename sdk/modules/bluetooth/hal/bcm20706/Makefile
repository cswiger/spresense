############################################################################
# bluetooth/hal/bcm20707/Makefile
#
#   Copyright 2018 Sony Semiconductor Solutions Corporation
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name of Sony Semiconductor Solutions Corporation nor
#    the names of its contributors may be used to endorse or promote
#    products derived from this software without specific prior written
#    permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

# Do not change

-include $(TOPDIR)/Make.defs
-include $(SDKDIR)/Make.defs
DELIM ?= $(strip /)
CXXEXT ?= .cpp

ifeq ($(WINTOOL),y)
INCDIROPT = -w
endif

# INCLUDES
ifeq ($(WINTOOL),y)
INCLUDES := -I"${shell cygpath -w $(APPDIR)/include}"
else
INCLUDES := -I$(APPDIR)/include
INCLUDES += -I$(SDKDIR)/modules/bluetooth/hal/bcm20706/include
endif # ($(WINTOOL),y)

# CFLAGS
CFLAGS += $(INCLUDES)

# C_SRCS
BT_C_SRCS_APP  =  bcm20706_hal.c
ifeq ($(CONFIG_BCM20706_LE),y)
BT_C_SRCS_APP +=  bcm20706_ble_common.c
ifeq ($(CONFIG_BCM20706_LE_GATT),y)
BT_C_SRCS_APP +=  bcm20706_ble_gap.c
BT_C_SRCS_APP +=  bcm20706_ble_gatts.c
endif
endif
ifeq ($(CONFIG_BCM20706_A2DP),y)
BT_C_SRCS_APP +=  bcm20706_bt_a2dp.c
endif
ifeq ($(CONFIG_BCM20706_AVRCP),y)
BT_C_SRCS_APP +=  bcm20706_bt_avrcp.c
endif
BT_C_SRCS_APP +=  bcm20706_bt_common.c
ifeq ($(CONFIG_BCM20706_HFP),y)
BT_C_SRCS_APP +=  bcm20706_bt_hfp.c
endif
ifeq ($(CONFIG_BCM20706_SPP),y)
BT_C_SRCS_APP +=  bcm20706_bt_spp.c
endif
BT_C_SRCS_APP +=  manager/bt_freq_lock.c
BT_C_SRCS_APP +=  manager/bt_hci_manager.c
BT_C_SRCS_APP +=  manager/bt_recv_task.c
BT_C_SRCS_APP +=  manager/bt_uart_manager.c
BT_C_SRCS_APP +=  manager/bt_storage_manager.c

ASRCS =
CSRCS = $(BT_C_SRCS_APP)

AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS)
OBJS = $(AOBJS) $(COBJS)

BIN = libbcm20706.a

DEPPATH = --dep-path . --dep-path $(BT_IF) --dep-path $(BT_MGR)

# Common build

VPATH = $(BT_IF) $(BT_MGR)

all: $(BIN)
.PHONY: context depend clean distclean

$(AOBJS): %$(OBJEXT): %.S
	$(call ASSEMBLE, $<, $@)

$(COBJS) $(MAINOBJ): %$(OBJEXT): %.c
	$(call COMPILE, $<, $@)

$(BIN): $(OBJS)
	$(call ARCHIVE, $@, $(OBJS))

install:

context:

# Create dependencies

.depend: Makefile $(SRCS)
	$(Q) $(MKDEP) $(DEPPATH) "$(CC)" -- $(CFLAGS) -- $(SRCS) >Make.dep
	$(Q) touch $@

depend: .depend

clean:
	$(call CLEAN)
	$(Q) rm -rf manager/*.o

distclean: clean
	$(call DELFILE, Make.dep)
	$(call DELFILE, .depend)

-include Make.dep
.PHONY: preconfig
preconfig:
