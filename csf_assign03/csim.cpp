// Feiyang Huang; fhuang15
// Tae Wan Kim; tkim104
#include "cfuncs.h"
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <utility>
using namespace std;

int main (int argc, char* argv[]) {
  // parameter error checking
  if (argc != 7) {
    std::cerr << "Invalid number of input parameters.\n";
    return 1;
  }

  // number of sets in the cache (a positive power-of-2)
  // number of blocks in each set (a positive power-of-2)
  // number of bytes in each block (a positive power-of-2, at least 4)
  
  int num_set = stoi(argv[1]);
  int blocks_per_set = stoi(argv[2]);
  int bytes_per_block = stoi(argv[3]);
  
  if (num_set < 0 || !isPowTwo(num_set)) {
    std::cerr << "Invalid number of sets in the cache.\n";
    return 1;
  }
  if (blocks_per_set < 0 || !isPowTwo(blocks_per_set)) {
    std::cerr << "Invalid number blocks per set.\n";
    return 1;
  }
  if (bytes_per_block < 4 || !isPowTwo(bytes_per_block)) {
    std::cerr << "Invalid number of bytes per block.\n";
    return 1;
  }
  
  int write_alloc; // 1: write-allocate; 0: no-write-allocate
  int write_mode;  // 1: write-through; 0: write-back
  int evic;       // 1: lru; 0: fifo
  
  string write_miss = argv[4];
  if (write_miss.compare("write-allocate") == 0){
    write_alloc = 1;
  } else if (write_miss.compare("no-write-allocate") == 0) {
    write_alloc = 0;
  } else {
    std::cerr << "Invalid write-miss mode.\n";
    return 1;
  }
  
  string write_hit = argv[5];
  if (write_hit.compare("write-through") == 0){
    write_mode = 1;
  } else if (write_hit.compare("write-back") == 0) {
    write_mode = 0;
  } else {
    std::cerr << "Invalid write-hit mode.\n";
    return 1;
  }
  
  string eviction = argv[6];
  if (eviction.compare("lru") == 0){
    evic = 1;
  } else if (eviction.compare("fifo") == 0) {
    evic = 0;
    } else {
    std::cerr << "Invalid eviction mode.\n";
    return 1;
  }
  
  // Parameters to keep track of and print
  int total_loads = 0;
  int total_stores = 0;
  int load_hits = 0;
  int load_miss = 0;
  int store_hits = 0;
  int store_miss = 0;
  int total_cycles = 0;

  unsigned use_ctr = 0;

  // Block Struct
  struct block {
    unsigned tag;
    unsigned access_ts = 0;
    unsigned load_ts = 0;
    bool dirty = false;
    uint32_t lru_count;
  };

  // Set Struct
  struct Set {
    vector<block> set;
    uint32_t lifetime_counter = 0;
  };
  
  // Cache set up
<<<<<<< HEAD
  map<int, Set> cache;
=======
  map<unsigned, vector<block>> cache;
>>>>>>> cbc439ebaabb83475e6f1d815b458cc6c3978f32
  string trace_line;

  unsigned offset_size = logTwo(bytes_per_block);
  unsigned idx_size = logTwo(num_set);

  while (getline(cin, trace_line)){
    stringstream ss(trace_line);
    string action; 
    string addr;
    ss >> action;
    ss >> addr;
    unsigned address = stol(addr, 0 , 16);
    // int offset = (address << (32 - offset_size)) >> (32 - offset_size);
    unsigned index = (address << (32 - offset_size - idx_size)) >> (32 - idx_size);
    unsigned tag = address >> (offset_size + idx_size);

    // Create the Block struct
    block new_block;
    new_block.tag = tag;

    // If loading
    if (action.compare("l") == 0) {
      total_loads++;
      // If there is no set existent yet
      if (cache.find(index) == cache.end()) {
<<<<<<< HEAD
        cache.insert(pair<int, Set> (index, Set() ));
	new_block.lru_count = cache.at(index).lifetime_counter;
	cache.at(index).lifetime_counter++;
        cache.at(index).set.push_back(new_block);
=======
        cache.insert(pair<unsigned, vector<block>> (index, vector<block>() ));
        cache.at(index).push_back(new_block);
>>>>>>> cbc439ebaabb83475e6f1d815b458cc6c3978f32
        load_miss++;
        total_cycles += 1 + (100 * (bytes_per_block / 4));
      }

      // If the set exists
      else {
        bool hit = false;
        for (vector<block>::iterator it = cache.at(index).set.begin(); it != cache.at(index).set.end(); ++it) {
          if ((*it).tag == new_block.tag) {
            hit = true;
<<<<<<< HEAD
            (*it).num_accesses++;
	    (*it).lru_count = cache.at(index).lifetime_counter;
	    cache.at(index).lifetime_counter++;
=======
            (*it).access_ts = ++use_ctr;
            break;
>>>>>>> cbc439ebaabb83475e6f1d815b458cc6c3978f32
          }
	}

        // If the block exists
        if (hit) {
          load_hits++;
          total_cycles++;
        }

        // If the block does not exist, space
<<<<<<< HEAD
        else if ((int) cache.at(index).set.size() < blocks_per_set) {
	  new_block.lru_count = cache.at(index).lifetime_counter;
	  cache.at(index).lifetime_counter++;
	  cache.at(index).set.push_back(new_block);
	  load_miss++;
	  total_cycles += 1 + (100 * (bytes_per_block / 4));
=======
        else if (cache.at(index).size() < (unsigned) blocks_per_set) {
            cache.at(index).push_back(new_block);
            load_miss++;
            total_cycles += 1 + (100 * (bytes_per_block / 4));
>>>>>>> cbc439ebaabb83475e6f1d815b458cc6c3978f32
        }
        // If the block does not exist,  no space
        else {
          // LRU eviction
          if (evic == 1) {
<<<<<<< HEAD
            int block_index_low_acc = 0;
            int count = 0;
	    uint32_t lowest_lru_count = cache.at(index).set.at(0).lru_count;
            for (vector<block>::iterator it = cache[index].set.begin(); it != cache[index].set.end(); ++ it) {
	      if ((*it).lru_count < lowest_lru_count) {
		block_index_low_acc = count;
		lowest_lru_count = (*it).lru_count;
	      }
=======
            unsigned least_recent = cache.at(index).at(0).access_ts;
            unsigned block_index_low_acc = 0;
            unsigned count = 0;
            for (vector<block>::iterator it = cache[index].begin(); it != cache[index].end(); ++ it) {
              if ((*it).access_ts < least_recent) {
                least_recent = (*it).access_ts;
                block_index_low_acc = count;
              }
>>>>>>> cbc439ebaabb83475e6f1d815b458cc6c3978f32
              count++;
            }
            if (cache.at(index).set.at(block_index_low_acc).dirty == true) {
              total_cycles += (100 * (bytes_per_block) / 4);
            }
	    new_block.lru_count = cache.at(index).lifetime_counter;
	    cache.at(index).lifetime_counter++;
            cache.at(index).set.at(block_index_low_acc) = new_block;
            load_miss++;
            total_cycles += 1 + (100 * (bytes_per_block / 4));
          }
          // TODO: FIFO eviction
        }
      }
	  }

    // store
    else if (action.compare("s") == 0) {
      total_stores++;
      // If there is no set existent yet
      if (cache.find(index) == cache.end()) {
<<<<<<< HEAD
        cache.insert(pair<int, Set> (index, Set() ));
	store_miss++;
=======
        cache.insert(pair<unsigned, vector<block>> (index, vector<block>() ));
	      store_miss++;
>>>>>>> cbc439ebaabb83475e6f1d815b458cc6c3978f32
        if (write_alloc == 1 && write_mode == 1) {
	  new_block.lru_count = cache.at(index).lifetime_counter;
	  cache.at(index).lifetime_counter++;
          cache.at(index).set.push_back(new_block);
          total_cycles += 1 + (100 * (bytes_per_block / 4));
        }
        else if (write_alloc == 0 && write_mode == 1) {
          total_cycles += 100 * (bytes_per_block / 4);
        }
        else {
          new_block.dirty = true;
	  new_block.lru_count = cache.at(index).lifetime_counter;
	  cache.at(index).lifetime_counter++;
          cache.at(index).set.push_back(new_block);
          total_cycles++;
        }
      }
      // If the set exists
      else if (cache.find(index) != cache.end()) {
        bool hit = false;
        for (vector<block>::iterator it = cache.at(index).set.begin(); it != cache.at(index).set.end(); ++it) {
          if ((*it).tag == new_block.tag) {
            hit = true;
<<<<<<< HEAD
            (*it).num_accesses++;
	    (*it).lru_count = cache.at(index).lifetime_counter;
	    cache.at(index).lifetime_counter++;
=======
            (*it).access_ts = ++use_ctr;
            break;
>>>>>>> cbc439ebaabb83475e6f1d815b458cc6c3978f32
          }
	      }
        // If the block exists
        if (hit) {
          store_hits++;
          total_cycles++;
        }
        // If the block does not exist, there is space
<<<<<<< HEAD
        else if ((int) cache.at(index).set.size() < blocks_per_set) {
=======
        else if ( cache.at(index).size() < (unsigned) blocks_per_set) {
>>>>>>> cbc439ebaabb83475e6f1d815b458cc6c3978f32
          store_miss++;
          if (write_alloc == 1 && write_mode == 1) {
	    new_block.lru_count = cache.at(index).lifetime_counter;
	    cache.at(index).lifetime_counter++;
            cache.at(index).set.push_back(new_block);
            total_cycles += 1 + (100 * (bytes_per_block / 4));
          }
          else if (write_alloc == 0 && write_mode == 1) {
            total_cycles += 100 * (bytes_per_block / 4);
          }
          else {
            new_block.dirty = true;
	    new_block.lru_count = cache.at(index).lifetime_counter;
	    cache.at(index).lifetime_counter++;
            cache.at(index).set.push_back(new_block);
            total_cycles++;
          }
        }
        // If the block does not exist, there is no space
        else {
          // LRU eviction
          if (evic == 1) {
<<<<<<< HEAD
            int block_index_low_acc = 0;
            int count = 0;
	    uint32_t lowest_lru_count = cache.at(index).set.at(0).lru_count;
            for (vector<block>::iterator it = cache[index].set.begin(); it != cache[index].set.end(); ++ it) {
	      if ((*it).lru_count < lowest_lru_count) {
		block_index_low_acc = count;
		lowest_lru_count = (*it).lru_count;
	      }
=======
            unsigned least_recent = cache.at(index).at(0).access_ts;
            unsigned block_index_low_acc = 0;
            unsigned count = 0;
            for (vector<block>::iterator it = cache[index].begin(); it != cache[index].end(); ++ it) {
              if ((*it).access_ts < least_recent) {
                least_recent = (*it).access_ts;
                block_index_low_acc = count;
              }
>>>>>>> cbc439ebaabb83475e6f1d815b458cc6c3978f32
              count++;
            }
            if (cache.at(index).set.at(block_index_low_acc).dirty == true) {
              total_cycles += 100 * (bytes_per_block / 4);
            }
            store_miss++;
            if (write_alloc == 1 && write_mode == 1) {
	      new_block.lru_count = cache.at(index).lifetime_counter;
	      cache.at(index).lifetime_counter++;
              cache.at(index).set.at(block_index_low_acc) = new_block;
              total_cycles += 1 + (100 * (bytes_per_block / 4));
            }
            else if (write_alloc == 0 && write_mode == 1) {
              total_cycles += 100 * (bytes_per_block / 4);
            }
            else {
              new_block.dirty = true;
	      new_block.lru_count = cache.at(index).lifetime_counter;
	      cache.at(index).lifetime_counter++;
              cache.at(index).set.at(block_index_low_acc) = new_block;
              total_cycles++;
            }
          }
          // TODO: The case for FIFO
          else {
          }
	      }
      }
    }

    // TODO: Was not given a valid block  
  }
  // TODO: Did not read 'l' or 's'


  // Print out parameter values asked for
  cout << "Total loads: " << total_loads << endl;
  cout << "Total stores: " << total_stores << endl;
  cout << "Load hits: " << load_hits << endl;
  cout << "Load misses: " << load_miss << endl;
  cout << "Store hits: " << store_hits << endl;
  cout << "Store misses: " << store_miss << endl;
  cout << "Total cycles: " << total_cycles << endl;
  
  return 0;
}
