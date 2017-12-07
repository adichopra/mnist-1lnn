cd ~/firesim/sim
sim_args=$(cat runtime-configs/default.conf)

case "$1" in
	0)
		sudo LD_LIBRARY_PATH=$(pwd)/output/f1/FireSimRocketChipConfig ./output/f1/FireSimRocketChipConfig/FireSim-f1 ${sim_args} +slotid=0 ~/firesim/sim/nodes/testbuild/firstnode.riscv
		;;
	[1-5])
		sudo LD_LIBRARY_PATH=$(pwd)/output/f1/FireSimRocketChipConfig ./output/f1/FireSimRocketChipConfig/FireSim-f1 ${sim_args} +slotid=$1 ~/firesim/sim/nodes/testbuild/node$(($1-1)).riscv
		;;
esac