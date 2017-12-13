if ! hash "$riscv64-unknown-elf-gcc" 2>/dev/null; then
	cd ~/firesim
	source ./env.sh
fi
cd ~/firesim/sim/nodes
python build.py $1
