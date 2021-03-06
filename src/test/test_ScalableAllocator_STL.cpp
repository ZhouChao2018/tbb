/*
    Copyright (c) 2005-2019 Intel Corporation

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.




*/

// Test whether scalable_allocator works with some of the host's STL containers.

#define HARNESS_NO_PARSE_COMMAND_LINE 1
#define __TBB_EXTRA_DEBUG 1 // enables additional checks
#define TBB_PREVIEW_MEMORY_POOL 1

#include "harness_assert.h"
#include "tbb/memory_pool.h"
#include "tbb/scalable_allocator.h"
#include <iostream>

// The actual body of the test is there:
#include "test_allocator_STL.h"

int TestMain () {
    TestAllocatorWithSTL<tbb::scalable_allocator<void> >();
    tbb::memory_pool<tbb::scalable_allocator<int> > mpool;
    TestAllocatorWithSTL(tbb::memory_pool_allocator<void>(mpool) );
    static char buf[1024*1024*4];
    tbb::fixed_pool fpool(buf, sizeof(buf));
    TestAllocatorWithSTL(tbb::memory_pool_allocator<void>(fpool) );

#if __TBB_CPP17_MEMORY_RESOURCE_PRESENT
    ASSERT(!tbb::scalable_memory_resource()->is_equal(*std::pmr::get_default_resource()),
            "Scalable resource shouldn't be equal to standard resource." );
    ASSERT(tbb::scalable_memory_resource()->is_equal(*tbb::scalable_memory_resource()),
            "Memory that was allocated by one scalable resource should be deallocated by any other instance.");

    typedef std::pmr::polymorphic_allocator<void> pmr_alloc_t;
    TestAllocatorWithSTL(pmr_alloc_t(tbb::scalable_memory_resource()));
#endif

    return Harness::Done;
}
