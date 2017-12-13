if [[ -z "${RISCV}" ]]; then
  source ~/firesim/sourceme-f1.sh
fi
cd ~/firesim/sim/nodes
python build.py $1
