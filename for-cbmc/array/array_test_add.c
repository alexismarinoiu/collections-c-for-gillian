int nondet_int(); char nondet_char(); /*
 * Collections-C
 * Copyright (C) 2013-2014 Srđan Panić <srdja.panic@gmail.com>
 *
 * This file is part of Collections-C.
 *
 * Collections-C is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Collections-C is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Collections-C.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "array.h"

#define DEFAULT_CAPACITY 8
#define DEFAULT_EXPANSION_FACTOR 2

struct array_s {
    size_t   size;
    size_t   capacity;
    float    exp_factor;
    void   **buffer;

    void *(*mem_alloc)  (size_t size);
    void *(*mem_calloc) (size_t blocks, size_t size);
    void  (*mem_free)   (void *block);
};

static enum cc_stat expand_capacity(Array *ar);


/**
 * Creates a new empty array and returns a status code.
 *
 * @param[out] out pointer to where the newly created Array is to be stored
 *
 * @return CC_OK if the creation was successful, or CC_ERR_ALLOC if the
 * memory allocation for the new Array structure failed.
 */
enum cc_stat array_new(Array **out)
{
    ArrayConf c;
    array_conf_init(&c);
    return array_new_conf(&c, out);
}

/**
 * Creates a new empty Array based on the specified ArrayConf struct and
 * returns a status code.
 *
 * The Array is allocated using the allocators specified in the ArrayConf
 * struct. The allocation may fail if underlying allocator fails. It may also
 * fail if the values of exp_factor and capacity in the ArrayConf do not meet
 * the following condition: <code>exp_factor < (CC_MAX_ELEMENTS / capacity)</code>.
 *
 * @param[in] conf array configuration structure
 * @param[out] out pointer to where the newly created Array is to be stored
 *
 * @return CC_OK if the creation was successful, CC_ERR_INVALID_CAPACITY if
 * the above mentioned condition is not met, or CC_ERR_ALLOC if the memory
 * allocation for the new Array structure failed.
 */
enum cc_stat array_new_conf(ArrayConf const * const conf, Array **out)
{
    float ex;

    /* The expansion factor must be greater than one for the
     * array to grow */
    if (conf->exp_factor <= 1)
        ex = DEFAULT_EXPANSION_FACTOR;
    else
        ex = conf->exp_factor;

    /* Needed to avoid an integer overflow on the first resize and
     * to easily check for any future overflows. */
    if (!conf->capacity || ex >= CC_MAX_ELEMENTS / conf->capacity)
        return CC_ERR_INVALID_CAPACITY;

    Array *ar = conf->mem_calloc(1, sizeof(Array));

    if (!ar)
        return CC_ERR_ALLOC;

    void **buff = conf->mem_alloc(conf->capacity * sizeof(void*));

    if (!buff) {
        conf->mem_free(ar);
        return CC_ERR_ALLOC;
    }

    ar->buffer     = buff;
    ar->exp_factor = ex;
    ar->capacity   = conf->capacity;
    ar->mem_alloc  = conf->mem_alloc;
    ar->mem_calloc = conf->mem_calloc;
    ar->mem_free   = conf->mem_free;

    *out = ar;
    return CC_OK;
}

/**
 * Initializes the fields of the ArrayConf struct to default values.
 *
 * @param[in, out] conf ArrayConf structure that is being initialized
 */
void array_conf_init(ArrayConf *conf)
{
    conf->exp_factor = DEFAULT_EXPANSION_FACTOR;
    conf->capacity   = DEFAULT_CAPACITY;
    conf->mem_alloc  = malloc;
    conf->mem_calloc = calloc;
    conf->mem_free   = free;
    return;
}

void array_destroy(Array *ar)
{
    ar->mem_free(ar->buffer);
    ar->mem_free(ar);
}


/**
 * Adds a new element to the Array. The element is appended to the array making
 * it the last element (the one with the highest index) of the Array.
 *
 * @param[in] ar the array to which the element is being added
 * @param[in] element the element that is being added
 *
 * @return CC_OK if the element was successfully added, CC_ERR_ALLOC if the
 * memory allocation for the new element failed, or CC_ERR_MAX_CAPACITY if the
 * array is already at maximum capacity.
 */
enum cc_stat array_add(Array *ar, void *element)
{
    if (ar->size >= ar->capacity) {
        enum cc_stat status = expand_capacity(ar);
        if (status != CC_OK)
            return status;
    }

    ar->buffer[ar->size] = element;
    ar->size++;

    return CC_OK;
}

/**
 * Gets an Array element from the specified index and sets the out parameter to
 * its value. The specified index must be within the bounds of the array.
 *
 * @param[in] ar the array from which the element is being retrieved
 * @param[in] index the index of the array element
 * @param[out] out pointer to where the element is stored
 *
 * @return CC_OK if the element was found, or CC_ERR_OUT_OF_RANGE if the index
 * was out of range.
 */
enum cc_stat array_get_at(Array *ar, size_t index, void **out)
{
    if (index >= ar->size)
        return CC_ERR_OUT_OF_RANGE;

    *out = ar->buffer[index];
    return CC_OK;
}
/**
 * Expands the Array capacity. This might fail if the the new buffer
 * cannot be allocated. In case the expansion would overflow the index
 * range, a maximum capacity buffer is allocated instead. If the capacity
 * is already at the maximum capacity, no new buffer is allocated.
 *
 * @param[in] ar array whose capacity is being expanded
 *
 * @return CC_OK if the buffer was expanded successfully, CC_ERR_ALLOC if
 * the memory allocation for the new buffer failed, or CC_ERR_MAX_CAPACITY
 * if the array is already at maximum capacity.
 */
static enum cc_stat expand_capacity(Array *ar)
{
    if (ar->capacity == CC_MAX_ELEMENTS)
        return CC_ERR_MAX_CAPACITY;

    size_t new_capacity = ar->capacity * ar->exp_factor;

    /* As long as the capacity is greater that the expansion factor
     * at the point of overflow, this is check is valid. */
    if (new_capacity <= ar->capacity)
        ar->capacity = CC_MAX_ELEMENTS;
    else
        ar->capacity = new_capacity;

    void **new_buff = ar->mem_alloc(new_capacity * sizeof(void*));

    if (!new_buff)
        return CC_ERR_ALLOC;

    memcpy(new_buff, ar->buffer, ar->size * sizeof(void*));

    // ar->mem_free(ar->buffer);
    ar->buffer = new_buff;

    return CC_OK;
}

/* TESTS START HERE */

static Array *v1;
static Array *v2;
static ArrayConf vc;
static int stat;

int main() {
  stat = array_new(&v1);
  assert(stat == CC_OK);

  int a = nondet_int();
  int b = nondet_int();
  int c = nondet_int();
  
  array_add(v1, &a);
  array_add(v1, &b);
  array_add(v1, &c);

  int *ar;
  int *br;
  int *cr;

  array_get_at(v1, 0, (void*)&ar);
  array_get_at(v1, 1, (void*)&br);
  array_get_at(v1, 2, (void*)&cr);

  assert(a == *ar);
  assert(b == *br);
  assert(c == *cr);

//   array_destroy(v1);
  
  return 0;
}
