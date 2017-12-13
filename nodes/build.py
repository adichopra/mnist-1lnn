import subprocess
import os
import sys

CC=["riscv64-unknown-elf-gcc"]
CFLAGS=["-mcmodel=medany", "-Wall", "-O2", "-fno-common", "-fno-builtin-printf", "-Wno-missing-braces"]
LDFLAGS=["-T", "link.ld", "-static", "-nostdlib", "-nostartfiles", "-lgcc"]

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

def ltoa(i):
    return str(i) + "L"

# A -> 0 -> 1 -> 2 -> 3 -> 4 -> A
FIRST_NODE_MAC = "00:12:6D:00:00:{:02x}".format(2)
MACS = [FIRST_NODE_MAC]
MACS += ["00:12:6D:00:00:{:02x}".format(i) for i in range(3, 8)]
MACS.append(FIRST_NODE_MAC)
PACKET_WORDS = 200
START_CYCLE = 0
MNIST_MAX_TESTING_IMAGES = int(sys.argv[1])

def main():
    if not os.path.isdir("testbuild"):
        os.makedirs("testbuild")

    compile("crt.S", "testbuild/crt.o", {})
    compile("syscalls.c", "testbuild/syscalls.o", {})

    compile(
        "firstnode.c",
        "testbuild/firstnode.o",
        {"PREV_MACADDR": mac_to_hex(MACS[5]),
         "NEXT_MACADDR": mac_to_hex(MACS[1]),
         "PACKET_WORDS": PACKET_WORDS,
         "START_CYCLE": ltoa(START_CYCLE),
         "MNIST_MAX_TESTING_IMAGES": MNIST_MAX_TESTING_IMAGES})
    link(["testbuild/firstnode.o", "testbuild/crt.o", "testbuild/syscalls.o"], "testbuild/firstnode.riscv")

    for i in range(1, 6):
        BASE = "testbuild/node{}".format(i - 1)
        compile(
            "main.c",
            BASE + ".o",
            {"START": 2 * (i - 1),
             "PREV_MACADDR": mac_to_hex(MACS[i - 1]),
             "NEXT_MACADDR": mac_to_hex(MACS[i + 1]),
             "PACKET_WORDS": PACKET_WORDS,
             "START_CYCLE": ltoa(START_CYCLE),
             "MNIST_MAX_TESTING_IMAGES": MNIST_MAX_TESTING_IMAGES})
        link([BASE + ".o", "testbuild/crt.o", "testbuild/syscalls.o"], BASE + ".riscv")

if __name__ == "__main__":
    main()
