// Feiyang Huang; fhuang15
// Tae Wan Kim; tkim104
#include "cfuncs.h"
#include <iostream>
using namespace std;

Cache::Cache(int num_set, int blocks_per_set){
    sets = vector<vector<Block>> (num_set, vector<Block>(blocks_per_set));
    param = new CacheParam();
    stats = new Stats();
}

Cache::~Cache() {
    delete param;
    delete stats;
}

bool isPowTwo(int n){
    if(n == 0) {
        return false;
    }
    // a power of 2 has a single 1-bit
    if ((n & (~ (n-1))) == n) {
        return true;
    } 
    return false;
};

int logTwo(int n){
    int log = 0;
    while(n >>= 1){
        log++;
    }
    return log;
};

// Cache load_create_set_block(Cache cache, Block new_block, int index){
//     cache.insert(pair<int, Set> (index, Set() ));
//     new_block.access_ts = cache.cur_ts;
//     cache.cache.at(index).set.push_back(new_block);
//     cache.stats.load_miss++;

//     cache.stats.total_cycles += 1 + (100 * (bytes_per_block / 4));
// };

int parameterCheck(int argc, char* argv[]) {
  if (argc != 7) {
    std::cerr << "Invalid number of input parameters.\n";
    return 1;
  }
  if (stoi(argv[1]) < 0 || !isPowTwo(stoi(argv[1]))) {
    std::cerr << "Invalid number of sets in the cache.\n";
    return 1;
  }
  if (stoi(argv[2]) < 0 || !isPowTwo(stoi(argv[2]))) {
    std::cerr << "Invalid number of blocks per set.\n";
    return 1;
  }
  if (stoi(argv[3]) < 4 || !isPowTwo(stoi(argv[3]))) {
    std::cerr << "Invalid number of bytes per block.\n";
    return 1;
  }
  return 0;
}

int cacheSetUp(char* argv[], Cache* cache) {
  cache->param->num_set = stoi(argv[1]);
  cache->param->blocks_per_set = stoi(argv[2]);
  cache->param->bytes_per_block = stoi(argv[3]);

  string write_miss = argv[4];
  if (write_miss.compare("write-allocate") == 0) {
    cache->param->write_alloc = true;
  } else if (write_miss.compare("no-write-allocate") == 0) {
    cache->param->write_alloc = false;
  } else {
    std::cerr << "Invalid write-miss mode.\n";
    return 1;
  }

  string write_hit = argv[5];
  if (write_hit.compare("write-through") == 0) {
    cache->param->write_through = true;
  } else if (write_hit.compare("write-back") == 0) {
    cache->param->write_back = true;
  } else {
    std::cerr << "Invalid write-hit mode.\n";
    return 1;
  }

  string eviction = argv[6];
  if (eviction.compare("lru") == 0) {
    cache->param->lru = true;
  } else if (eviction.compare("fifo") == 0) {
    cache->param->fifo = true;
  } else {
    std::cerr << "Invalid eviction mode.\n";
    return 1;
  }

  // Case for no-write-allocare and write-back
  if (!cache->param->write_alloc && cache->param->write_back) {
    std::cerr << "Impossible write-miss mode and write-hit mode.\n";
    return 1;
  }
  return 0;
}

uint32_t getIndex(uint32_t address, int bytes_per_block, int num_set) {
  uint32_t index = address << (32 - logTwo(bytes_per_block) - logTwo(num_set));
  if (logTwo(num_set) == 0) {
    index = 0;
  } else {
    index >>= (32 - logTwo(num_set));
  }
  return index;
}

void load(Cache* cache, uint32_t index, uint32_t tag, int bytes_per_block) {
  cache->stats->total_loads++;
  cache->cur_ts++;

  bool hit = false;
  vector<Block>* target_set = &cache->sets.at(index);
  Block* min_access = &target_set->front();
  Block* min_load = &target_set->front();

  for(vector<Block>::iterator it = target_set->begin(); it != target_set->end(); ++it) {
    if (it->load_ts < min_load->load_ts) {
      min_load = &(*it);
    }
    if (it->access_ts < min_access->access_ts) {
      min_access = &(*it);
    }
    if (it->tag == tag && it->valid) {
      // Load Hit
      hit = true;
      cache->stats->load_hits++;
      it->access_ts = cache->cur_ts;
      cache->stats->total_cycles++;
      break;
    }
  }
  if (!hit) {
    cache->stats->load_miss++;
    cache->stats->total_cycles += 1 + (100 * (bytes_per_block / 4));
    miss(cache, tag, bytes_per_block, min_access, min_load);
  }
}

void miss(Cache* cache, uint32_t tag, int bytes_per_block, Block* min_access, Block* min_load) {
  // FIFO Eviction
  if (cache->param->fifo) {
    min_load->access_ts = cache->cur_ts;
    min_load->load_ts = cache->cur_ts;
    min_load->valid = true;
    min_load->tag = tag;
    if (min_load->dirty) {
      // Dirty Block Eviction
      cache->stats->total_cycles += (100 * (bytes_per_block / 4));
      min_load->dirty = false;
    }
  }
  // LRU Eviction
  else if (cache->param->lru) {
    min_access->access_ts = cache->cur_ts;
    min_access->load_ts = cache->cur_ts;
    min_access->tag = tag;
    // Dirty Block Eviction
    if (min_access->dirty) {
      cache->stats->total_cycles += (100 * (bytes_per_block / 4));
      min_access->dirty = false;
    }
  }
}

void store(Cache* cache, uint32_t index, uint32_t tag, int bytes_per_block) {
  cache->stats->total_stores++;
  cache->cur_ts++;

  bool hit = false;
  vector<Block>* target_set = &cache->sets.at(index);
  Block* min_access = &target_set->front();
  Block* min_load = &target_set->front();

  for(vector<Block>::iterator it = target_set->begin(); it != target_set->end(); ++it) {
    if (it->load_ts < min_load->load_ts) {
      min_load = &(*it);
    }
    if (it->access_ts < min_access->access_ts) {
      min_access = &(*it);
    }
    if (it->tag == tag && it->valid) {
      // Hit
      hit = true;
      cache->stats->store_hits++;
      it->access_ts = cache->cur_ts;
      it->valid = true;
      if (cache->param->write_through) {
        // Write Through
        cache->stats->total_cycles += 100; // Double Check this value
      } else if (cache->param->write_back) {
        // Write Back
        it->valid = true;
        it->dirty = true;
        cache->stats->total_cycles++;
      }
      break;
    }
  }
  if (!hit) {
    cache->stats->store_miss++;
    if (cache->param->write_alloc) {
      // Write Allocate: Load block from memory into cache, update line cache
      // If Write Through, write data to main memory
      cache->stats->total_cycles += 1 + (100 * (bytes_per_block / 4));
      miss(cache, tag, bytes_per_block, min_access, min_load);
      if (cache->param->write_through) {
        // Write Through Fee
        cache->stats->total_cycles += 100; // Double Check Value
      }
    } else {
      // No Write Allocate
      cache->stats->total_cycles += (100 * (bytes_per_block / 4));
    }
  }
}

void printParameters(Cache* cache) {
  // Print out the parameter values asked for
  cout << "Total loads: " << cache->stats->total_loads << endl;
  cout << "Total stores: " << cache->stats->total_stores << endl;
  cout << "Load hits: " << cache->stats->load_hits << endl;
  cout << "Load misses: " << cache->stats->load_miss << endl;
  cout << "Store hits: " << cache->stats->store_hits << endl;
  cout << "Store misses: " << cache->stats->store_miss << endl;
  cout << "Total cycles: " << cache->stats->total_cycles << endl;
}

