#include "dura2d/memory/d2BlockAllocator.h"

#include <cstring>
#include <cstdlib>

static constexpr int d2_chunkSize = 16 * 1024;
static constexpr int d2_maxBlockSize = 640;
static constexpr int d2_chunkArrayIncrement = 128;

static constexpr int d2_blockSizes[d2_blockSizeCount] =
{
        16,     // 0
        32,     // 1
        64,     // 2
        96,     // 3
        128,    // 4
        160,    // 5
        192,    // 6
        224,    // 7
        256,    // 8
        320,    // 9
        384,    // 10
        448,    // 11
        512,    // 12
        640,    // 13
};

struct d2SizeMap
{
    uint8 values[d2_maxBlockSize + 1]{};

    d2SizeMap()
    {
        int j = 0;
        values[0] = 0;
        for (int i = 1; i <= d2_maxBlockSize; ++i)
        {
            if (i <= d2_blockSizes[j])
            {
                values[i] = static_cast<uint8>(j);
            }
            else
            {
                ++j;
                values[i] = static_cast<uint8>(j);
            }
        }
    }
};

static const d2SizeMap d2_sizeMap;

struct d2Block
{
    d2Block* next;
};

struct d2Chunk
{
    int blockSize;
    d2Block* blocks;
};

d2BlockAllocator::d2BlockAllocator()
{
    m_chunkSpacing = d2_chunkArrayIncrement;
    m_chunkCount = 0;
    m_chunks = (d2Chunk*)(malloc(m_chunkSpacing * sizeof(d2Chunk)));

    std::memset(m_chunks, 0, m_chunkSpacing * sizeof(d2Chunk));
    std::memset(m_freeLists, 0, sizeof(m_freeLists));
}

d2BlockAllocator::~d2BlockAllocator()
{
    for (signed int i = 0; i < m_chunkCount; ++i)
    {
        std::free(m_chunks[i].blocks);
    }

    std::free(m_chunks);
}

void* d2BlockAllocator::Allocate(int32 size)
{
    if (size == 0) return nullptr;

    if (size > d2_maxBlockSize) return malloc(size);

    int index = d2_sizeMap.values[size];

    if (m_freeLists[index])
    {
        d2Block* block = m_freeLists[index];
        m_freeLists[index] = block->next;
        return block;
    }
    else
    {
        if (m_chunkCount == m_chunkSpacing)
        {
            d2Chunk* oldInitializer = m_chunks;
            m_chunkSpacing += d2_chunkArrayIncrement;
            m_chunks = (d2Chunk*)(malloc(m_chunkSpacing * sizeof(d2Chunk)));
            std::memcpy(m_chunks, oldInitializer, m_chunkCount * sizeof(d2Chunk));
            std::memset(m_chunks + m_chunkCount, 0, d2_chunkArrayIncrement * sizeof(d2Chunk));
            free(oldInitializer);
        }

        d2Chunk* chunk = m_chunks + m_chunkCount;
        chunk->blocks = (d2Block*)(malloc(d2_chunkSize));

        int32 blockSize = d2_blockSizes[index];
        chunk->blockSize = blockSize;
        int32 blockCount = d2_chunkSize / blockSize;

        for (int i = 0; i < blockCount - 1; ++i)
        {
            d2Block* block = (d2Block*)((uint8*)(chunk->blocks) + blockSize * i);
            d2Block* next = (d2Block*)((uint8*)(chunk->blocks) + blockSize * (i + 1));
            block->next = next;
        }
        d2Block* last = (d2Block*)((uint8*)(chunk->blocks) + blockSize * (blockCount - 1));
        last->next = nullptr;

        m_freeLists[index] = chunk->blocks->next;
        ++m_chunkCount;

        return chunk->blocks;
    }
}

void d2BlockAllocator::Free(void* p, int32 size)
{
    if (size == 0) return;

    if (size > d2_maxBlockSize)
    {
        std::free(p);
        return;
    }

    int index = d2_sizeMap.values[size];

    d2Block* block = (d2Block*)(p);
    block->next = m_freeLists[index];
    m_freeLists[index] = block;
}

void d2BlockAllocator::Clear()
{
    for (int i = 0; i < m_chunkCount; ++i)
    {
        std::free(m_chunks[i].blocks);
    }

    m_chunkCount = 0;
    std::memset(m_chunks, 0, m_chunkSpacing * sizeof(d2Chunk));
    std::memset(m_freeLists, 0, sizeof(m_freeLists));
}
