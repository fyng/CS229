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
  
  param = new CacheParam;
  int num_set = stoi(argv[1]);
  int blocks_per_set = stoi(argv[2]);
  int bytes_per_block = stoi(argv[3]);
  
  if (num_set < 0 || !isPowTwo(num_set)) {
    std::cerr << "Invalid number of sets in the cache.\n";
    return 1;
  }
  param.num_set = num_set;

  if (blocks_per_set < 0 || !isPowTwo(blocks_per_set)) {
    std::cerr << "Invalid number blocks per set.\n";
    return 1;
  }
  param.blocks_per_set = blocks_per_set;

  if (bytes_per_block < 4 || !isPowTwo(bytes_per_block)) {
    std::cerr << "Invalid number of bytes per block.\n";
    return 1;
  }
  param.bytes_per_block = bytes_per_block;
  
  string write_miss = argv[4];
  if (write_miss.compare("write-allocate") == 0){
    param.write_alloc = true;
  } else if (write_miss.compare("no-write-allocate") == 0) {
    param.write_alloc = false;
  } else {
    std::cerr << "Invalid write-miss mode.\n";
    return 1;
  }
  string write_hit = argv[5];
  if (write_hit.compare("write-through") == 0){
    param.write_through = true;
  } else if (write_hit.compare("write-back") == 0) {
    param.write_back = true;
  } else {
    std::cerr << "Invalid write-hit mode.\n";
    return 1;
  }
  string eviction = argv[6];
  if (eviction.compare("lru") == 0){
    param.lru = true;
  } else if (eviction.compare("fifo") == 0) {
    param.fifo = true;
    } else {
    std::cerr << "Invalid eviction mode.\n";
    return 1;
  }
  
  // TODO: fix instantiation
  // Cache set 
  cache = new Cache;

  // read from input
  string trace_line;
  while (getline(cin, trace_line)){
    stringstream ss(trace_line);
    string action, addr;
    ss >> action;
    ss >> addr;
    uint32_t address = stol(addr, 0 , 16);
    // uint32_t offset = (address << (32 - logTwo(bytes_per_block))) >> (32 - logTwo(bytes_per_block));
    uint32_t index = (address << ((32 - logTwo(bytes_per_block) - logTwo(num_set))) >> (32 - logTwo(num_set)));
    uint32_t tag = address >> (logTwo(bytes_per_block) + logTwo(num_set));

    // Create the Block struct
    Block new_block;
    new_block.tag = tag;
    new_block.param = param;

    // If loading
    if (action.compare("l") == 0) {
      cache.stats.total_loads++;
      cache.cur_ts++;

      // Load miss: set does not exist
      if (cache.sets.find(index) == cache.sets.end()) {
        cache = create_set_block(cache, new_block, index);
      }

      else {
        bool hit = false;
        for (vector<block>::iterator it = cache.at(index).set.begin(); it != cache.at(index).set.end(); ++it) {
          if ((*it).tag == new_block.tag) {
            hit = true;
            // load hit
            (*it).num_accesses++;
            (*it).lru_count = cache.at(index).lifetime_counter;

            break;
          }
        }

        if (hit) {
            cache.stats.load_hits++;
            cache.sets.at(index).access_ts = cache.cur_ts;
            cache.stats.total_cycles++;
        }
        // Load miss, no eviction
        else if (cache.sets.at(index).set.size() < cache.param.blocks_per_set) {
          new_block.lru_count = cache.at(index).lifetime_counter;
          cache.at(index).lifetime_counter++;
          cache.at(index).set.push_back(new_block);
          load_miss++;
          total_cycles += 1 + (100 * (bytes_per_block / 4));
        }
        // If the block does not exist,  no space
        else {
          // LRU eviction
          if (evic == 1) {
            int block_index_low_acc = 0;
            int count = 0;
	          uint32_t lowest_lru_count = cache.at(index).set.at(0).lru_count;
            for (vector<block>::iterator it = cache[index].set.begin(); it != cache[index].set.end(); ++ it) {
              if ((*it).lru_count < lowest_lru_count) {
                block_index_low_acc = count;
                lowest_lru_count = (*it).lru_count;
              }
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
        cache.insert(pair<int, Set> (index, Set() ));
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
      // If the set exists
      else if (cache.find(index) != cache.end()) {
        bool hit = false;
        for (vector<block>::iterator it = cache.at(index).set.begin(); it != cache.at(index).set.end(); ++it) {
          if ((*it).tag == new_block.tag) {
            hit = true;
            (*it).num_accesses++;
	    (*it).lru_count = cache.at(index).lifetime_counter;
	    cache.at(index).lifetime_counter++;
          }
	      }
        // If the block exists
        if (hit) {
          store_hits++;
          total_cycles++;
        }
        // If the block does not exist, there is space
        else if ((int) cache.at(index).set.size() < blocks_per_set) {
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
            int block_index_low_acc = 0;
            int count = 0;
	    uint32_t lowest_lru_count = cache.at(index).set.at(0).lru_count;
            for (vector<block>::iterator it = cache[index].set.begin(); it != cache[index].set.end(); ++ it) {
	      if ((*it).lru_count < lowest_lru_count) {
		block_index_low_acc = count;
		lowest_lru_count = (*it).lru_count;
	      }
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
          // The case for FIFO
          else {
          }
	      }
      }
    }

    // Was not given a valid block  
  }
  
  // Did not read 'l' or 's'


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
