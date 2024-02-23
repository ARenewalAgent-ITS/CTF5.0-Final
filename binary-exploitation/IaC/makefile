CXX = gcc
FLAGS = -fno-stack-protector -no-pie
LDFLAGS = -s -lpthread

SRC_DIR = src
OUT_DIR = dist

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OUT_DIR)/%.o,$(SRCS))
TARGET = $(OUT_DIR)/IaC

$(TARGET): $(OBJS)
	$(CXX) $(FLAGS) -o $@ $^ $(LDFLAGS)
	strip $@

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OUT_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OUT_DIR)/*.o
