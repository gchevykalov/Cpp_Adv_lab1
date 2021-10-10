#pragma once

#include <iostream>
#include <exception>
#include "allocator.h"

using std::exception;

/**
 * @brief Deque class
 * @tparam elemType type of stored elements
 * @tparam memoryAllocator the allocator to be used (allocator based on malloc/free is used by default)
 * @warning for guaranteed compatibility, the allocator must be inherited from allocator_interface_t
 * @see allocator_interface_t
 */
template <typename elemType, typename memoryAllocator = simple_allocator_t>
class deque_t {
private:
  /**
   * @brief deque node struct
   * 
   * The structure of the node storing the value of the specified 'elemType' type
   */
  struct node_t {
    elemType value;           ///< stored value
    node_t* prev;             ///< pointer to previous element (nullptr if the node is the last one)
    node_t* next;             ///< pointer to next element (nullptr if the node is the first)
  };

  node_t* head;               ///< pointer to the beginning of the deque (nullptr if the deque is empty)
  node_t* tail;               ///< pointer to the end of the deque (nullptr if the deque is empty)
  unsigned int size;          ///< number of elements in the deque

  memoryAllocator allocator;  ///< the allocator to be used
public:
  /**
   * Friend operator<< for deque
   * @tparam type type of deque elements
   * @param[in] stream output stream
   * @param[in] deque deque to output
   * @return reference to stream
   */
  template <typename type>
  friend std::ostream& operator<<(std::ostream& stream, deque_t<type> const& deque);

  /**
   * @brief Deque iterator
   * 
   * Allows to iterate in direct order in deque
   */
  class iterator {
  private:
    node_t* curNode;   ///< Pointer to the deque node to which the iterator corresponds
  public:
    /**
     * Default constructor for iterator
     */
    iterator() : curNode(head) {}

    /**
     * Constructor from pointer to the deque node
     * @param[in] node pointer to the deque node we want to build iterator from
     */
    iterator(node_t* node) : curNode(node) {}

    /**
     * Default copy constructor
     * @param[in] iter iterator to copy
     */
    iterator(iterator const& iter) = default;

    /**
     * Default move constructor
     * @param[in] iter iterator to move
     */
    iterator(iterator&& iter) = default;

    /**
     * Default copy operator
     * @param[in] iter iterator to copy
     * @return reference to iterator
     */
    iterator& operator=(iterator const& iter) = default;

    /**
     * Default move operator
     * @param[in] iter iterator to move
     * @return reference to iterator
     */
    iterator& operator=(iterator&& iter) = default;

    /**
     * Prefix ++ operator
     * @return reference to next iterator
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    iterator& operator++() {
      if (curNode == nullptr)
        throw exception("Try to use end iterator");

      curNode = curNode->next;

      return *this;
    }

    /**
     * Postfix ++ operator
     * @return reference to current iterator
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    iterator operator++(int) {
      if (curNode == nullptr)
        throw exception("Try to use end iterator");

      iterator tmp = *this;

      curNode = curNode->next;

      return tmp;
    }

    /**
     * Comparison operator ==
     * @param[in] iter iterator we want to compare with
     * @return true if equals, false otherwise
     */
    bool operator==(iterator const& iter) noexcept {
      return curNode == iter.curNode;
    }

    /**
     * Comparison operator !=
     * @param[in] iter iterator we want to compare with
     * @return false if equals, true otherwise
     */
    bool operator!=(iterator const& iter) noexcept {
      return curNode != iter.curNode;
    }

    /**
     * Operator *
     * @return reference on value to which the iterator corresponds
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    elemType& operator*() {
      if (curNode == nullptr)
        throw exception("Try to use end iterator");

      return curNode->value;
    }
  };

  /**
   * Method begin for iterator
   * @return iterator that corresponds first element
   */
  iterator begin() {
    return iterator(head);
  }

  /**
   * Method end for iterator
   * @return iterator that corresponds element after last
   */
  iterator end() {
    return iterator(nullptr);
  }

