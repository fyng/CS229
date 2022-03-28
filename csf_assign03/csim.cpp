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

  // Declare Cache instance
  Cache* cache = new Cache(num_set, blocks_per_set);
  cache->param->num_set = num_set;
  cache->param->blocks_per_set = blocks_per_set;
  cache->param->bytes_per_block = bytes_per_block;
  
  string write_miss = argv[4];
  if (write_miss.compare("write-allocate") == 0){
    cache->param->write_alloc = true;
  } else if (write_miss.compare("no-write-allocate") == 0) {
    cache->param->write_alloc = false;
  } else {
    std::cerr << "Invalid write-miss mode.\n";
    return 1;
  }
  string write_hit = argv[5];
  if (write_hit.compare("write-through") == 0){
    cache->param->write_through = true;
  } else if (write_hit.compare("write-back") == 0) {
    cache->param->write_back = true;
  } else {
    std::cerr << "Invalid write-hit mode.\n";
    return 1;
  }
  string eviction = argv[6];
  if (eviction.compare("lru") == 0){
    cache->param->lru = true;
  } else if (eviction.compare("fifo") == 0) {
    cache->param->fifo = true;
    } else {
    std::cerr << "Invalid eviction mode.\n";
    return 1;
  }
  
  
  // read from input
  string trace_line;
  while (getline(cin, trace_line)){
    stringstream ss(trace_line);
    string action, addr;
    ss >> action;
    ss >> addr;
    uint32_t address, index, tag;
    address = stol(addr, 0 , 16);
    // uint32_t offset = (address << (32 - logTwo(bytes_per_block))) >> (32 - logTwo(bytes_per_block));
    index = (address << ((32 - logTwo(bytes_per_block) - logTwo(num_set))) >> (32 - logTwo(num_set)));
    tag = address >> (logTwo(bytes_per_block) + logTwo(num_set));

    // Create the Block struct

    // If loading
    if (action.compare("l") == 0) {
      cache->stats->total_loads++;
      cache->cur_ts++;

      bool hit = false;
      Block* empty_block = NULL;
      Block* min_access = &cache->sets.at(index).front();
      Block* min_load = &cache->sets.at(index).front();

      for(vector<Block>::iterator it = cache->sets.at(index).begin(); it != cache->sets.at(index).end(); ++it) {
        if (empty_block == NULL && !it->valid){
          empty_block = &(*it);
        }
        if (it->load_ts < min_load->load_ts){
          min_load = &(*it);
        }
        if (it->access_ts < min_access->access_ts){
          min_access = &(*it);
        }
        if (it->tag == tag && it->valid) {
          // load hit
          hit = true;
          cache->stats->load_hits++;
          it->access_ts = cache->cur_ts;
          cache->stats->total_cycles++;
          break;
        }
      }

      // load miss
      if (!hit) {
        cache->stats->load_miss++;
        cache->stats->total_cycles += 1 + (100 * (bytes_per_block / 4));

        // if there exist an invalid block
        if (empty_block != NULL){
          empty_block->load_ts = cache->cur_ts;
          empty_block->access_ts = cache->cur_ts;
          empty_block->valid = true;
          empty_block->tag = tag;
        } else {
          if (cache->param->fifo) {
            min_load->access_ts = cache->cur_ts;
            min_load->load_ts = cache->cur_ts;
            min_load->tag = tag;
            if (min_load->dirty){
              // dirty block eviction
              cache->stats->total_cycles += (100 * (bytes_per_block / 4));
              min_load->dirty = false;
            }
          }
          else if (cache->param->lru){
            min_access->access_ts = cache->cur_ts;
            min_access->load_ts = cache->cur_ts;
            min_access->tag = tag;
            // dirty block eviction
            if (min_access->dirty){
              cache->stats->total_cycles += (100 * (bytes_per_block / 4));
              min_access->dirty = false;
            }
          }
        }
      }
    }

    // store
    else if (action.compare("s") == 0) {
      cache->stats->total_stores++;
      cache->cur_ts++;
      bool hit = false;
      Block* empty_block = NULL;
      Block* min_access = &cache->sets.at(index).front();
      Block* min_load = &cache->sets.at(index).front();
      
      for(vector<Block>::iterator it = cache->sets.at(index).begin(); it != cache->sets.at(index).end(); ++it) {
        if (empty_block == NULL && !it->valid){
          empty_block = &(*it);
        }
        if (it->load_ts < min_load->load_ts){
          min_load = &(*it);
        }
        if (it->access_ts < min_access->access_ts){
          min_access = &(*it);
        }

        if (it->tag == tag && it->valid) {
          // hit
          hit = true;
          cache->stats->store_hits++;
          it->access_ts = cache->cur_ts;
          
          if (cache->param->write_through) {
            // write through
            cache->stats->total_cycles += 100;
          } else if (cache->param->write_back){
            // write back
            it->dirty = true;
            cache->stats->total_cycles++;
          }
          break;
        }
      }      

      if (!hit) {
        cache->stats->store_miss++;

        if (cache->param->write_alloc) {
          // write allocate: load block from memory into cache, update line in cache
          // if write through, write data to main memory
          cache->stats->total_cycles += 1 + (100 * (bytes_per_block / 4));
          
          if (cache->param->write_through) {
            // write through fee
            cache->stats->total_cycles += 100;
          }
        } else {
          // no write allocate: write straight to memory
          cache->stats->total_cycles += (100 * (bytes_per_block / 4));
        }
      }
    }
  }
  // Did not read 'l' or 's'

  // Print out parameter values asked for
  cout << "Total loads: " << cache->stats->total_loads << endl;
  cout << "Total stores: " << cache->stats->total_stores << endl;
  cout << "Load hits: " << cache->stats->load_hits << endl;
  cout << "Load misses: " << cache->stats->load_miss << endl;
  cout << "Store hits: " << cache->stats->store_hits << endl;
  cout << "Store misses: " << cache->stats->store_miss << endl;
  cout << "Total cycles: " << cache->stats->total_cycles << endl;
  
  delete cache;
  return 0;
};
