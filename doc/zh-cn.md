> 模板版本：v0.3.0

<p align="center">
  <h1 align="center"> <code>react-native-mmkv</code> </h1>
</p>


本项目基于 [react-native-mmkv@v3.3.0](https://github.com/mrousavy/react-native-mmkv/tree/v3.3.0) 开发。

请到三方库的 Releases 发布地址查看配套的版本信息：

| 三方库版本                 | 发布信息                                      |  支持RN版本                 |
| ------------------------- | ------------------------------------------------- |  -------------------------- |
| v3.3.0                 | [@react-native-ohos/react-native-mmkv Releases](https://github.com/react-native-oh-library/react-native-mmkv/releases)  | 0.72/0.77 |


## 1. 安装与使用

进入到工程目录并输入以下命令：

#### **npm**

```bash
npm install @react-native-ohos/react-native-mmkv
```

#### **yarn**

```bash
yarn add @react-native-ohos/react-native-mmkv
```

下面的代码展示了这个库的基本使用场景：

> [!WARNING] 使用时 import 的库名不变。

```js
import { StyleSheet, Text, View, Button } from 'react-native';
import React, { useState } from 'react';
import { MMKV } from 'react-native-mmkv';

export default function getStringTest() {
  let storageString = new MMKV()
  const [getStringText, setgetStringText] =
    useState<string>('');

  function setProp() {
    storageString.set('mmkv.string.test.sex', "男")
    storageString.set('mmkv.string.test.name', "john")
    setgetStringText("set string success")
  }

  function getProp() {
    const name = storageString.getString('mmkv.string.test.name')
    const sex = storageString.getString('mmkv.string.test.sex')
    setgetStringText(`mmkv.string.test.name:${name} \n mmkv.string.test.sex:${sex}` )
  }

  return (
    <View>
        <Text>result：{getStringText}</Text>
          <View style={style.actionBtn}>
            <Button title="mmkv setStringTest" onPress={setProp}></Button>
            <Button title="mmkv getStringTest" onPress={getProp}></Button>
          </View>
    </View >
  );
}

const style = StyleSheet.create({
  cameraPreview: { width: '100%', aspectRatio: 56 / 100 },
  actionBtn: {
    flexDirection: 'row',
    flexWrap: 'wrap',
    justifyContent: 'space-evenly',
    padding: 10,
    gap: 5,
    position: 'absolute',
    top: 300,
  },
  text: {
    fontSize: 20,
    textAlign: 'center',
    color: '#000',
  },
  flipText: {
    color: 'white',
    fontSize: 15,
  },
  flipButton: {
    flex: 0.3,
    height: 40,
    marginHorizontal: 2,
    marginBottom: 10,
    marginTop: 10,
    borderRadius: 8,
    borderColor: 'white',
    borderWidth: 1,
    padding: 5,
    alignItems: 'center',
    justifyContent: 'center',
  },
  zoomText: {
    position: 'absolute',
    bottom: 70,
    zIndex: 2,
    left: 2,
  },
});

```

## 2. Manual Link

此步骤为手动配置原生依赖项的指导。

首先需要使用 DevEco Studio 打开项目里的 HarmonyOS 工程 `harmony`。

### 2.1. Overrides RN SDK

为了让工程依赖同一个版本的 RN SDK，需要在工程根目录的 `oh-package.json5` 添加 overrides 字段，指向工程需要使用的 RN SDK 版本。替换的版本既可以是一个具体的版本号，也可以是一个模糊版本，还可以是本地存在的 HAR 包或源码目录。

关于该字段的作用请阅读[官方说明](https://developer.huawei.com/consumer/cn/doc/harmonyos-guides-V5/ide-oh-package-json5-V5#zh-cn_topic_0000001792256137_overrides)：

```json
{
  "overrides": {
    "@rnoh/react-native-openharmony" : "./react_native_openharmony.har" // 指向本地 har 包的路径
    // "@rnoh/react-native-openharmony" : "./react_native_openharmony" // 指向源码路径
  }
}
```
### 2.2. 引入原生端代码

目前有两种方法：

1. 通过 har 包引入（在 IDE 完善相关功能后该方法会被遗弃，目前首选此方法）；
2. 直接链接源码。

方法一：通过 har 包引入（推荐）

> [!TIP] har 包位于三方库安装路径的 `harmony` 文件夹下。

打开 `entry/oh-package.json5`，添加以下依赖

```json
"dependencies": {
    "@react-native-ohos/react-native-mmkv": "file:../../node_modules/@react-native-ohos/react-native-mmkv/harmony/reactNativeMMKV.har"
  }
```

点击右上角的 `sync` 按钮

或者在终端执行：

```bash
cd entry
ohpm install
```

方法二：直接链接源码

> [!TIP] 如需使用直接链接源码，请参考[直接链接源码说明](/link-source-code.md)

### 2.3.配置 CMakeLists 和引入 NativeCameraPackage

打开 `entry/src/main/cpp/CMakeLists.txt`，添加：

```diff
+ set(OH_MODULES "${CMAKE_CURRENT_SOURCE_DIR}/../../../oh_modules")

# RNOH_BEGIN: manual_package_linking_1
+ add_subdirectory("${OH_MODULES}/@react-native-ohos/react-native-mmkv/src/main/cpp" ./reactNativeMMKV)
# RNOH_END: manual_package_linking_1

# RNOH_BEGIN: manual_package_linking_2
+ target_link_libraries(rnoh_app PUBLIC rnoh_native_mmkv)
# RNOH_END: manual_package_linking_2
```

打开 `entry/src/main/cpp/PackageProvider.cpp`，添加：

```diff
#include "RNOH/PackageProvider.h"
#include "generated/RNOHGeneratedPackage.h"
+ #include "NativeMMKVPackage.h"

using namespace rnoh;

std::vector<std::shared_ptr<Package>> PackageProvider::getPackages(Package::Context ctx) {
    return {
      std::make_shared<RNOHGeneratedPackage>(ctx),
+      std::make_shared<NativeMMKVPackage>(ctx)
    };
}
```

###  2.4.在 ArkTs 侧引入 reactNativeMMKVPackage

打开 `entry/src/main/ets/RNPackagesFactory.ts`，添加：


```diff  
  ... 
+ import { MmkvCxxPackage, MmkvPlatformContextPackage } from '@react-native-ohos/react-native-mmkv/ts';
export function createRNPackages(ctx: RNPackageContext): RNPackage[] {
  return [
    new SamplePackage(ctx),
+   new MmkvPlatformContextPackage(ctx),
+   new MmkvCxxPackage(ctx)
  ];
}
```

###  2.6.运行

点击右上角的 `sync` 按钮

或者在终端执行：

```bash
cd entry
ohpm install
```

然后编译、运行即可。

## 3.  约束与限制

### 3.1. 兼容性

本文档内容基于以下环境验证通过：
1. RNOH:0.72.96; SDK: HarmonyOS 6.0.0 Release SDK; IDE: DevEco Studio  6.0.0.868; ROM: 6.0.0.112;
2. RNOH:0.77.18; SDK: HarmonyOS 6.0.0 Release SDK; IDE: DevEco Studio  6.0.0.868; ROM: 6.0.0.112;

## 4. 静态方法

> [!TIP] "Platform"列表示该属性在原三方库上支持的平台。

> [!TIP] "HarmonyOS Support"列为 yes 表示 HarmonyOS 平台支持该属性；no 则表示不支持；partially 表示部分支持。使用方法跨平台一致，效果对标 iOS 或 Android 的效果。

| Name | Description | Type | Required | Platform | HarmonyOS Support |
| ---- | ----------- | ---- | -------- | -------- |-------------------|
| set      | 存储键值对数据 | void | no | iOS/Android | yes             |
| getBoolean     | 获取boolean值                 | boolean \| undefined | no       | iOS/Android | yes               |
| getString          | 获取string值             | string \| undefined      | no       | iOS/Android | yes               |
| getNumber        | 获取number值             | number \| undefined                | no       | iOS/Android | yes               |
| getBuffer     | 获取buffer值 | ArrayBufferLike \| undefined          | no       | iOS/Android | yes               |
| contains | 判断mmkv是否包含某个key值             | boolean                | no       | iOS/Android | yes               |
| delete      | mmkv删除某个key值 | void | no | iOS/Android | yes             |
| getAllKeys     | 获取所有key值                 | string[] | no       | iOS/Android | yes               |
| clearAll          | 删除所有数据             | void      | no       | iOS/Android | yes               |
| recrypt        | mmkv设置加密             | void                | no       | iOS/Android | yes               |
| trim     | mmkv清理存储内容 | void          | no       | iOS/Android | yes               |
| toString | 获取所有key值并转成string值             | string                | no       | iOS/Android | yes               |
| addOnValueChangedListener      | 设置数据变动的监听 | Listener | no | iOS/Android | yes             |
| size     | 获取mmkv存储的文件大小                 | number | no       | iOS/Android | yes               |
| isReadOnly          | 是否设置mmkv只读             | boolean      | no       | iOS/Android | yes               |
| useMMKV        | 使用hook的方式创建mmkv             | MMKV                | no       | iOS/Android | yes               |
| useMMKVString     | 使用hook的方式存储String | MMKV          | no       | iOS/Android | yes               |
| useMMKVNumber | 使用hook的方式存储Number           | (    key: string,    instance?: MMKV  ): [value: T, setValue: (value: TSetAction) => void] | no       | iOS/Android | yes               |
| useMMKVBoolean      | 使用hook的方式存储Boolean | (    key: string,    instance?: MMKV  ): [value: T, setValue: (value: TSetAction) => void] | no | iOS/Android | yes             |
| useMMKVBuffer     | 使用hook的方式存储Buffer                 | (    key: string,    instance?: MMKV  ): [value: T, setValue: (value: TSetAction) => void] | no       | iOS/Android | yes               |
| useMMKVObject          | 使用hook的方式存储Object             | (    key: string,    instance?: MMKV  ): [value: T, setValue: (value: TSetAction) => void] | no       | iOS/Android | yes               |
| useMMKVListener        | 使用hook的方式监听数据变动             | (    key: string,    instance?: MMKV  ): [value: T, setValue: (value: TSetAction) => void] | no       | iOS/Android | yes               |


## 5.遗留问题
无

## 6.其他
无

## 7.开源协议

本项目基于 [MIT License (MIT)](https://github.com/mrousavy/react-native-mmkv/blob/v3.3.0/LICENSE) ，请自由地享受和参与开源。
