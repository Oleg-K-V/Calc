/*
������� �����������

������ �����:
������������ ����� ������������ � ����� 2007 �.
������������ ����� ������������ � ����� 2006 �.
������������ ����� ������������ � ����� 2004 �.
(bs@cs.tamu.edu) ������ 2004 �.

�� �������� ������ ������ ������� ������������.
��� � ����� cin, ���� � ���� cout.
*/
#include "stdafx.h"
#include <std_lib_facilities.h>
#include <math.h>

class Token //��������� ���� ��� �������� ������� �� �������
{
public:
	char kind;//���� �������
	double value;//���� �����
	string name;//����� ��������
	Token(char ch)//���������� ������� ��������� �������//
		:kind(ch), value(0){}
	Token(char ch, double val)//���������� ������� ��������� ����� � �������� '0'
		:kind(ch), value(val){}
	Token(char ch, string s)//���������� ������� ����� ��� ���� ���������� �� �������
		:kind(ch), name(s){}
};

class Token_stream
{
public:
	Token_stream();//�������� ������ Token_stream� ���������� � ������ cin
	Token get();//����������� �������
	void unget(Token t) { buffer=t; full=true; }//���������� �������
	void ignore(char ch);
private:
	bool full;// ������ �������
	Token buffer;//������ ������ �� ���� ��������
};

struct Variable//���������� ��������� �������
{
	string name;//���� ���� �����
	double value;//���� ���� ����� � ��������� ������
	Variable(string n, double v)//������� ���������������� ����� ���� 
		:name(n), value(v) { }
};

const string prompt ="> ";//���������� �����
const string result = "= ";//���������
const char let = 'L';//������� let
const char quit = 'Q';//�����
const char print = ';';//���������
const char number = '0';//������� �����
const char name = 'a';//������� �����
const string declkey ="#";//������� ���

Token_stream::Token_stream()//������� ������� ������
	:full (false), buffer(0){}

Token Token_stream::get()//������� ���������� ������� � ������ cin
{
	if (full)//��������� �� ����� ������
	{//���� ������
		full=false;//������ ������
		return buffer;//�������� ���� ������
	}//���� ����� ������
	char ch;
	cin>>ch;//������� ������ � ������
	switch(ch)//��������� ������ �� ��������� � ��������:
	{
	case quit://�� ������� ������?
	case print://�� ������� ��������� ���������� ���������?
	case 'k':
	case 'p':
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case '{':
	case '}':
	case '!':
	case '=':
	case '^':
		return Token(ch);//���� ������ ������� ������� � ���� ��������� ������ ���� �� ���������
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		{
			cin.unget();//�������� ����� � ����
			double val;
			cin>>val;//������� ����� � ��������� ������
			return Token(number,val);//�������� �����
		}

	default:
		if (/*(isalpha(ch))|| */(ch='#'))//��������� �� �������� ������ � ������
		{
			string s;
			s+=ch;//����� �� ����� s �������� ������
			while (cin.get(ch) && ((isalpha(ch) || isdigit(ch)) || ch=='_'))//������� �������� ����� � ��������� �� �������� ������ � ������ ��� ������
			{
				//if ((isalpha(ch) || isdigit(ch)) || ch!='_') break;
				s+=ch;//����� �� ����� s �������� ������
			}
			cin.unget();//��������� ������ � ���� cin
			if (s==declkey)//���������� �� ������� ����� �� � �������� ������
				return Token(let);//�������� let	
			return Token(name, s);//��������� ����� s � ��������� ���� �� ������� �����
		}
		error("nepravelnii sumvol");//��������� ����*
	}
}

void Token_stream::ignore(char c)
{
	if (full && c==buffer.kind)//�������� ���������� �����
	{//���� �� ������� ������ �������
		full=false;//������ �����
		return;
	}
	full=false;//������� �����
	char ch=0;
	while (cin>>ch)//������� �������
		if (ch==c)//���� ������ ������� ��������
			return;
}

