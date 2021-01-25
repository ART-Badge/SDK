import {uglify} from 'rollup-plugin-uglify'
import commonjs from "@rollup/plugin-commonjs"
import nodeResolve from "@rollup/plugin-node-resolve"

let isProd = process.env.NODE_ENV === 'production'

// 通用的插件
const basePlugins = [nodeResolve(),commonjs(),]
// 开发环境需要使用的插件
const devPlugins = []
// 生产环境需要使用的插件
const prodPlugins = [uglify()]

let plugins = [...basePlugins].concat(isProd ? prodPlugins : devPlugins)
let outputPath = isProd ? '../hardware/js/dist/hardware.js': '../hardware/js/dist/hardware.unzip.js'

export default {
  input: '../hardware/js/src/hardware.js',
  output: {
    file: outputPath,
    format: 'cjs',
    name: 'hardware'
  },
  plugins: plugins
}
