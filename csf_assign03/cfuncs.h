// Feiyang Huang; fhuang15
// Tae Wan Kim; tkim104
#ifndef CFUNCS_H
#define CFUNCS_H

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
}

// Block Struct
struct Block {
  uint32_t tag;
  bool valid = false;
  bool dirty = false;
  int num_accesses = 0;
  uint32_t access_ts = 0;
};

// Set Struct
struct Set {
  vector<Block> block;
  // uint32_t lifetime_counter = 0;
};

struct Cache {
  map <int, Set> sets;
  CacheParam param;
  unsigned cur_ts;
  Stats stats;
};

// check if an integer is a power of 2
bool isPowTwo(int n);

int logTwo (int n);

Cache load_create_set_block(Cache cache, Block new_block, int index);
Cache load_add_block(Cache cache, Block new_block, int index);


#endif