vector<Variable> names;//���������� ������� ���� ���� ������ ������ ����	
Token_stream ts; //���� ���� ����� ����
Token get_token();
bool faktor=false;
double expression();
double term();
double primary();
double fakt(double zm);
vector<Token> tok;

double get_value(string s)//������� ������ ������� ���� � �����
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) return names[i].value;//��������� �� �-�� ���� = �������
	error("get: undefined name 1",s);//���� �� ������� ����� �������
}

void set_value(string s, double d)//������� ������ �������� ���� � ������
{
	for (int i = 0; i<=names.size(); ++i)
		if (names[i].name == s)//����� ������� ����� 
		{//���� �������
			names[i].value = d;//����������
			return;
		}
	error("set: undefined name 2",s);//���� �� ������� �������� �������
}

bool is_declared(string s)//������� �������� �� � ���� s � �����
{
	for (int i=0;i<names.size();++i)
		if (names[i].name==s)//��������� �� � ���� s � ����� 
			return true;//���� � �� ������ ���
	return false;//���� ���� ������ �
}

double define_name(string s,double d)//��������� ����� � ����� �����
{
	if (is_declared(s))//��������� �� ��� ������� ���� �?
		error(s ," Wedeno dvichi");//������� ������� � ����������� ���������
	names.push_back(Variable(s,d));//�������� � �����
	return d;
}

double declaration()
{
	Token t = ts.get();
	if (t.kind != name) error ("name expected in declaration");
		string name = t.name;
	if (is_declared(name)) 
		error ("declared twice");//��������� �� ��� ������� name
	Token t2 = ts.get();
	if (t2.kind != '=') 
		error("= missing in declaration of " ,name);//��������� �� ���� ������ =
	double d = expression();
	names.push_back(Variable(name,d));//define_name(name,d);//������� ���� ��� � ������
	return d;
}

double statement()
{
	Token t=ts.get();//������� ��������� ������
	switch(t.kind) 
	{
	case let://���� ���� �����
		return declaration();//�������� � �����
	default://� ������ ������� ��������� � ����
		ts.unget(t);//��������� � ����
		return expression();//��������� ������� expression
	}
}

void clean_up_mess()
{
	ts.ignore(print);//� ��� � ���� �� ������, ��� ���� ��� ����
}

void calculate()
{
	while (cin)
	try
	{
		cout<<prompt;//�������� ���������� �����
		Token t=ts.get();//������� ������ �������
		while (t.kind==print)//���������� �� �� ������� ������� ��������� 
			t=ts.get();//������� ��������� ������
		if (t.kind==quit)//���������� �� �� �� ������ ������ � �������� 
			return; 
		ts.unget(t);//��������� ������ ����� � ����
		cout<<result<<statement()<<endl;//�������� ��������� ���������
	}
	catch (runtime_error& e)//�������� ��������� ��������
	{
		cerr<<e.what()<<endl;//������ �������
		clean_up_mess();//������� �������
	}
}

int _tmain(int argc, _TCHAR* argv[])
try
{
	define_name("pi", 3.1415926535);//�������� ����� ϳ
	define_name("e", 2.7182818284);//�������� ����� �
	/*cout<<"Laskavo procumo do programu CALKULATOR\n"
		<<"Dla togo shcod buv vuvedenu resultat vvedit ';'"
		<<"\nDla vuhodu natusnit enter a potim 'Q'"<<endl;//�������� ��������� ����������*/
	calculate();//�������� ������� �������
	return 0;
}
catch (exception& e)
{
	cerr<<"exception: "<<e.what()<< endl;
	char c;
	while (cin>>c && c!=';');
	return 1;
}
catch (...)
{
	cerr<<"exception\n";
	char c;
	while (cin>>c && c!=';');
	return 2;
}

