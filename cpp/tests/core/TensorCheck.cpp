// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018-2021 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "open3d/core/TensorCheck.h"

#include "tests/UnitTest.h"
#include "tests/core/CoreTest.h"

#define LogDayin(...) \
    _LogDayin(__FILE__, __LINE__, (const char *)__FN__, __VA_ARGS__)

// In console.cpp
void VDayin(const char *file_name,
            int line_number,
            const char *function_name,
            const char *message) {
    std::string err_msg = fmt::format("[Open3D DAYIN] {}", message);
    std::cout << err_msg << std::endl;
}

void VDayin(const char *file_name,
            int line_number,
            const char *function_name,
            const char *format,
            fmt::format_args args) {
    std::string err_msg = fmt::vformat(format, args);
    VDayin(file_name, line_number, function_name, err_msg.c_str());
}

// In console.h
template <typename... Args>
void _LogDayin(const char *file_name,
               int line_number,
               const char *function_name,
               const char *format,
               Args &&... args) {
    if (sizeof...(Args) > 0) {
        VDayin(file_name, line_number, function_name, format,
               fmt::make_format_args(args...));
    } else {
        VDayin(file_name, line_number, function_name, format);
    }
}

namespace open3d {
namespace tests {

class TensorCheckPermuteDevices : public PermuteDevices {};
INSTANTIATE_TEST_SUITE_P(Tensor,
                         TensorCheckPermuteDevices,
                         testing::ValuesIn(PermuteDevices::TestCases()));

TEST_P(TensorCheckPermuteDevices, AssertTensorDtype) {
    core::Device device = GetParam();
    core::Tensor t = core::Tensor::Empty({}, core::Float32, device);

    // Check error message in Google test:
    // https://stackoverflow.com/a/23271612/1255535
    try {
        core::AssertTensorDtype(t, core::Int32);
        FAIL() << "Should not reach here.";
    } catch (std::runtime_error const &err) {
        EXPECT_TRUE(utility::ContainsString(
                err.what(),
                "Tensor has dtype Float32, but is expected to be Int32."));
        EXPECT_TRUE(utility::ContainsString(err.what(),
                                            "tests/core/TensorCheck.cpp:"));
        EXPECT_TRUE(utility::ContainsString(err.what(), "AssertTensorDtype"));
    } catch (...) {
        FAIL() << "std::runtime_error not thrown.";
    }
}

TEST_P(TensorCheckPermuteDevices, AssertTensorDevice) {
    core::Device device = GetParam();
    core::Tensor t = core::Tensor::Empty({}, core::Float32, device);

    try {
        core::AssertTensorDevice(t, core::Device("CUDA:1000"));
        FAIL() << "Should not reach here.";
    } catch (std::runtime_error const &err) {
        EXPECT_TRUE(utility::ContainsString(err.what(), "Tensor has device"));
        EXPECT_TRUE(utility::ContainsString(err.what(),
                                            "but is expected to be CUDA:1000"));
        EXPECT_TRUE(utility::ContainsString(err.what(),
                                            "tests/core/TensorCheck.cpp:"));
        EXPECT_TRUE(utility::ContainsString(err.what(), "AssertTensorDevice"));
    } catch (...) {
        FAIL() << "std::runtime_error not thrown.";
    }
}

TEST_P(TensorCheckPermuteDevices, AssertTensorShape) {
    core::Device device = GetParam();
    core::Tensor t = core::Tensor::Empty({}, core::Float32, device);

    std::cout << "To call LogDayin with {}" << std::endl;
    LogDayin("hello {}", "hello {} world");
    std::cout << "Called LogDayin with {}" << std::endl;
    std::cout << "##################################" << std::endl;

    std::cout << "To call _LogDayin" << std::endl;
    _LogDayin("file", 100, "function", "hello world");
    std::cout << "Called _LogDayin" << std::endl;
    std::cout << "##################################" << std::endl;

    std::cout << "To call _LogDayin with {}" << std::endl;
    _LogDayin("file", 100, "function", "hello {} world");
    std::cout << "Called _LogDayin with {}" << std::endl;
}

TEST_P(TensorCheckPermuteDevices, AssertTensorShapeCompatible) {
    core::Device device = GetParam();
    (void)device;
}

}  // namespace tests
}  // namespace open3d
