/**
 * @file   GpuAddTest.cpp
 * @brief  GpuAdd class tester.
 * @author zer0
 * @date   2018-01-07
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/backend/kernels/GpuAdd.hpp>
#include <libtbag/gpu/Gpu.hpp>

using namespace libtbag;
using namespace libtbag::gpu;
using namespace libtbag::gpu::backend;
using namespace libtbag::gpu::backend::kernels;

struct GpuAddKernelTest
{
    UniqueGpu   gpu;
    GpuContext  context;
    GpuStream   stream;
    int         count;

    GpuMemory  gpu_v1;
    GpuMemory  gpu_v2;
    GpuMemory  gpu_result;

    HostMemory  host_v1;
    HostMemory  host_v2;
    HostMemory  host_result;

    GpuAddKernelTest(GpuBackendType t, int c) : GpuAddKernelTest(createGpuContext(t), c)
    {
        // EMPTY.
    }

    GpuAddKernelTest(UniqueGpu && g, int c) : gpu(std::move(g)), count(c)
    {
        context = gpu->createContext(0, 0);
        assert(context.isUnknownContext() == false);

        stream = gpu->createStream(context);
        assert(stream.isUnknownStream() == false);

        gpu_v1     = gpu->malloc(context, sizeof(float) * count);
        gpu_v2     = gpu->malloc(context, sizeof(float) * count);
        gpu_result = gpu->malloc(context, sizeof(float) * count);

        host_v1     = gpu->mallocHost(context, sizeof(float) * count);
        host_v2     = gpu->mallocHost(context, sizeof(float) * count);
        host_result = gpu->mallocHost(context, sizeof(float) * count);
    }

    ~GpuAddKernelTest()
    {
        gpu->freeHost(host_v1);
        gpu->freeHost(host_v2);
        gpu->freeHost(host_result);

        gpu->free(gpu_v1);
        gpu->free(gpu_v2);
        gpu->free(gpu_result);

        gpu->releaseStream(stream);
        gpu->releaseContext(context);
    }

    bool run(std::vector<float> const & v1, std::vector<float> const & v2, std::vector<float> & result)
    {
        ::memcpy(host_v1.data, v1.data(), sizeof(float) * count);
        ::memcpy(host_v2.data, v2.data(), sizeof(float) * count);

        if (gpu->write(stream, gpu_v1, host_v1, sizeof(float) * count) == false) { return false; }
        if (gpu->write(stream, gpu_v2, host_v2, sizeof(float) * count) == false) { return false; }
        if (gpu->runAdd(stream, gpu_v1, gpu_v2, gpu_result, type::TypeTable::TT_FLOAT, count) == false) { return false; }
        if (gpu->finish(stream) == false) { return false; }
        if (gpu->read(stream, gpu_result, host_result, sizeof(float) * count) == false) { return false; }

        ::memcpy(result.data(), host_result.data, sizeof(float) * count);
        return true;
    }
};

TEST(GpuAddTest, Cpu)
{
    runAllIfSupported([](UniqueGpu & gpu){
        std::cout << "GPU type: " << gpu->getTypeString() << std::endl;

        if (gpu->getType() != GpuBackendType::GBT_CPU || gpu->getType() != GpuBackendType::GBT_CUDA) {
            return;
        }

        int const TEST_COUNT = 99;
        GpuAddKernelTest tester(gpu->getType(), TEST_COUNT);

        std::vector<float> v1(TEST_COUNT);
        std::vector<float> v2(TEST_COUNT);
        std::vector<float> v3(TEST_COUNT);
        std::vector<float> result(TEST_COUNT);
        std::size_t i = 0;

        for (i = 0; i < TEST_COUNT; ++i) {
            v1[i] = i;
            v2[i] = 100 * i;
            v3[i] = v1[i] + v2[i];
        }

        ASSERT_TRUE(tester.run(v1, v2, result));
        for (i = 0; i < TEST_COUNT; ++i) {
            ASSERT_EQ(v3[i], result[i]);
        }
    });
}

