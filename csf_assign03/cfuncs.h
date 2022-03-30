// Feiyang Huang; fhuang15
// Tae Wan Kim; tkim104
#ifndef CFUNCS_H
#define CFUNCS_H
#include <cstdint>
#include <vector>

struct CacheParam {
  unsigned num_set = 0;
  unsigned blocks_per_set = 0;
  unsigned bytes_per_block = 0;
  bool write_alloc = false;
  bool write_through = false;
  bool write_back = false;
  bool lru = false;
  bool fifo = false;
};

// Parameters to keep track of and print
struct Stats {
  uint32_t total_loads = 0;
  uint32_t total_stores = 0;
  uint32_t load_hits = 0;
  uint32_t load_miss = 0;
  uint32_t store_hits = 0;
  uint32_t store_miss = 0;
  uint32_t total_cycles = 0;
};

// Block Struct
struct Block {
  uint32_t tag = 0;
  bool valid = false;
  bool dirty = false;
  uint32_t load_ts = 0; // FIFO
  uint32_t access_ts = 0; // LRU
};

// // Set Struct
// struct Set {
//   vector<Block> block;
//   // uint32_t lifetime_counter = 0;
// };

struct Cache {
  std::vector<std::vector<Block>> sets;
  CacheParam* param;
  unsigned cur_ts = 0;
  Stats* stats;
  Cache(int num_set, int blocks_per_set);
  ~Cache();
};

// check if an integer is a power of 2
bool isPowTwo(int n);

int logTwo (int n);

// Cache load_create_set_block(Cache cache, Block new_block, int index);
// Cache load_add_block(Cache cache, Block new_block, int index);

int parameterCheck(int argc, char* argv[]);

int cacheSetUp(char* argv[], Cache* cache);

uint32_t getIndex(uint32_t address, int bytes_per_block, int num_set);

void load(Cache* cache, uint32_t index, uint32_t tag, int bytes_per_block);

void miss(Cache* cache, uint32_t tag, int bytes_per_block, Block* empty_block, Block* min_access, Block* min_load);

void store(Cache* cache, uint32_t index, uint32_t tag, int bytes_per_block);

void printParameters(Cache* cache);

#endif
