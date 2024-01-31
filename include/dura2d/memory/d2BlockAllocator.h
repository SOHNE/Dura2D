// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef D2BLOCKALLOCATOR_H
#define D2BLOCKALLOCATOR_H

#include "dura2d/d2api.h"
#include "dura2d/d2Types.h"

// Allocation sizes
constexpr int d2_blockSizeCount = 14;

// Forward declarations
struct d2Block;
struct d2Chunk;

/**
 * @brief The d2BlockAllocator class is responsible for managing memory allocation and deallocation.
 * It maintains a list of free blocks of different sizes, and can allocate and deallocate blocks of memory.
 */
class D2_API d2BlockAllocator
{
public:
    /** @brief Default constructor */
    d2BlockAllocator();

    /** @brief Destructor */
    ~d2BlockAllocator();

    /**
     * @brief Allocates a block of memory of the specified size
     * @param size The size of the block to allocate
     * @return A pointer to the allocated block
     */
    void* Allocate(int32 size);

    /**
     * @brief Frees a block of memory
     * @param p A pointer to the block to free
     * @param size The size of the block to free
     */
    void Free(void* p, int32 size);

    /** @brief Clears all allocated blocks */
    void Clear();

private:
    /** @brief A pointer to the first chunk in the list of chunks */
    d2Chunk* m_chunks {nullptr};

    /** @brief The number of chunks in the list */
    int m_chunkCount {0};

    /** @brief The spacing between chunks in the list */
    int m_chunkSpacing {0};

    /** @brief An array of pointers to the first block in each free list */
    d2Block* m_freeLists[d2_blockSizeCount] {nullptr};
};

#endif //D2BLOCKALLOCATOR_H