  /**
   * @brief Deque const iterator
   *
   * Allows to iterate in direct order in deque (does not allow changing elements)
   */
  class const_iterator {
  private:
    const node_t* curNode;   ///< Pointer to the deque const node to which the iterator corresponds
  public:
    /**
     * Default constructor for const iterator
     */
    const_iterator() : curNode(head) {}

    /**
     * Constructor from pointer to the deque node
     * @param[in] node pointer to the deque node we want to build iterator from
     */
    const_iterator(const node_t* node) : curNode(node) {}

    /**
     * Default copy constructor
     * @param[in] iter iterator to copy
     */
    const_iterator(const_iterator const& iter) = default;

    /**
     * Default move constructor
     * @param[in] iter iterator to move
     */
    const_iterator(const_iterator&& iter) = default;

    /**
     * Default copy operator
     * @param[in] iter iterator to copy
     * @return reference to iterator
     */
    const_iterator& operator=(const_iterator const& iter) = default;

    /**
     * Default move operator
     * @param[in] iter iterator to move
     * @return reference to iterator
     */
    const_iterator& operator=(const_iterator&& iter) = default;

    /**
     * Prefix ++ operator
     * @return reference to next iterator
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    const_iterator& operator++() {
      if (curNode == nullptr)
        throw exception("Try to use end iterator");

      curNode = curNode->next;

      return *this;
    }

    /**
     * Postfix ++ operator
     * @return reference to current iterator
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    const_iterator operator++(int) {
      if (curNode == nullptr)
        throw exception("Try to use end iterator");

      const_iterator tmp = *this;

      curNode = curNode->next;

      return tmp;
    }

    /**
     * Comparison operator ==
     * @param[in] iter iterator we want to compare with
     * @return true if equals, false otherwise
     */
    bool operator==(const_iterator const& iter) noexcept {
      return curNode == iter.curNode;
    }

    /**
     * Comparison operator !=
     * @param[in] iter iterator we want to compare with
     * @return false if equals, true otherwise
     */
    bool operator!=(const_iterator const& iter) noexcept {
      return curNode != iter.curNode;
    }

    /**
     * Operator *
     * @return reference on value to which the iterator corresponds
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    const elemType& operator*() {
      if (curNode == nullptr)
        throw exception("Try to use end iterator");

      return curNode->value;
    }
  };

  /**
   * Method begin for iterator
   * @return iterator that corresponds first element
   */
  const_iterator begin() const {
    return const_iterator(head);
  }

  /**
   * Method end for iterator
   * @return iterator that corresponds element after last
   */
  const_iterator end() const {
    return const_iterator(nullptr);
  }

  /**
   * @brief Deque reverse iterator
   *
   * Allows to iterate in reverse order in deque
   */
  class reverse_iterator {
  private:
    node_t* curNode;   ///< Pointer to the deque node to which the iterator corresponds
  public:
    /**
     * Default constructor for reverse iterator
     */
    reverse_iterator() : curNode(tail) {}

    /**
     * Constructor from pointer to the deque node
     * @param[in] node pointer to the deque node we want to build iterator from
     */
    reverse_iterator(node_t* node) : curNode(node) {}

    /**
     * Default copy constructor
     * @param[in] iter iterator to copy
     */
    reverse_iterator(reverse_iterator const& iter) = default;

    /**
     * Default move constructor
     * @param[in] iter iterator to move
     */
    reverse_iterator(reverse_iterator&& iter) = default;

    /**
     * Default copy operator
     * @param[in] iter iterator to copy
     * @return reference to iterator
     */
    reverse_iterator& operator=(reverse_iterator const& iter) = default;

    /**
     * Default move operator
     * @param[in] iter iterator to move
     * @return reference to iterator
     */
    reverse_iterator& operator=(reverse_iterator&& iter) = default;

