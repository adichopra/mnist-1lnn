vec_nn_c_src = \
	main.c \
	1lnn.c \
	mnist-utils.c \
	syscalls.c

vec_nn_riscv_src = \
	crt.S \
	vec_mul_asm.S \


vec_nn_c_objs     = $(patsubst %.c, %.o, $(vec_nn_c_src))
vec_nn_riscv_objs = $(patsubst %.S, %.o, $(vec_nn_riscv_src))

vec_nn_host_bin = vec-nn.host
$(vec_nn_host_bin): $(vec_nn_c_src)
	$(HOST_COMP) $^ -o $(vec_nn_host_bin)

vec_nn_riscv_bin = vec-nn.riscv
$(vec_nn_riscv_bin): $(vec_nn_c_objs) $(vec_nn_riscv_objs)
	$(RISCV_LINK) $(vec_nn_c_objs) $(vec_nn_riscv_objs) -o $(vec_nn_riscv_bin) $(RISCV_LINK_OPTS)

junk += $(vec_nn_c_objs) $(vec_nn_riscv_objs) \
        $(vec_nn_host_bin) $(vec_nn_riscv_bin)
