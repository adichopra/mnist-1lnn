import subprocess
import os

CC=["gcc"]
CFLAGS=[]
LDFLAGS=[]

def compile(source_name, target_name, macros):
    macro_flags = ["-D{}={}".format(name, macros[name]) for name in macros]
    command = CC + CFLAGS + macro_flags + ["-c", source_name, "-o", target_name]
    print(' '.join(command))
    return subprocess.call(command)

def link(source_names, target_name):
    command = CC + LDFLAGS + source_names + ["-o", target_name]
    print(' '.join(command))
    return subprocess.call(command)

def mac_to_hex(macaddr):
    return '0x' + ''.join(reversed(macaddr.split(":")))

# A -> 0 -> 1 -> 2 -> 3 -> 4 -> A
FIRST_NODE_MAC = "00:12:6D:00:00:{:02x}".format(2)
MACS = [FIRST_NODE_MAC]
MACS += ["00:12:6D:00:00:{:02x}".format(i) for i in range(3, 8)]
MACS.append(FIRST_NODE_MAC)
PACKET_WORDS = 200

def main():
    if not os.path.isdir("testbuild"):
        os.makedirs("testbuild")

    compile(
        "firstnode.c",
        "testbuild/firstnode.o",
        {"PREV_MACADDR": mac_to_hex(MACS[5]),
         "NEXT_MACADDR": mac_to_hex(MACS[1]),
         "PACKET_WORDS": PACKET_WORDS})
    link(["testbuild/firstnode.o"], "testbuild/firstnode")

    for i in range(1, 6):
        BASE = "testbuild/node{}".format(i - 1)
        compile(
            "main.c",
            BASE + ".o",
            {"START": i-1,
             "PREV_MACADDR": mac_to_hex(MACS[i - 1]),
             "NEXT_MACADDR": mac_to_hex(MACS[i + 1]),
             "PACKET_WORDS": PACKET_WORDS})
        link([BASE + ".o"], BASE)

if __name__ == "__main__":
    main()
