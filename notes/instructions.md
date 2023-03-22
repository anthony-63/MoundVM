# Instruction set to MoundVM

## REMINDER: Everything is in hex

## Layout:
* Instruction (ex. `ED`)
    * Name: What it does (ex. Jump to address `AAAA` and push `BBBB` to stack)
    * Arguments (ex. `AA` `AA` `BB` `BB`)

* `00`
    * `hlt`: Halt
* `FF`
    * `nop`: No operation... does nothing.
* `01`
    * `mov8`: Move `AA` into register number `BB`
    * `AA` `BB`
* `02`
    * `mov16`: Move `AAAA` into register number `BB`
    * `AA` `AA` `BB`
* `03`
    * `mov32`: Move `AAAAAAAA` into register number `BB`
    * `AA` `AA` `AA` `AA` `BB`
* `04`
    * `ld8`: Move `AA` into bank `BB` and address `CCCC`
    * `AA` `BB` `CC` `CC` 
* `05`
    * `ld16`: Move `AAAA` into bank `BB` and address `CCCC`
    * `AA` `AA` `BB` `CC` `CC` 
* `06`
    * `ld32`: Move `AAAAAAAA` into bank `BB` and address `CCCC`
    * `AA` `AA` `AA` `AA` `BB` `CC` `CC` 
* `07`
    * `add`: Add register `AA` and `BB`,  and store the result into `AA`
    * `AA` `BB`
* `08`
    * `sub`: Subtract register `AA` and `BB`,  and store the result into `AA`
    * `AA` `BB`
* `09`
    * `mul`: Multiply register `AA` and `BB`,  and store the result into `AA`
    * `AA` `BB`
* `0A`
    * `jmp`: Branch to bank `AA` and address `BBBB`
    * `AA` `BB` `BB`
* `0B`
    * `jmpe`: Branch to bank `CC` and address `DDDD`  if register `AA` is equal to register `BB`
    * `AA` `BB` `CC` `DD` `DD` 
* `0C`
    * `jmpne`: Branch to bank `CC` and address `DDDD`  if register `AA` is not equal to register `BB`
    * `AA` `BB` `CC` `DD` `DD` 
* `0D`
    * `jmpl`: Branch to bank `CC` and address `DDDD`  if register `AA` is less than register `BB`
    * `AA` `BB` `CC` `DD` `DD` 
* `0E`
    * `jmpg`: Branch to bank `CC` and address `DDDD`  if register `AA` is greater than register `BB`
    * `AA` `BB` `CC` `DD` `DD` 
* `0F`
    * `push`: Push `AAAAAAAA` to stack
    * `AA` `AA` `AA` `AA`
* `10`
    * `pop`: Pop to register `AA`
    * `AA`
* `11`
    * `ldr`: Load register `AA` to bank `BB` and address `CCCC`
    * `AA` `BB` `CC` `CC`