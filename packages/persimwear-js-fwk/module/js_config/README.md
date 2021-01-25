# JS 文件打包说明

## 准备工作
### 安装 nodejs
JS 压缩打包功能依赖于 nodejs 及其相关工具。所以我们必须先安装 nodejs
下载地址： https://nodejs.org/zh-cn/

下载安装完成后可以打开命令行工具输入 node -v 来测试一下是否安装成功

### 安装 rollup.js
rollup 就是我们要使用的打包工具，下面来安装它，

nodejs 安装完成后，在命令行中输入 npm install --global rollup 来全局安装 rollup

在安装完成后同样可以使用 rollup -v 来测试是否安装成功

### 安装打包插件及相关依赖
在命令行中进入 persimwear-js-fwk/module/js_config ，这个目录是我们存放各种配置文件的地方

进入该目录后输入 npm install ，npm 会读取 package.json 文件的内容，自动帮我们安装好需要的依赖文件
依赖文件都装完以后就可以进行打包了


## JS 文件打包
```json
"scripts": {
  "dev-mvvm": "rollup --config mvvm.config.js --environment NODE_ENV:development",
  "build-mvvm": "rollup --config mvvm.config.js --environment NODE_ENV:production",
  "dev-sensor": "rollup --config sensor.config.js --environment NODE_ENV:development",
  "build-sensor": "rollup --config sensor.config.js --environment NODE_ENV:production",
  "dev-hardware": "rollup --config hardware.config.js --environment NODE_ENV:development",
  "build-hardware": "rollup --config hardware.config.js --environment NODE_ENV:production"
  },
```

在 package.json 中有如上的一段代码，这里的内容就是我们在打包时需要执行的命令。可以看到上面的命令分为两种，分别为 dev 和 build

- dev 表示为开发环境，用这种命令打出的文件内容不会压缩和混淆，方便调试
- build 表示为生产环境，用这种命令打出的文件内容会被压缩和混淆，适用于生产环境

在 dev 或 build 后面跟着的是要打包的模块，比如 dev-mvvm 命令就是给 mvvm 模块打开发环境的包

### 打包
1. 在命令行中进入 persimwear-js-fwk/module/js_config 文件夹
2. 输入相应的打包命令，比如要打 dev-mvvm 包就输入 npm run dev-mvvm

执行完上面两步，一切正常的话，打包好的文件会在相应模块内的 dist 文件夹内，其中：
- npm run dev-mvvm: 打包出的文件为 mvvm.unzip.js
- npm run build-mvvm: 打包出的文件为 mvvm.js