double expression()
{
	double left=term();//������� ��������� �������� � ������� term
	while (true)//��������� ����
	{
		Token t=ts.get();//������� ������� �������� 
		switch(t.kind)//���������� 
		{
		case '+'://���� +
			left+=term();
			break;//�������� ��������
		case '-'://���� -
			left-=term();
			break;//�������� ��������
		default://���� ������ �� ��������
			ts.unget(t);//��������� �������� � ����
			return left;//��������� ���������
		}
	}
}

double term()
{
	double left=primary();//�������� ����������� �������� � ������� primary
	while (true)//���������� ����
	{
		Token t=ts.get();
		switch(t.kind)//��������� ���� ������� �������:
		{
		case '*'://���� *
			left*=primary();//������ ��������� �� ��������
			//t=ts.get();//������� ���������
			break;//�������� ��������
		case '/':
			{//���� /
				double d=primary();//������� �������� �����
				if (d==0)//��������� �� �� 0 
					error(" NA 0 !mogjna");//���� ��� ������ �������
				left/=d;//������� ������
				//t=ts.get();//������� ���������
				break;//������� ��������
			}
		case '%':
			{//���� %
				int i1=narrow_cast<int>(left);//��������� ��������� �� ���� � ����� ������
				int i2=narrow_cast<int>(primary());//��������� �������� �� ���� � ����� ������
				if (i2==0)//���������� �� �������� �� � ����� 
					error(" NA 0 !mogjna");//���� ���� ������� �������
				left=i1%i2;//���������� ������ � ������
				//t=ts.get();//������� �������� �����
				break;//�������� ��������
			}
		case '!':
			{//���� �������� 
				double zm1=left;//������� ����� 
				left=fakt(zm1);//��������� ������� ���������
				//t=ts.get();//������� �������� �������
				break;//�������� ��������
			}
		default://���� �� �������� 
			ts.unget(t);//��������� ������ � ����
			return left;//�������� �������� �����
		}
	}
}

double primary()
{
	Token t=ts.get();//������� �������
	switch (t.kind)//��������� �� ��:
	{
	case '(':
		{//���� �� '('
			double d=expression();//���������� ������� ��������� ������ � ������
			t=ts.get();
			if (t.kind!=')')//���� �� != ')'
				error("')' expected");//������� �������
			return d;
		}
	case 'p':
		{
			t=ts.get();
			if (t.kind!='{')//���� �� != '{'
				error("'{' expected");//������� �������
			double d=expression();//���������� ������� ��������� ������ � ������
			t=ts.get();
			if (t.kind!='^')//���� �� != ','
				error("'^' expected");//������� �������*/
			double p=expression();//���������� ������� ��������� ������ � ������
			t=ts.get();
			if (t.kind!='}')//���� �� != '}'
				error("'}' expected");//������� �������
			if (p<=0)//���������� �� �������� �� ����� ���� ���� 
				error(" NA 0 !mogjna");//���� ���� ������� �������
			return pow(d, p);
		}
	/*case '{':
		{//���� �� '('
			double d=expression();//���������� ������� ��������� ������ � ������
			t=ts.get();
			if (t.kind!='}')//���� �� != ')'
				error("')' expected");//������� �������
			return d;
		}*/
	case 'k':
		{
			double d=expression();
			if (d<=0)//���������� �� �������� �� ����� ���� ���� 
				error(" NA 0 !mogjna");//���� ���� ������� �������
			return sqrt(d);
		}
	case number:
		return t.value;//��������� �����
	case '-': 
		return -primary();//�������� ������� primary � ������ ����
	case '+': 
		return primary();//�������� ������� primary
	case name:
		return get_value(t.name);
	default:
		error("primary expected");//������ �������
	}
}

double fakt(double Z)
{	
	int F=int(Z);
	if (F==0) return 1;		
	else 
	{ 
		int R=F-1;
		double J=F*fakt(R);
		return J;
	}
}
