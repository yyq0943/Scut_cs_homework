#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

string key[17]  = {"true","false","or","and","int","bool","string","while","do","if","then","else","end","repeat","until","read","weite"};


//判断字符是否为字母
bool isLetter(char t)
{
	if((t>='a'&&t<='z')||(t>='A'&&t<='Z'))
		return true;
	else 
		return false;
}

//判断字符是否为数字
bool isNumber(char t)
{
	if(t>='0'&&t<='9')
		return true;
	else 
		return false;
}

//判断字符是否为特殊操作符
bool isSYM(char t)
{
	if(t==','||t==':'||t==';'||t=='<'||t=='>'||t=='='||t=='+'||t=='-'||t=='*'||t=='/'||t=='('||t==')')
	return true;
	else 
		return false;
}

bool isKEY(string t)
{
	//cout<<t<<endl;
	if(t.compare("true")==0||t=="false"||t=="or"||t=="and"||t=="not"||t=="int"||t=="bool"||t=="string"||t=="while"||t=="do"||t=="if"||t=="then"||t=="else"||t=="end"||t=="repeat"||t=="until"||t=="read"||t=="write")
	{//cout << "KEy";
		return true;}
	else
		return false;
}
bool isKongzifu(char a)
{
	if(a==0x20||a==0x09||a==0x0B||a==0x0C||a==0x0D||a==0x0A||a==EOF)
		return true;
	else
		return false;
}
//判断是否为非法字符
bool isUnlawful(char a)
{
	char specialChar[15]={ ',', ';', ':', '<', '>', '=', '+', '-', '*', '/', '(', ')', '{', '}', '\'' };
	for( int j=0;j<strlen(specialChar);j++ )
	{
		if( a==specialChar[j] )
		{
			return false;
		}
	}
	if(isalpha(a) || isdigit(a))
		return false;
	return true;
}

//判断是否为空格，回车等等..
bool isSpacess(char a)
{
	char spacesChar[6]={ 0x20, 0x09, 0x0B, 0x0C, 0x0D, 0x0A};
	for( int j=0;j<strlen(spacesChar);j++ )
	{
		if( a==spacesChar[j] )
		{
			return true;
		}
	}
	return false;
}

bool isdanyihao();
bool isString();

int main()
{
	int i=1;
	bool isRight=true;
	char a;//记录一个字符
	string str;//收集每一个合法的字符
	ifstream in("in.txt",ios::in);
	ofstream out("out.txt",ios::out);
	a=in.get();
	
	while((in.peek()!=EOF)&&i++)
	{
		while(isSpacess(a))
			a=in.get();
		if(i%5==0)out<<endl;

	    //第一个为非法字符
		if(isUnlawful(a))
		{
			str=a;
			while( (!isSpacess(a)) && !in.eof())
			{
				a=in.get();
				if(!isSpacess(a))
					str+=a;
			}
			out<<"(WRO, " <<str<<" )      ";
			in.seekg(-1,ios::cur);
		}

				//第一个字符为单引号
		if(a=='\'')
		{
			str=a;
			int mark=0;
			char b=a;
			a=in.get();
			while( (!isSpacess(a)) &&!in.eof())
			{
				if(a=='\'')
				{
					str+=a;
					mark=1;
					b=a;
					if( isSpacess( a=in.get() ) || in.eof())
					{
						//遇到右边的单引号且接下来是空格，判定为字符串
						
						out <<"(STR , "<<str<<")     ";
						in.seekg(-1,ios::cur);
						break;
					}
					else
					{
						while( (!isSpacess(a)) && !in.eof())
						{
							str+=a;
							a=in.get();					
						}
						//遇到右单引号后又继续有其他字符而没有以空格结束判定为非法标识符
						out<<"(WRO, " <<str<<" )      ";
						in.seekg(-1,ios::cur);
						break;
					}
				}
				str+=a;
				a=in.get();
			}
			//没有遇到右单引号就遇到空格结束了
			if(mark==0)
			{
				in.seekg(-1,ios::cur);
				out<<"(WRO, " <<str<<" )      ";
			}
		}
		
		//第一个为注释符
		if(a=='{')
		{
			while((((a=in.get()) != '}')||(isKongzifu(a)))&&(!in.eof()));
			if(a=='}')
				out<<"(注释)"<<endl;
			else
				out<<"错误"<<endl;
			// 
		}
		//letter
		if(isLetter(a))
		{
			
			str=a;
			while(a=in.get())
			{
				//cout<<a;
				if (isLetter(a)||isNumber(a)){str+=a;}
		        else if(isKongzifu(a))
				{
					//cout<<str;
					if(isKEY(str))
				       out<<left<<setw(7)<<"(KEY ，"<<setw(9)<<str<<setw(1)<<")";
			        else 
				       out<<left<<setw(7)<<"(ID ，"<<setw(9)<<str<<setw(1)<<")";
					in.seekg(-1,ios::cur);
				    break;
				}
				else if(a=='}')out<<"( 缺失{ )";
				else if(a=='\'')out<<"( 缺失左‘ )";
				else
				{
					out<<left<<setw(17)<<"(WROKEY&ID )";
					break;
				}

			}
			
		}

		//number
		if(isNumber(a))
		{
			str=a;
			while(a=in.get())
			{
				if(isNumber(a))str+=a;
				else if ((isKongzifu(a))||(isSYM(a)))
				{
					out<<left<<setw(7)<<"(NUM ,"<<setw(9)<<str<<setw(1)<<")";
					in.seekg(-1,ios::cur);
					break;
				}
				else
				{
					str+=a;
					while(a=in.get()&&(!in.eof()))
					{
						if(isKongzifu(a)==false)
							str+=a;
					}

					out<<left<<setw(17)<<"(WRONUM "<<str<<")";
					in.seekg(-1,ios::cur);
					break;
				}
			}

		}
		//sym
		if(isSYM(a))
		{
			str=a;
			while(a=in.get())
			{
				if(isSYM(a))str+=a;
				else if((isKongzifu(a))||(isLetter(a))||(isNumber(a)))
				{
					out<<left<<setw(7)<<"(SYM ,"<<setw(9)<<str<<setw(1)<<")";
					in.seekg(-1,ios::cur);
					break;
				}
				else
				{
					out<<left<<setw(17)<<"(WROSYM )";
				}
			}
		}
		/*
		if(a=='{')
		{
			while(!in.eof()&&(a=in.get()))
			{
				if(a=='}')
					isRight=true;
				else
					isRight=false;
			}
			if(isRight)
				out<<left<<setw(16)<<"{注释内容"<<setw(1)<<"}"<<endl;
			else
				out<<left<<setw(16)<<"{缺少‘}’"<<setw(1)<<"}"<<endl;
		}
		*/
			
		//string+单引号
		//非法字符
		//注释
		//a=in.get();
		in>>a;
	}



}