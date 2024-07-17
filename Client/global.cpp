#include<global.h>

QString gate_url_prefix="";
//polish实现
std::function<void(QWidget*)> repolish = [](QWidget* w){
    w->style()->unpolish(w);
    w->style()->polish(w);//卸掉样式重新刷新
};

std::function<QString(QString)> xorString = [](QString input){

    QString result = input; //复制源字符串，以便进行修改
    int length = input.length(); //获取字符串的长度
    length = length%255; //length是字节的倍数,2^8 = 256
    for(int i=0;i<length;++i)
    {
        //对每个字符进行异或操作
        //注意：这里假设字符都是ASCII，因此直接转为QChar
        result[i] = QChar(static_cast<ushort>(input[i].unicode()^ static_cast<ushort>(length)));
    }
    return result;
};
