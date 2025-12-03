/**
 * MIT License
 *
 * Copyright (C) 2025 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include "MMKV.h"
#include <jsi/jsi.h>


using namespace facebook;
using namespace mmkv;


enum class NativeMmkvMode : int {
    SingleProcess = 0,
    MultiProcess = 1,
};

struct NativeMmkvConfiguration {
    // MMKV 实例唯一标识（对应存储文件名称）
    std::string id;
    // 加密密钥（AES-128/256，为空则不加密）
    std::optional<std::string> encryptionKey;
    NativeMmkvMode mode;
    bool readOnly;
    // 自定义存储根路径（可选，默认使用平台默认路径）
    std::optional<std::string> path;
};

using MMKVConfig = NativeMmkvConfiguration;

class MmkvHostObject : public jsi::HostObject {
public:
    MmkvHostObject(const MMKVConfig &config);
    ~MmkvHostObject();

public:
    jsi::Value get(jsi::Runtime &, const jsi::PropNameID &name) override;
    std::vector<jsi::PropNameID> getPropertyNames(jsi::Runtime &rt) override;

private:
    static MMKVMode getMMKVMode(const MMKVConfig &config);

private:
    MMKV *instance;
};