    /**
     * Prefix ++ operator
     * @return reference to next reverse iterator
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    reverse_iterator& operator++() {
      if (curNode == nullptr)
        throw exception("Try to use end iterator");

      curNode = curNode->prev;

      return *this;
    }

    /**
     * Postfix ++ operator
     * @return reference to current reverse iterator
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    reverse_iterator operator++(int) {
      if (curNode == nullptr)
        throw exception("Try to use end iterator");

      reverse_iterator tmp = *this;

      curNode = curNode->prev;

      return tmp;
    }

    /**
     * Comparison operator ==
     * @param[in] iter iterator we want to compare with
     * @return true if equals, false otherwise
     */
    bool operator==(reverse_iterator const& iter) noexcept {
      return curNode == iter.curNode;
    }

    /**
     * Comparison operator !=
     * @param[in] iter iterator we want to compare with
     * @return false if equals, true otherwise
     */
    bool operator!=(reverse_iterator const& iter) noexcept {
      return curNode != iter.curNode;
    }

    /**
     * Operator *
     * @return reference on value to which the iterator corresponds
     * @exception "Try to use end iterator" if curNode is equal to nullptr
     */
    elemType& operator*() {
      if (curNode == nullptr)
        throw exception("Try to use end iterator");

      return curNode->value;
    }
  };

  /**
   * Method begin for reverse iterator
   * @return iterator that corresponds last element
   */
  reverse_iterator rbegin() {
    return reverse_iterator(tail);
  }

  /**
   * Method end for reverse iterator
   * @return iterator that corresponds element before first
   */
  reverse_iterator rend() {
    return reverse_iterator(nullptr);
  }

  /**
   * Default deque constructor
   */
  deque_t() : head(nullptr), tail(nullptr), size(0) {};

  /**
   * Deque constructor with initializer list
   * @param[in] list list of 'elemType' values
   */
  deque_t(std::initializer_list<elemType> list) : head(nullptr), tail(nullptr), size(0) {
    for (auto& l : list)
      PushBack(l);
  };

  /**
   * Copy constructor
   * @param[in] deque const reference on deque to copy
   */
  deque_t(deque_t const& deque) : head(nullptr), tail(nullptr), size(0) {
    for (auto& d : deque)
      PushBack(d);
  };

  /**
   * Move constructor
   * @param[in] deque rvalue reference on deque to move
   */
  deque_t(deque_t&& deque) {
    head = deque.head;
    tail = deque.tail;
    size = deque.size;

    deque.head = nullptr;
    deque.tail = nullptr;
    deque.size = 0;
  };

  /**
   * Copy operator=
   * @param[in] deque const reference on deque to copy
   */
  void operator=(deque_t const& deque) {
    Clear();

    for (auto& d : deque)
      PushBack(d);
  }

  /**
   * Move operator=
   * @param[in] deque rvalue reference on deque to move
   */
  void operator=(deque_t&& deque) {
    Clear();
    head = deque.head;
    tail = deque.tail;
    size = deque.size;

    deque.head = nullptr;
    deque.tail = nullptr;
    deque.size = 0;
  }

  /**
   * Check is deque empty method
   * @return true if deque is empty, false otherwise
   */
  bool IsEmpty() const {
    return head == nullptr;
  }

  /**
   * Get deque size method
   * @return number of elements in the deque
   */
  unsigned int Size() const {
    return size;
  }

  /**
   * Method to see what first element is
   * @returns const reference on element
   * @exception "Deque is empty" if deque is empty
   */
  elemType const& PeekHead() const {
    if (!head)
      throw exception("Deque is empty");

    return head->value;
  }

  /**
   * Method to get first element
   * @returns reference on element
   * @exception "Deque is empty" if deque is empty
   */
  elemType& GetFront() {
    if (!head)
      throw exception("Deque is empty");

    return head->value;
  }

  /**
   * Method to see what last element is
   * @returns const reference on element
   * @exception "Deque is empty" if deque is empty
   */
  elemType const& PeekTail() const {
    if (!tail)
      throw exception("Deque is empty");

    return tail->value;
  }

  /**
   * Method to get last element
   * @returns reference on element
   * @exception "Deque is empty" if deque is empty
   */
  elemType& GetBack() {
    if (!tail)
      throw exception("Deque is empty");

    return tail->value;
  }

