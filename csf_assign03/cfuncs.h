// Feiyang Huang; fhuang15
// Tae Wan Kim; tkim104
#ifndef CFUNCS_H
#define CFUNCS_H

// check if an integer is a power of 2
bool isPowTwo(int n);

int logTwo (int n);

// while getline(cin, string)
string sl 
string addr

// assume perfect address
string >> sl;
string >> addr;

int address = stoi(addr, 0 , 16);
int offset = (address << (32 - logTwo(bytes_per_block))) >> (32 - logTwo(bytes_per_block));
int index = (address << (32 - logTwo(bytes_per_block) - logTwo(num_set))) >> (32 - logTwo(num_set));
int tag = address >> (logTwo(bytes_per_block) + logTwo(num_set));

// read UI input
// determine s/l --> 
// determine offset --> int
// determine index --> int 
// determines tag 

#endif