OUTVM = mound.exe
OUTASM = mvmasm.exe
SRCVM = vm/*.c vm/*/*.c
SRCASM = asm/mvmasm.py
FLAGS = -g

all: $(OUTVM) $(OUTASM)

$(OUTVM): $(SRCVM)
	gcc $(SRCVM) -o $(OUTVM) $(FLAGS)

$(OUTASM): $(SRCASM)
	python -m PyInstaller --onefile $(SRCASM)
	del *.spec
	copy .\dist\mvmasm.exe .\mvmasm.exe
	rmdir /s /q dist build

vm: $(OUTVM)
asm: $(OUTASM)

test: $(OUTVM) $(OUTASM)
	$(OUTASM) programs/test/test.mvmasm
	$(OUTVM) programs/test/test.mvm > programs/test/test.output
