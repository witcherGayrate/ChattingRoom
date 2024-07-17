//加载需要的库
const path = require('path')
const grpc = require('@grpc/grpc-js')
const protoLoader = require('@grpc/proto-loader')

//message.proto文件全路径,_dirname为当前文件目录
const PROTO_PATH = path.join(__dirname,'message.proto')

//protoloader根据参数设置读取proto文件的package定义返回packagedefinition
const packageDefinition = protoLoader.loadSync(PROTO_PATH,{keepCase:true,longs:String,
  enums:String,defaults:true,oneofs:true })

//grpc解析package
const protoDescriptor = grpc.loadPackageDefinition(packageDefinition)

const message_proto = protoDescriptor.message
//导出解析类供其他文件访问
module.exports = message_proto

