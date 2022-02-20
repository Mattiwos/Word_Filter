# The Great Firewall of Santa Cruz:
## Bloom Filters, Linked Lists and Hash Tables
Description:
	For this assignment, I will be implementing a bloom filter. A bloom filter is a space-efficient probabilistic data structure, used to test whether an element is a member of a set and return its probability of it being in the set. The goal of the project is to catch and punish those who practice wrongthink and continue to use oldspeak,which is not allowed in the new regime. Thus, using a bloom filter I will exercise mercy and counsel the old speakers so that they atone and use newspeak, a replacement of the oldspeak words.
## How To Run Program
```
make
```
- Used to build all programs
### Banhammer
```
./banhammer [-flag]
```
- Used to filter input from stdin by using bloom filter and hash table.
```
  -h           Program usage and help.
  -s           Print program statistics.
  -m           Enable move-to-front rule.
  -t size      Specify hash table size (default: 10000).
  -f size      Specify Bloom filter size (default: 2^20).
```
- Available flags.
---
## How to use Makefile
```
make
**or**
make all
```
- Used to build Program
```
make clean
```
- Used to removes all files that compiler generated.
```
make format
```
- Format all source code, including the header files.
```
make scan-build
```
- Tests binary file for any missed errors
```
make [flag]
```
- Used to build specific files.
```
banhammer - filters input
```
- Flags used to only build specific programs.

