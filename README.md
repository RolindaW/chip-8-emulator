# CHIP-8 Emulator

CHIP-8 is an interpreted programming language (and, therefore, an interpreter of it) originally designed by Joseph Weisbecker to develop programs (in this case, video games) for the COSMAC VIP microcomputer (which mounted an RCA 1802 microprocessor, also known as COSMAC).

The programming language is used to write the programs (erroneously also called ROM) that the interpreter will later execute. These programs were commonly stored on tapes (which were later loaded into the microcomputer's RAM) and, in the case of the COSMAC VIP microcomputer, could also be encoded directly (then immediately loaded into RAM).

The interpreter (also called virtual machine) is a specialized program that executes programs coded in the CHIP-8 programming language. In the original microcomputers, the interpreter had to be coded for the instruction set of the corresponding microprocessor and was stored in ROM memory and later loaded into RAM memory to be executed by the microprocessor.

Warning!
It is important to understand the role of the CHIP-8 interpreter, which consists of translating the instructions written in the CHIP-8 programming language into instructions that can be processed by the corresponding microprocessor. Thus, an instruction in the CHIP-8 programming language could have as many interpretations as there are different instruction sets for the different microprocessors on which it can be executed. This, together with the fact that different microprocessors (and their corresponding microcomputers) have different architectures, instruction execution times are not unique (although an average value per instruction could be estimated to some extent).

---

History

Extensions:
-CHIP-10 and Hi-Res CHIP-8: higher resolution.
-CHIP-8C and CHIP-8X: (limited) color.

Improved versions:
-CHIP-48 (1990; by Andreas Gustafsson).
-SUPER-CHIP (a.k.a. SCHIP / S-CHIP; 1990; by Erik Bryntse).

Outstanding partners:
-David Winter: interpreter, disassembler, and extended technical documentation.

---

Hardware (Host)

-COSMAC VIP (original)
Microcomputer
RCA - Joseph Weisbecker
1977, USA
Microprocessor: RCA 1802 (a.k.a. COSMAC), 8-bit word.

-Telmac 1800
Microcomputer
Telercas Oy
1977, Finland
Microprocessor: RCA 1802.

-DREAM 6800
Hobby computer
Michael Bauer
1978, Australia
Microprocessor: Motorola (MC)6800, 8-bit word.

---

Specifications

We are really talking about the requirements of the interpreter (although it is certainly very much conditioned by the original hardware for which it was designed).
The performance of the microcomputers could be superior, although in this type of dedicated systems (unless general purpose) it would not make any sense (i.e. they would be wasted).

---

Memory

RAM

DMA (Direct Memory Access)

4 kB
4096 B (`4 kB == 4 * 2**10 B == 4 * 1024 B == 4096 B` or `4 kB == 4 * 2**10 B == 2**2 * 2**10 B == 2**12 B == 4096 B`) (0x1000 B - HEX notation).
Memory location range: 0x0000 (0) - 0x0FFF (4095)

First 512 B reserved for the CHIP-8 interpreter.
512 B == 0x0200 B
Memory location range: 0x0000 (0) - 0x01FF (511)
Note: in modern emulators font data is commonly stored here in the range 0x0050 (80) - 0x009F (159) i.e. 0x0050 (80) B

Program start on memory location 0x0200 (512).
Do not access any memory under this location.
Memory location range: 0x0200 (512) - 0x0E9F (3743)
Available space: 0x0CA0 B == 3232 B

Uppermost 256 B reserved for display refresh.
256 B == 0x00FF B
Memory location range: 0x0F00 (3840) - 0x0FFF (4095)
Why 256 B? Because MONOCHROME (true/false value per px) display resolution is 64x32 px i.e. 2048 px, which can be tightly represented with 256 B (8 px encoded per byte)
Warning! It is not a good idea to implement it this way; we will use a separate array of 64x32 - 2048 - elements.

Below 96 B reserved for the call stack, internal use and other variables.
96 B == 0x0060 B
Memory location range: 0x0EA0 (3744) - 0x0EFF (3839)
Warning! It is not a good idea to implement it this way; we will use a separate array of n (number of maximum nesting levels) elements.

Summary:

```
0x0000 (0)    - 0x0FFF (4095) : 0x1000 (4096) B : Total
0x0000 (0)    - 0x01FF (511)  : 0x0200 (512) B  : Interpreter (+ Fonts)
0x0200 (512)  - 0x0E9F (3743) : 0x0CA0 (3232) B : Program
0x0EA0 (3744) - 0x0EFF (3839) : 0x0060 (96) B   : Others
0x0F00 (3840) - 0x0FFF (4095) : 0x00FF (256) B  : Display
```

ROM

CHIP-8 programs can and do modify themselves (unlike the conveniently so-called ROM cartridges).

---

Registers

GP (General Purpose) register: x16 (V0 - VF), 8-bit (1 B).
VF special use as flag register (better avoid using it): [Warning! I have not seen any CHIP-8 interpreter instructions that check or make use of the value of this register when it is set as a flag. Is it really necessary to implement in the cases that indicates this functionality? I guess that who could use this flag would be the processor, but this part is not included in this interpreter - that would be a matter of cmoplete console emulators (processor included) that use the CHIP-8 interpreter]
-Add operation: carry flag.
-Subtract operation: no-borrow flag.
-Drawing operation: pixel collision flag.

Address (or index) register (to point at memory locations): x1 (I), 12-bit.
12-bit because the memory size is 4KB (i.e. 4096 B or, in other words, `2**12` B).
Actually, when running on a microprocessor with 8-bit (1 B) words, the register will use 2 words (i.e. a total of 16-bit - 2 B), so it will be partially wasted.

---

Stack

Store return addresses (i.e. PC value) when subroutines are called.
LIFO (Last In First Out)
Size: 48 B
x12 levels of nesting - Warning! Modern implementations may have more (e.g. x16 or "unlimited"); do not need to worry actually, as existing programs were created for old hardware - which was limited - and not for modern emulators - which can be much more rich in available resources. [it can be implemented simply like an array of n(=16?) fixed number of levels]
Each level 16-bit long (i.e. 2 B) - Warning! Would really only be using the 12 least significant bits i.e. 12-bit.

SP (Stack Pointer): 8-bit (1 B) - Point to the topmost level of the stack. [it can be implemented simply like an index - 1 B - for the array - Warning! Must be big enough to index the array]

Warning! Stack values do not need to be removed on poping (because it will be overwritten by another process soon; not in this case, but in actual CPU)

---

Timers

x2:
-Delay timer (DT): timing events; value can be set (write) and get (read).
-Sound timer (ST): sound effects (beep when value is nonzero); value can only be set. [produced sound has only one tone, whose frequency is decided by the author of the interpreter]
Size: 8-bit (1 B)
Frequency: count down (decremented by 1) at 60 Hz (cycles/second) until reach 0 - Warning! Independent of the speed of the CPU (fetch/decode/execute) loop, that can (or not; watch out) be controlled.

---

Input

HEX Keyboard: 16 keys (0 - F).

```
1 2 3 C
4 5 6 D
7 8 9 E
A 0 B F
```

Commonly replaced with standard QERTY keyboard layout:

```
1 2 3 4
Q W E R
A S D F
Z X C V
```

---

Graphics

Display:
-64x32 (width-height) px (pixel).
-Monochrome (black or white).
-Originally updated (by the interpreter) at 60Hz (i.e. 60 FPS) - Warning! In the emulator it would be more convenient (to maximize efficiency) to limit the display update to when a drawing command is executed.

```
(0,0)    ...    (63,0)
  .               .
(0,31)   ...   (63,31)
```

Only by drawing...
-XOR with corresponding screen px (i.e. set sprite px flip correspoding screen px; unset sprite px do nothing).
-Collision detection: VF is set to 1 if flipped px; otherwise, 0.
...sprites:
(Size: 1-15 B)
-Wide: 8 px (i.e. each of the bits of corresponding byte).
-Height: 1-15 px (corresponding bytes).

Warning! Ojo cuidado con intercambiar los ejes (X,Y)...
...del display original (para el cual estan disenados los sprites; +Y es top to bottom) y...
...los de una textura 2D en OpenGL (+Y es bottom to top).
El eje +X tiene el mismo sentido en ambos casos: left to right.
Si los calculos en memoria lo haces igual que luego lo pintas en la textura, el resultado va a estar invertido en el eje Y (o, lo que es lo mismi, mirrored en el eje X)

Font:
Built-in
Characters: x16 (0-F; hexadecimal notation); sprites, each 5 B in 4x5 px format - Warning! Actually represented as 8x5 i.e. 1 B per row.

```
0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
0x20, 0x60, 0x20, 0x20, 0x70, // 1
0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
0x90, 0x90, 0xF0, 0x10, 0x10, // 4
0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
0xF0, 0x10, 0x20, 0x40, 0x40, // 7
0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
0xF0, 0x90, 0xF0, 0x90, 0x90, // A
0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
0xF0, 0x80, 0x80, 0x80, 0xF0, // C
0xE0, 0x90, 0x90, 0x90, 0xE0, // D
0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
0xF0, 0x80, 0xF0, 0x80, 0x80  // F
```

```
11110000    00100000            11110000
10010000    10100000            10000000
10010000    00100000     ...    11110000
10010000    00100000            10000000
11110000    01110000            10000000
```

---

Sound

Beep when the value of the sound timer is nonzero

---

Opcode

PC (Program Counter): to point at next instruction memory location. [in the emulator, RAM memory will be represented with an array; as in real hardware (same concept but on much larger scale), memory locations (i.e. addresses) will correspond to the positions (or the indices) within the array]
Timing: 700 CHIP-8 instructions per second works well for most old programs.

x35
Size: 2 B
Endianness: BE (big-endian) i.e. Memory arrangement: ..., B-n (0xAB), B-n+1 (0xCD), ... / Decode (2 B): 0xB-nB-n+1 (0xABCD)
Processed by nibbles (low nibble 1st): N1N2N3N4 == ABCD (where B-n (0xAB), B-n+1 (0xCB)) - Warning! It is recommended to perform full instruction decoding out of each instruction code (or function) so in case there is any mistake there is just a single place where it should be fixed.
-N1: instruction type
(N2,N3 and N4: meaning depends on instruction type)
-N2: X (register identifier - VX)
-N3: Y (register identifier - VY)
-N4: N (4-bit constant)
-N3N4: NN (8-bit constant)
-N2N3N4: NNN (12-bit memory address)

Table
-0NNN: call machine code routine at address NNN; not necessary for most programs.
-00E0: clear screen. [turn all px to 0]
-00EE: return form subroutine. [pop last address from the stack; the set PC to it]
-1NNN: jump to address NNN. [set PC to NNN]
-2NNN: call subroutine at address NNN. [push current PC into stack; then set PC to NNN]
-3XNN: skip next instruction if VX equals NN - Warning! These type of instructions act like an opposite "if" statement: if the condition is met, then related code is skipped - skip next instruction (that usually is a jump instruction to corresponding code); otherwise, code is executed - execute next instruction. [increment PC by 2 if condition is met]
-4XNN: skip next instruction if VX not equals NN. [increment PC by 2 if condition is met]
-5XY0: skip next instruction if VX equals VY. [increment PC by 2 if condition is met]
-6XNN: set VX to constant NN
-7XNN: add constant NN to VX (VF - carry flag - is not affected)
-8XY0: set VX to VY
-8XY1: set VX to (VX OR VY). [VY is not affected]
-8XY2: set VX to (VX AND VY). [VY is not affected]
-8XY3: set VX to (VX XOR VY). [VY is not affected]
-8XY4: add VY to VX (set VF to 1 if overflow; otherwise, 0). [VY is not affected]
-8XY5: set VX to (VX - VY) (set VF to 0 if underflow; otherwise 1). [VY is not affected]
-8XY6: right-shift VX by 1 (store in VF previous LSB from VX). [alternative behaviour: first set VX to VY; then, proceed normally - Warning! Implement both solutions and make it configurable]
-8XY7: set VX to (VY - VX) (set VF to 0 if underflow; otherwise 1). [VY is not affected]
-8XYE: left-shift VX by 1 (store in VF previous MSB from VX). [alternative behaviour: first set VX to VY; then, proceed normally - Warning! Implement both solutions and make it configurable]
-9XY0: skip next instruction if VX not equals VY. [increment PC by 2 if condition is met]
-ANNN: set I to address NNN
-BNNN (TODO): jump to address NNN plus V0. [alternative behaviour BXNN: jump to address XNN plus VX - Warning! Implement both solutions and make it configurable]
-CXNN (TODO): set VX to (rand(8-bit) AND constant NN)
-DXYN: draw sprite at coordinate (VX, VY) with a size of 8xN px. Start reading each B (up to N) at address I (I is not affected). Set VF to 1 if px flipping; otherwise, 0. [Tip: starting drawing position must be wrapped - MOD by corresponding dimension (i.e. screen size) to get correct value; sprite drawing must be clipped against the border] [alternative behaviour: sprite drawing must be wrapped - and not clipped - against the border - Warning! Implement both solutions and make it configurable]
-EX9E (TODO): skip next instruction if VX is pressed key - Warning! Do not await
-EXA1 (TODO): skip next instruction if VX is not pressed key - Warning! Do not await
-FX07 (TODO): set VX to delay timer value
-FX0A (TODO): await to key press (bloking operation - Warning! Depending on implementation, PC may be decremented 2 B so current instruction keep being execued; timers should keep decremeting if active); then, store in VX. [alternative behaviour: await to key press & release - Warning! Implement both solutions and make it configurable]
-FX15 (TODO): set delay timer to VX
-FX18 (TODO): set sound timer to VX
-FX1E (TODO): add VX to I (VF is not affected). [alternative behaviour: set VF to 1 if 12-bit overflow; otherwise, 0 - Warning! Implement both solutions and make it configurable]
-FX29 (TODO): set I to the memory location of the font sprite in VX. [use last - high - nibble from VX].
-FX33 (TODO): store BCD (Binary-Coded Decimal) of VX - Warning! Convert to decimal first: hundreds at memory location I, tens at I+1, and ones at I+2.
-FX55 (TODO): Store range V0-VX (included) starting at memory location I and offseting by 1 B for each value (I is not affected). [alternative behaviour: update I - Warning! Implement both solutions and make it configurable]
-FX65 (TODO): Fill range V0-VX (included) starting at memory location I and offseting by 1 B for each value (I is not affected). [alternative behaviour: update I - Warning! Implement both solutions and make it configurable]

---

Endianess - Example - IBM Logo

Memory: 00 E0 A2 2A 60 0C 61 08 D0 1F 70 09 A2 39 D0 1F

(BE) > Does make sense
00E0 - CLEAR SCREEN
A22A - SET I TO 22A
600C - SET V0 TO 0C
6108 - SET V1 TO 08
D01F - DRAW AT (V0,V1) SPRITE 8xF (START AT I)
7009 - ADD 09 TO V0
A239 - SET I TO 239
D01F - DRAW AT (V0,V1) SPRITE 8xF (START AT I)
...

---

Reference

-Tobias V. Langhoff (https://tobiasvl.github.io/blog/write-a-chip-8-emulator/)
-Austin Morlan (https://austinmorlan.com/posts/chip8_emulator/)
-David Winter (https://www.pong-story.com/chip8/)
-Cowgod (http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
-Jackson Sommerich (https://jackson-s.me/2019/07/13/Chip-8-Instruction-Scheduling-and-Frequency.html)
-Wikipedia (https://es.wikipedia.org/wiki/CHIP-8)
