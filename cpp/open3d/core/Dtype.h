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

#pragma once

#include <cstring>
#include <string>

#include "open3d/Macro.h"
#include "open3d/core/Dispatch.h"
#include "open3d/utility/Logging.h"

namespace open3d {
namespace core {

class OPEN3D_API Dtype {
public:
    static const Dtype Undefined;
    static const Dtype Float32;
    static const Dtype Float64;
    static const Dtype Int8;
    static const Dtype Int16;
    static const Dtype Int32;
    static const Dtype Int64;
    static const Dtype UInt8;
    static const Dtype UInt16;
    static const Dtype UInt32;
    static const Dtype UInt64;
    static const Dtype Bool;

public:
    enum class DtypeCode {
        Undefined,
        Bool,  // Needed to distinguish bool from uint8_t.
        Int,
        UInt,
        Float,
        Object,
    };

    Dtype() : Dtype(DtypeCode::Undefined, 1, "Undefined") {}

    Dtype(DtypeCode dtype_code, int64_t byte_size, const std::string &name);

    /// Convert from C++ types to Dtype. Known types are explicitly specialized,
    /// e.g. FromType<float>(). Unsupported type results in an exception.
    template <typename T>
    static inline const Dtype FromType() {
        utility::LogError("Unsupported data for Dtype::FromType.");
    }

    int64_t ByteSize() const { return byte_size_; }

    DtypeCode GetDtypeCode() const { return dtype_code_; }

    bool IsObject() const { return dtype_code_ == DtypeCode::Object; }

    std::string ToString() const { return name_; }

    bool operator==(const Dtype &other) const;

    bool operator!=(const Dtype &other) const;

private:
    static constexpr size_t max_name_len_ = 16;
    DtypeCode dtype_code_;
    int64_t byte_size_;
    char name_[max_name_len_];  // MSVC warns if std::string is exported to DLL.
};

const Dtype kUndefined = Dtype::Undefined;
const Dtype kFloat32 = Dtype::Float32;
const Dtype kFloat64 = Dtype::Float64;
const Dtype kInt8 = Dtype::Int8;
const Dtype kInt16 = Dtype::Int16;
const Dtype kInt32 = Dtype::Int32;
const Dtype kInt64 = Dtype::Int64;
const Dtype kUInt8 = Dtype::UInt8;
const Dtype kUInt16 = Dtype::UInt16;
const Dtype kUInt32 = Dtype::UInt32;
const Dtype kUInt64 = Dtype::UInt64;
const Dtype kBool = Dtype::Bool;

template <>
inline const Dtype Dtype::FromType<float>() {
    return Dtype::Float32;
}

template <>
inline const Dtype Dtype::FromType<double>() {
    return Dtype::Float64;
}

template <>
inline const Dtype Dtype::FromType<int8_t>() {
    return Dtype::Int8;
}

template <>
inline const Dtype Dtype::FromType<int16_t>() {
    return Dtype::Int16;
}

template <>
inline const Dtype Dtype::FromType<int32_t>() {
    return Dtype::Int32;
}

template <>
inline const Dtype Dtype::FromType<int64_t>() {
    return Dtype::Int64;
}

template <>
inline const Dtype Dtype::FromType<uint8_t>() {
    return Dtype::UInt8;
}

template <>
inline const Dtype Dtype::FromType<uint16_t>() {
    return Dtype::UInt16;
}

template <>
inline const Dtype Dtype::FromType<uint32_t>() {
    return Dtype::UInt32;
}

template <>
inline const Dtype Dtype::FromType<uint64_t>() {
    return Dtype::UInt64;
}

template <>
inline const Dtype Dtype::FromType<bool>() {
    return Dtype::Bool;
}

}  // namespace core
}  // namespace open3d
