import sys
from sys import exit
from itertools import chain
import os
MVM_TYPE_KEYWORD = 0
MVM_TYPE_NUMBER = 1
MVM_TYPE_STRING = 2
MVM_TYPE_FUNCTION = 3
MVM_TYPE_REGISTER = 4
MVM_TYPE_OPEN_CURLY_BRACKET = 5
MVM_TYPE_CLOSE_CURLY_BRACKET = 6

BANK_OFFS = 0xf0
BYTES_PER_BANK = 0xffff
KEYWORD_INSTRUCTION_TABLE = {
    "mov": 0x03,
    "nop": 0xff,
    "ld8": 0x04,
    "ld16": 0x05,
    "ld32": 0x06,
    "add": 0x07,
    "sub": 0x08,
    "mul": 0x09,
    "jmp": 0x0a,
    "jmpre": 0x0b,
    "jmprne": 0x0c,
    "jmprl": 0x0d,
    "jmprg": 0x0e,
    "push": 0x0f,
    "pop": 0x10,
    "ldr": 0x11,
    "call": 0x12,
    "ret": 0x13,
    "movr": 0x14,
    "shl": 0x15,
    "shr": 0x16,
    "and": 0x17,
    "or": 0x18,
    "xor": 0x19,
    "subi": 0x20,
    "addi": 0x21,
    "jmpie": 0x22,
    "jmpine": 0x23,
    "jmpil": 0x24,
    "jmpig": 0x25,
}

class MvmCompiler:
    def __init__(self, src_file):
        self.src = ""
        with open(src_file, "r") as f:
            self.src = f.read()
        
        self.lables = {}
        self.src = self.src.replace("\n", " \n ").replace("\r", "")
        self.toksnoln = self.src.split(" ")

        while "" in self.toksnoln:
            self.toksnoln.remove("")

        self.toks = []
        cl = 1
        for i in self.toksnoln:
            if i == '\n': cl += 1
            else: self.toks.append((cl, i))
        self.mem = [[0] * (BYTES_PER_BANK + 1)] * 0x0f
        self.pc = 0
        self.pc_bank = 0xf0
        print(self.toks)
    def parse(self):
        for i in self.toks:
            if i[1] in KEYWORD_INSTRUCTION_TABLE:
                self.mem[self.pc_bank - BANK_OFFS][self.pc] = KEYWORD_INSTRUCTION_TABLE[i[1]]
            elif i[1].startswith("#"):
                n = i[1][1:]
                n = ("0" * (8 - len(n))) + n
                print(n)
                b = int(len(n) / 2)
                chunks = [n[f:f+2] for f in range(0, len(n), 2)]
                print(chunks)
                g = 0
                for a in chunks:
                    self.mem[self.pc_bank - BANK_OFFS][self.pc + g] = int(a, 16)
                    g += 1
                self.pc += g - 1
            elif "#" in i[1]:
                bits_in_number = int(i[1][0:i[1].index("#")])
                if bits_in_number == 32:
                    print("Suggestion: using '#' implicitly makes the literal 32-bit")
                if bits_in_number != 32 and bits_in_number != 16 and bits_in_number != 8:
                    print("Error: Invalid bit size ('%s') for literal provided on line %d" % (bits_in_number, i[0]))
                    os._exit(-1)
                n = i[1][i[1].index("#")+1:]
                n = ("0" * (int(bits_in_number / 4) - len(n))) + n
                print(n)
                b = int(len(n) / 2)
                chunks = [n[f:f+2] for f in range(0, len(n), 2)]
                print(chunks)
                g = 0
                for a in chunks:
                    self.mem[self.pc_bank - BANK_OFFS][self.pc + g] = int(a, 16)
                    g += 1
                self.pc += g - 1
            elif i[1].startswith("r"):
                n = i[1][1:]
                self.mem[self.pc_bank - BANK_OFFS][self.pc] = int(n, 16)
            elif i[1].startswith(";"):
                t = []
                for z in self.toks:
                    if z[0] != i[0]:
                        t.append(z)
                self.toks = t
            elif ":" in i[1]:
                self.lables[i[1][0:i[1].index(":")]] = (self.pc_bank, self.pc - 1)
                print("Creating label '" + str(i[1][0:i[1].index(":")]) + "' with info", (hex(self.pc_bank), hex(self.pc - 1)))
                continue
            elif i[1].startswith("$"):
                if not i[1][1:] in self.lables:
                    print(f"Error: Label '%s' does not exist on line %d" % (i[1][1:], i[0]))
                print(hex(self.pc_bank), hex(self.pc), hex(self.lables[i[1][1:]][0]), hex(self.lables[i[1][1:]][1]))
                self.mem[self.pc_bank - BANK_OFFS][self.pc] = self.lables[i[1][1:]][0]
                self.mem[self.pc_bank - BANK_OFFS][self.pc + 1] = (self.lables[i[1][1:]][1] & 0xff00) >> 8
                self.mem[self.pc_bank - BANK_OFFS][self.pc + 2] = self.lables[i[1][1:]][1] & 0xff
                self.pc += 2
            else:
                print(f"Error: Invalid keyword '%s' on line %d" % (i[1], i[0]))
                os._exit(-1)
            self.pc += 1
            if self.pc > BYTES_PER_BANK:
                self.pc = 0
                self.pc_bank += 1
    def output(self, path):
        with open(path, "wb") as f:
            self.pc += 1
            finished_addr = self.pc + ((self.pc_bank - BANK_OFFS) * BYTES_PER_BANK)
            fl = list(chain.from_iterable(self.mem))
            a = 0
            for i in range(0, finished_addr):
                if a % 16 == 0:
                    print("\n%08x: " % i, end = "")
                print("%02x" % fl[i], end=" ")
                a += 1
            print()
            
            f.write(bytes(fl[0:finished_addr]))
if len(sys.argv) != 3:
    print("Usage: \nmvmasm.exe <input mvmasm file> <output .mvm file>")
    exit(0)

compiler = MvmCompiler(sys.argv[1])
compiler.parse()
compiler.output(sys.argv[2])