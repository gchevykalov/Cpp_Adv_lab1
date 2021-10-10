#pragma once

#ifndef __ALLOCATOR_INTERFACE_H_INCLUDED
#define __ALLOCATOR_INTERFACE_H_INCLUDED

/**
 * @brief Minimal allocator interface
 *
 * Base class for allocators used in a deque
 */
class allocator_interface_t {
public:
  /**
   * A method that allocates a block of size 'size'
   * 
   * @param[in] size number of bytes to allocate
   * @return pointer to allocated memory
   */
  virtual void* alloc(size_t size) = 0;

  /**
   * Dealocation a block by pointer
   * 
   * @param[in] data pointer to block
   */
  virtual void dealloc(void* data) = 0;
};

#endif /* __ALLOCATOR_INTERFACE_H_INCLUDED */