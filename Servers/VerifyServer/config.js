const fs = require('fs');//加载文件读写库
let config = JSON.parse(fs.readFileSync('config.json', 'utf8'));//加载json配置文件并解析
let email_user = config.email.user;//读取email的user值
let email_pass = config.email.pass;
let mysql_host = config.mysql.host;
let mysql_port = config.mysql.port;
let mysql_passwd = config.mysql.passwd;
let redis_host = config.redis.host;
let redis_port = config.redis.port;
let redis_passwd = config.redis.passwd;
let code_prefix = "code_";

module.exports = {email_pass, email_user, mysql_host, mysql_port,mysql_passwd,redis_host, redis_port, redis_passwd, code_prefix}