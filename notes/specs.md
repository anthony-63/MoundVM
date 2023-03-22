# MoundVM Specifications

- 255 32-bit registers

- `0xFFFF` 32-bit spaces in the stack

- `0xFF` Banks of `0xFFFF` bytes of memory

## Memory Layout
### Adressing format: Bank Address (ex. `f0` `0001`)
`00`-`df` `0000`-`ffff`: General purpose memory to store things in

`e0`-`ef` `0000`-`ffff`: Video memory (WIP)

`f0`-`ff` `0000`-`ffff`: Program memory

## Function information
- Functions are called using an address in memory(can be in any place in memory in bank `f0`-`ff`)

- When calling a function push any arguments you want on the stack, as when calling functions you will be put on a new register-frame

### **Reminder. Register rfe is used to store the address to return to. Do NOT modify this register when your inside a function unless you know what your doing**