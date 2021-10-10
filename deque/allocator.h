#pragma once

#include <cstdlib>
#include "allocator_interface.h"

/**
 * @brief The simplest allocator
 *
 * The simplest allocator based on standard malloc/free functions
 */
class simple_allocator_t : public allocator_interface_t {
public:
  /**
   * A method that allocates a block of size 'size'
   *
   * @param[in] size number of bytes to allocate
   * @return pointer to allocated memory
   */
  void* alloc(size_t size) override {
    return malloc(size);
  }

  /**
   * Dealocation a block by pointer
   *
   * @param[in] data pointer to block
   */
  void dealloc(void* data) override {
    free(data);
  }
};