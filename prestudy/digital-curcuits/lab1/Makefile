TOPNAME = top
INC_PATH ?=

VERILATOR = verilator

BUILD_DIR = ./build
OBJ_DIR = $(BUILD_DIR)/obj_dir
BIN = $(BUILD_DIR)/$(TOPNAME)
WAVE= $(BUILD_DIR)/dump.vcd

VSRCS = $(shell find $(abspath .) -name "*.v")
CSRCS = $(shell find $(abspath .) -name "*.c" -or -name "*.cc" -or -name "*.cpp")

VERILATOR_CFLAGS += -MMD --build -cc  \
				-O3 --x-assign fast --x-initial fast --noassert --trace

INCFLAGS = $(addprefix -I, $(INC_PATH))
CFLAGS += $(INCFLAGS) -DTOP_NAME=V$(TOPNAME) -DWAVEFILE=\\\"$(WAVE)\\\"
# LDFLAGS += -lSDL2 -lSDL2_image

$(BIN): $(VSRCS) $(CSRCS)
	@rm -rf $(OBJ_DIR)
	$(VERILATOR) $(VERILATOR_CFLAGS) \
		--top-module $(TOPNAME) $^ \
		$(addprefix -CFLAGS , $(CFLAGS)) $(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(OBJ_DIR) --exe -o $(abspath $(BIN))

run: $(BIN)
	@$^

wave: run
	gtkwave $(WAVE)

clean:
	rm -rf $(BUILD_DIR)

default: $(BIN)

all: default

# ensure
$(shell mkdir -p $(BUILD_DIR))

.PHONY: default all clean run wave
