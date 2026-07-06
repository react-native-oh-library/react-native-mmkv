> Template version: v0.3.0

<p align="center">
  <h1 align="center"> <code>react-native-mmkv</code> </h1>
</p>

This project is based on [react-native-mmkv@v3.3.0](https://github.com/mrousavy/react-native-mmkv/tree/v3.3.0)。

Please check the corresponding version information in the Releases section of the third-party library's repository:

| Version                                    | Repository                                               |  Version for RN                  |
| ------------------------- | ------------------------------------------------- |  -------------------------- |
| v3.3.0                 | [@react-native-ohos/react-native-mmkv Releases](https://github.com/react-native-oh-library/react-native-mmkv/releases)  | 0.72/0.77 |


## 1. Installation and Usage

Go to the project directory and execute the following instruction:

#### **npm**

```bash
npm install @react-native-ohos/react-native-mmkv
```

#### **yarn**

```bash
yarn add @react-native-ohos/react-native-mmkv
```

The following code shows the basic use scenario of the repository:

> [!WARNING] The name of the imported repository remains unchanged.

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

This section provides guidance for manually linking native dependencies.

First, you need to open the HarmonyOS project `harmony` in your project with DevEco Studio.

### 2.1. Overrides RN SDK

To ensure the project depends on the same version of the RN SDK, you need to add an `overrides` field in the `harmony/oh-package.json5` file at the project's root directory. This field should point to the RN SDK version required by the project. The version can be a specific version number, a fuzzy version, a local HAR package, or a source code directory.

For more information about this field, please refer to the [official documentation](https://developer.huawei.com/consumer/cn/doc/harmonyos-guides-V5/ide-oh-package-json5-V5#zh-cn_topic_0000001792256137_overrides).

```json
{
  "overrides": {
    "@rnoh/react-native-openharmony" : "./react_native_openharmony.har" // Path to local har package
    // "@rnoh/react-native-openharmony" : "./react_native_openharmony" // Point to source code path
  }
}
```
### 2.2. Importing Native Code
- By importing HAR packages;
- By directly linking the source code.

Method 1: Importing via HAR package (Recommended)

> [!TIP]
> The HAR package is located in the `harmony` folder of the third-party library's installation path.

Open `entry/oh-package.json5` and add the following dependency:

```json
"dependencies": {
    "@react-native-ohos/react-native-mmkv": "file:../../node_modules/@react-native-ohos/react-native-mmkv/harmony/reactNativeMMKV.har"
  }
```

Click the `sync` button in the top right corner.

Or execute in the terminal:

```bash
cd entry
ohpm install
```

Method 2: Directly linking the source code

> [!TIP]
> To link the source code directly, please refer to the [instructions for linking source code directly](/zh-cn/link-source-code.md).


### 2.3.Configure CMakeLists and Import NativeMMKVPackage

Open `entry/src/main/cpp/CMakeLists.txt` and add the following:

```diff
+ set(OH_MODULES "${CMAKE_CURRENT_SOURCE_DIR}/../../../oh_modules")

# RNOH_BEGIN: manual_package_linking_1
+ add_subdirectory("${OH_MODULES}/@react-native-ohos/react-native-mmkv/src/main/cpp" ./reactNativeMMKV)
# RNOH_END: manual_package_linking_1

# RNOH_BEGIN: manual_package_linking_2
+ target_link_libraries(rnoh_app PUBLIC rnoh_native_mmkv)
# RNOH_END: manual_package_linking_2
```

Open `entry/src/main/cpp/PackageProvider.cpp` and add the following:

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

###  2.4.Introducing the reactNativeMMKVPackage on the ArkTs side

打开 `entry/src/main/ets/RNPackagesFactory.ts`，add：


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

###  2.5.Run

Click the `sync` button in the top right corner.

Or execute in the terminal:

```bash
cd entry
ohpm install
```
Then, build and run the project.

## 3.  Constraints

### 3.1. Compatibility

The content of this document has been validated based on the following version:
1. RNOH:0.72.96; SDK: HarmonyOS 6.0.0 Release SDK; IDE: DevEco Studio  6.0.0.868; ROM: 6.0.0.112;
2. RNOH:0.77.18; SDK: HarmonyOS 6.0.0 Release SDK; IDE: DevEco Studio  6.0.0.868; ROM: 6.0.0.112;

## 4. Methods

> [!TIP] The **Platform** column indicates the platform where the properties are supported in the original third-party library.

> [!TIP] If the value of **HarmonyOS Support** is **yes**, it means that the HarmonyOS platform supports this property; **no** means the opposite; **partially** means some capabilities of this property are supported. The usage method is the same on different platforms and the effect is the same as that of iOS or Android.

| Name | Description | Type | Required | Platform | HarmonyOS Support |
| ---- | ----------- | ---- | -------- | -------- |-------------------|
| set      | Store key-value pair data | void | no | iOS/Android | yes             |
| getBoolean     | Obtain the boolean value                 | boolean \| undefined | no       | iOS/Android | yes               |
| getString          | Obtain the string value              | string \| undefined      | no       | iOS/Android | yes               |
| getNumber        | Obtain the number value              | number \|undefined                | no       | iOS/Android | yes               |
| getBuffer     | Obtain the buffer value  | ArrayBufferLike \| undefined          | no       | iOS/Android | yes               |
| contains | Determine whether mmkv contains a certain key value             | boolean                | no       | iOS/Android | yes               |
| delete      | Delete a key value from mmkv | void | no | iOS/Android | yes             |
| getAllKeys     | Get all key values                 | string[] | no       | iOS/Android | yes               |
| clearAll          | Delete all data             | void      | no       | iOS/Android | yes               |
| recrypt        | MMKV encryption settings             | void                | no       | iOS/Android | yes               |
| trim     | Mmkv cleaning storage content | void          | no       | iOS/Android | yes               |
| toString | Retrieve all key values and convert them to string values             | string                | no       | iOS/Android | yes               |
| addOnValueChangedListener      | Set up monitoring for data changes | Listener | no | iOS/Android | yes             |
| size     | Retrieve the file size stored in mmkv                 | number | no       | iOS/Android | yes               |
| isReadOnly          | set mmkv read-only             | boolean      | no       | iOS/Android | yes               |
| useMMKV        | Create MMKV using hook method             | MMKV                | no       | iOS/Android | yes               |
| useMMKVString     | Using hooks to store strings | MMKV          | no       | iOS/Android | yes               |
| useMMKVNumber | Using hooks to store number           | (    key: string,    instance?: MMKV  ): [value: T, setValue: (value: TSetAction) => void] | no       | iOS/Android | yes               |
| useMMKVBoolean      | Using hooks to store boolean | (    key: string,    instance?: MMKV  ): [value: T, setValue: (value: TSetAction) => void] | no | iOS/Android | yes             |
| useMMKVBuffer     | Using hooks to store buffer                 | (    key: string,    instance?: MMKV  ): [value: T, setValue: (value: TSetAction) => void] | no       | iOS/Android | yes               |
| useMMKVObject          | Using hooks to store object             | (    key: string,    instance?: MMKV  ): [value: T, setValue: (value: TSetAction) => void] | no       | iOS/Android | yes               |
| useMMKVListener        | Monitor data changes using hooks             | (    key: string,    instance?: MMKV  ): [value: T, setValue: (value: TSetAction) => void] | no       | iOS/Android | yes               |


## 5.Known Issues

## 6.Others

## 7.License
This project is licensed under [MIT License (MIT)](https://github.com/mrousavy/react-native-mmkv/blob/v3.3.0/LICENSE) .
