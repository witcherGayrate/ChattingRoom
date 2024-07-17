const nodemailer = require('nodemailer');
const config_module = require("./config");

/*
创建发送邮件的代理
*/

let transport = nodemailer.createTransport({
    host:'smtp.163.com',
    port:465,//邮箱服务器端口
    secure:true,
    auth:{
        user: config_module.email_user,//发送方邮箱地址
        pass:config_module.email_pass//邮箱授权码或者密码
    }
});

/*
*发送邮件的函数
* @param {*} mailOptions_ 发送邮件的参数
* @returns
*/
function SendMail(mailOptions_)
{
    //外部封装Promise，把异步回调变成阻塞等待的效果,resolve是正确情况下的return，reject是错误情况
    return new Promise(function(resolve,reject){
        transport.sendMail(mailOptions_,function(error,info)
    {
        if(error)
            {
                console.log(error);
                reject(error);
            }else{
                console.log('邮件已成功发送：'+ info.response);
                resolve(info.response)
            }
    });
    })
}
//抛出API
module.exports.SendMail = SendMail