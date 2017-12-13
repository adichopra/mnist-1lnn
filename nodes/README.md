# Load nodes
- `cd ~/firesim/sim/`
- `../scripts/firstload.sh 6`(if first load) 
- `../scripts/load.sh 6`

# Build switch
- `cd ~/firesim/sw/firesim-nettransport`
- `python manager.py buildswitches`

# Run switch
- `. ~/firesim/sim/nodes/runswitch.sh`

# Build nodes
- `. ~/firesim/sim/nodes/buildnodes.sh <num_images>`

# Run nodes
- `. ~/firesim/sim/nodes/runnode.sh <index>`