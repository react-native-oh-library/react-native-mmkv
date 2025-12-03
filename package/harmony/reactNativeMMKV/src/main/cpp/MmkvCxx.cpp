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

#include "MmkvCxx.h"
#include "MMKVPredef.h"
#include "MMKV.h"
#include "MmkvHostObject.h"
#include <stdio.h>
#include <string.h>

namespace rnoh {
using namespace facebook;
jsi::Value initialize(facebook::jsi::Runtime &rt, react::TurboModule &turboModule, const facebook::jsi::Value *args,
                      size_t count) {
#ifdef DEBUG
    MMKVLogLevel logLevel = MMKVLogDebug;
#else
    MMKVLogLevel logLevel = MMKVLogWarning;
#endif
    if (count != 1) {
        throw jsi::JSError(rt, "initialize 方法需传入 1 个配置对象参数");
    }

    const jsi::Value &jsConfigVal = args[0];
    if (!jsConfigVal.isString()) {
        throw jsi::JSError(rt, "传入的参数必须是string类型");
    }

    std::string basePath = jsConfigVal.asString(rt).utf8(rt);

    MMKV::initializeMMKV(basePath, logLevel);

    return jsi::Value(true);
}

jsi::Value createMMKV(facebook::jsi::Runtime &rt, react::TurboModule &turboModule, const facebook::jsi::Value *args,
                      size_t count) {
    DLOG(INFO) << "mmkv createMMKV";
    if (count != 1) {
        throw jsi::JSError(rt, "createMMKV 方法需传入 1 个配置对象参数");
    }

    const jsi::Value &jsConfigVal = args[0];
    if (!jsConfigVal.isObject()) {
        throw jsi::JSError(rt, "传入的参数必须是对象类型");
    }

    jsi::Object jsConfig = jsConfigVal.asObject(rt);

    std::string id;
    std::string mode = "SINGLE_PROCESS";
    std::optional<std::string> encryptionKey;
    bool readOnly = false;
    std::string path = "";

    if (!jsConfig.hasProperty(rt, "id")) {
        throw jsi::JSError(rt, "配置对象缺少必选属性 'id'");
    }
    jsi::Value jsId = jsConfig.getProperty(rt, "id");
    if (!jsId.isString()) {
        throw jsi::JSError(rt, "属性 'id' 必须是字符串类型");
    }
    id = jsId.asString(rt).utf8(rt);

    if (jsConfig.hasProperty(rt, "mode")) {
        jsi::Value jsMode = jsConfig.getProperty(rt, "mode");
        if (jsMode.isString()) {
            mode = jsMode.asString(rt).utf8(rt);
            // 可选：校验 mode 的有效值（枚举映射）
            if (mode != "SINGLE_PROCESS" && mode != "MULTI_PROCESS") {
                throw jsi::JSError(rt, "属性 'mode' 仅支持 'SINGLE_PROCESS' 或 'MULTI_PROCESS'");
            }
        } else {
            throw jsi::JSError(rt, "属性 'mode' 必须是字符串类型");
        }
    }

    if (jsConfig.hasProperty(rt, "encryptionKey")) {
        jsi::Value jsKey = jsConfig.getProperty(rt, "encryptionKey");
        if (!jsKey.isNull() && !jsKey.isUndefined()) {
            if (jsKey.isString()) {
                encryptionKey = jsKey.asString(rt).utf8(rt);
            } else {
                throw jsi::JSError(rt, "属性 'encryptionKey' 必须是字符串类型");
            }
        }
    }

    if (jsConfig.hasProperty(rt, "readOnly")) {
        jsi::Value jsReadOnly = jsConfig.getProperty(rt, "readOnly");
        if (jsReadOnly.isBool()) {
            readOnly = jsReadOnly.getBool(); // JS Boolean → C++ bool
        } else {
            throw jsi::JSError(rt, "属性 'jsReadOnly' 必须是布尔类型");
        }
    }

    if (jsConfig.hasProperty(rt, "path")) {
        jsi::Value jsPath = jsConfig.getProperty(rt, "path");
        if (jsPath.isString()) {
            path = jsPath.asString(rt).utf8(rt);
        } else {
            throw jsi::JSError(rt, "属性 'mode' 必须是字符串类型");
        }
    }

    NativeMmkvConfiguration config;
    config.id = id;
    if (mode == "SINGLE_PROCESS") {
        config.mode = NativeMmkvMode::SingleProcess;
    } else if (mode == "MULTI_PROCESS") {
        config.mode = NativeMmkvMode::MultiProcess;
    }
    if (path != "") {
    config.path = "/data/storage/el2/base/haps/entry/files" + path;
    }
    config.readOnly = readOnly;
    config.encryptionKey = encryptionKey;
    auto instance = std::make_shared<MmkvHostObject>(config);
    return jsi::Object::createFromHostObject(rt, instance);
}

MmkvCxx::MmkvCxx(const ArkTSTurboModule::Context ctx, const std::string name) : ArkTSTurboModule(ctx, name) {
    methodMap_ = {
        {"initialize", {1, rnoh::initialize}},
        {"createMMKV", {1, rnoh::createMMKV}},
    };
}


} // namespace rnoh