  /**
   * Method put element to begin of deque (copy semantics)
   * @param[in] elem const reference on element
   */
  void PushFront(elemType const& elem) {
    node_t* tmp = (node_t*)allocator.alloc(sizeof(node_t));

    tmp->value = elem;
    tmp->next = head;
    tmp->prev = nullptr;

    if (head)
      head->prev = tmp;

    head = tmp;

    if (tail == nullptr)
      tail = head;
    
    size++;
  }

  /**
   * Method put element to begin of deque (move semantics)
   * @param[in] elem rvalue reference on element
   */
  void PushFront(elemType&& elem) {
    node_t* tmp = (node_t*)allocator.alloc(sizeof(node_t));

    tmp->value = std::move(elem);
    tmp->next = head;
    tmp->prev = nullptr;

    if (head)
      head->prev = tmp;

    head = tmp;

    if (tail == nullptr)
      tail = head;

    size++;
  }

  /**
   * Method put element to end of deque (copy semantics)
   * @param[in] elem const reference on element
   */
  void PushBack(elemType const& elem) {
    node_t* tmp = (node_t*)allocator.alloc(sizeof(node_t));

    tmp->value = elem;
    tmp->next = nullptr;
    tmp->prev = tail;

    if (tail)
      tail->next = tmp;

    tail = tmp;

    if (head == nullptr)
      head = tail;

    size++;
  }

  /**
   * Method put element to end of deque (move semantics)
   * @param[in] elem rvalue reference on element
   */
  void PushBack(elemType&& elem) {
    node_t* tmp = (node_t*)allocator.alloc(sizeof(node_t));

    tmp->value = std::move(elem);
    tmp->next = nullptr;
    tmp->prev = tail;

    if (tail)
      tail->next = tmp;

    tail = tmp;

    if (head == nullptr)
      head = tail;

    size++;
  }

  /**
   * Method to remove first element from deque
   * @exception "Deque is empty" if deque is empty
   */
  void PopFront() {
    if (head == nullptr)
      throw exception("Deque is empty");

    node_t* tmp = head;

    head = head->next;
    allocator.dealloc((void*)tmp);

    if (head == nullptr)
      tail = nullptr;
    else
      head->prev = nullptr;

    size--;
  }

  /**
   * Method to remove last element from deque
   * @exception "Deque is empty" if deque is empty
   */
  void PopBack() {
    if (tail == nullptr)
      throw exception("Deque is empty");

    node_t* tmp = tail;

    tail = tail->prev;
    allocator.dealloc((void*)tmp);

    if (tail == nullptr)
      head = nullptr;
    else
      tail->next = nullptr;

    size--;
  }

  /**
   * Method for adding elements of another deque to the end of the current one (copy semantics)
   * @param[in] deque const reference on other deque
   * @returns refernece on current deque
   */
  deque_t& AddOtherDeque(deque_t const& deque) {
    for (auto& d : deque)
      PushBack(d);

    return *this;
  }

  /**
   * Method for adding elements of another deque to the end of the current one (move semantics)
   * @param[in] deque rvalue reference on other deque
   * @returns refernece on current deque
   */
  deque_t& AddOtherDeque(deque_t&& deque) {
    if (tail == nullptr) {
      head = deque.head;
      tail = deque.tail;
      size = deque.size;

      deque.head = nullptr;
      deque.tail = nullptr;
      deque.size = 0;
    }
    else {
      if (deque.head) {
        tail->next = deque.head;
        deque.head->prev = tail;
        tail = deque.tail;
        size += deque.size;

        deque.head = nullptr;
        deque.tail = nullptr;
        deque.size = 0;
      }
    }

    return *this;
  }

  /**
   * Clear deque
   */
  void Clear(void) {
    while (head)
      PopBack();
  }

  /**
   * Deque destructor
   */
  ~deque_t() {
    Clear();
  }
};

/**
 * Operator<< for deque
 * @tparam type type of deque elements
 * @param[in] stream output stream
 * @param[in] deque deque to output
 * @return reference to stream
 */
template <typename type>
std::ostream& operator<<(std::ostream& stream, deque_t<type> const& deque) {
  for (auto& d : deque)
    std::cout << d << " ";
  std::cout << std::endl;
  
  return stream;
}