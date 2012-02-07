/*
Простий калькулятор

Исторія версій:
Перероблений Бярне Страуструпом в травні 2007 р.
Перероблений Бярне Страуструпом в серпні 2006 р.
Перероблений Бярне Страуструпом в серпні 2004 р.
(bs@cs.tamu.edu) Весною 2004 р.

Ця програма виконує основні функції калькулятора.
Ввід в поток cin, вивід в потік cout.
*/
#include "stdafx.h"
#include <std_lib_facilities.h>
#include <math.h>

class Token //оголошуємо клас для розподіли символів на лексеми
{
public:
	char kind;//зміна символів
	double value;//зміна чисел
	string name;//змінна констант
	Token(char ch)//оголошення функції присвоєння символу//
		:kind(ch), value(0){}
	Token(char ch, double val)//оголошення функції присвоєння числа з символом '0'
		:kind(ch), value(val){}
	Token(char ch, string s)//оголошення функції назви яук задає користувач до символа
		:kind(ch), name(s){}
};

class Token_stream
{
public:
	Token_stream();//стврення потоку Token_streamб зчитування з потоку cin
	Token get();//Зчитучвання лексеми
	void unget(Token t) { buffer=t; full=true; }//Повернення лексеми
	void ignore(char ch);
private:
	bool full;// флажок буффера
	Token buffer;//буффер лексем які були повернені
};

struct Variable//оголошення структури варанти
{
	string name;//зміна типу стрінг
	double value;//зміна типу число з плавоючою точкою
	Variable(string n, double v)//функція отримуванняданиїх даних пипів 
		:name(n), value(v) { }
};

const string prompt ="> ";//Запрошення вводу
const string result = "= ";//Рузультат
const char let = 'L';//Лексема let
const char quit = 'Q';//ВИхід
const char print = ';';//Результат
const char number = '0';//Лексема число
const char name = 'a';//Лексема назки
const string declkey ="#";//Лексема лет

Token_stream::Token_stream()//Функція очистки буфера
	:full (false), buffer(0){}

Token Token_stream::get()//функція зчитування символу з потоку cin
{
	if (full)//перевіряєм чи буфер повний
	{//якщо повний
		full=false;//очищаєм флажок
		return buffer;//повертаєм вміст буфера
	}//якщо буфер пустий
	char ch;
	cin>>ch;//зчитуємо символ з потоку
	switch(ch)//перевіряєм символ на співпадіння з символом:
	{
	case quit://це лексема виходу?
	case print://це лексема виведення результату обрахунків?
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
		return Token(ch);//якщо символ дорівнює символу з вище наведених видати його як результат
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
			cin.unget();//повертаєм чифру в потік
			double val;
			cin>>val;//зчитуємо число з плавуючою точкою
			return Token(number,val);//повертаєм число
		}

	default:
		if (/*(isalpha(ch))|| */(ch='#'))//перевіряєм чи зчитаний сивлол є буквою
		{
			string s;
			s+=ch;//додаєм до слова s зчитаний символ
			while (cin.get(ch) && ((isalpha(ch) || isdigit(ch)) || ch=='_'))//зчитуємо наступне число і перевіряєм чи зчитаний символ є буквою або числом
			{
				//if ((isalpha(ch) || isdigit(ch)) || ch!='_') break;
				s+=ch;//додаєм до слова s зчитаний символ
			}
			cin.unget();//повертаємо символ в потік cin
			if (s==declkey)//перевіряємо чи зчитане слово не є ключовим словом
				return Token(let);//повертаєм let	
			return Token(name, s);//повертаємо слово s і позначаємо його як ключове слово
		}
		error("nepravelnii sumvol");//виняткова подія*
	}
}

void Token_stream::ignore(char c)
{
	if (full && c==buffer.kind)//Зпочатку перевіряємо буфер
	{//якщо він дорівнює нашому символу
		full=false;//очищаєм буфер
		return;
	}
	full=false;//очищаємо буфер
	char ch=0;
	while (cin>>ch)//зчитуємо символи
		if (ch==c)//якщо символ дорівнює заданому
			return;
}

vector<Variable> names;//Оголошення вектора який буде вміщати введені змінні	
Token_stream ts; //Зміна типу токен стрім
Token get_token();
bool faktor=false;
double expression();
double term();
double primary();
double fakt(double zm);
vector<Token> tok;

double get_value(string s)//функція пошоку введеної зміної в масиві
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) return names[i].value;//перевіряєм чи і-та зміна = введеній
	error("get: undefined name 1",s);//якщо не знайшли видаєм помилку
}

void set_value(string s, double d)//Функція запису значення зміної в зміннну
{
	for (int i = 0; i<=names.size(); ++i)
		if (names[i].name == s)//пошук потрібної змінної 
		{//якщо знайшли
			names[i].value = d;//присвоюємо
			return;
		}
	error("set: undefined name 2",s);//якщо не знайшли виводимо помилку
}

bool is_declared(string s)//Функція перевірки чи є зміна s в масиві
{
	for (int i=0;i<names.size();++i)
		if (names[i].name==s)//перевіряєм чи є зміна s в масиві 
			return true;//якщо є то вертаєм так
	return false;//якщо немає вертаєм ні
}

double define_name(string s,double d)//добавляєм змінну в масив зміних
{
	if (is_declared(s))//перевіряєм чи вже введена зміна є?
		error(s ," Wedeno dvichi");//виводим помилку в позитивному результаті
	names.push_back(Variable(s,d));//записуємо в масив
	return d;
}

double declaration()
{
	Token t = ts.get();
	if (t.kind != name) error ("name expected in declaration");
		string name = t.name;
	if (is_declared(name)) 
		error ("declared twice");//перевіряєм чи був ведений name
	Token t2 = ts.get();
	if (t2.kind != '=') 
		error("= missing in declaration of " ,name);//перевіряєм чи було ведено =
	double d = expression();
	names.push_back(Variable(name,d));//define_name(name,d);//записуєм нову міну в вектор
	return d;
}

double statement()
{
	Token t=ts.get();//зчитуємо наступний символ
	switch(t.kind) 
	{
	case let://якщо нова змінна
		return declaration();//записуємо в масив
	default://в іншому випадку повернути в потік
		ts.unget(t);//повертаємо в потік
		return expression();//повертаємо функцію expression
	}
}

void clean_up_mess()
{
	ts.ignore(print);//з чим я шось не вшарив, іне знаю нах воно
}

void calculate()
{
	while (cin)
	try
	{
		cout<<prompt;//Виводимо запрошення вводу
		Token t=ts.get();//зчитуємо перший елемент
		while (t.kind==print)//Перевіряємо чи не потрібно вивести результат 
			t=ts.get();//зчитуємо наступний символ
		if (t.kind==quit)//перевіряємо чи це не символ виходу з програми 
			return; 
		ts.unget(t);//повертаємо символ назад в потік
		cout<<result<<statement()<<endl;//виводимо результат обрахунків
	}
	catch (runtime_error& e)//обробник виключних ситуацій
	{
		cerr<<e.what()<<endl;//виводм помилку
		clean_up_mess();//ігнорим помилку
	}
}

int _tmain(int argc, _TCHAR* argv[])
try
{
	define_name("pi", 3.1415926535);//записуємо змінну Пі
	define_name("e", 2.7182818284);//записуємо змінну е
	/*cout<<"Laskavo procumo do programu CALKULATOR\n"
		<<"Dla togo shcod buv vuvedenu resultat vvedit ';'"
		<<"\nDla vuhodu natusnit enter a potim 'Q'"<<endl;//записуємо початкове запрошення*/
	calculate();//визиваємо функцію початку
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
	double left=term();//зчитуємо попередне значення з функції term
	while (true)//безкінецній цикл
	{
		Token t=ts.get();//зчитуємо настпне значення 
		switch(t.kind)//перевіряємо 
		{
		case '+'://якщо +
			left+=term();
			break;//закінчуємо перевірку
		case '-'://якщо -
			left-=term();
			break;//закінчуємо перевірку
		default://якщо символ не знайдено
			ts.unget(t);//повертаємо значення в потік
			return left;//повертаємо результат
		}
	}
}

double term()
{
	double left=primary();//зчитужмо поперередне значення з функції primary
	while (true)//безкінечний цикл
	{
		Token t=ts.get();
		switch(t.kind)//перевіряєм чому дорівнює лексема:
		{
		case '*'://якщо *
			left*=primary();//множим попередне на наступне
			//t=ts.get();//зчитуємо наступний
			break;//закінчуємо перевірку
		case '/':
			{//якщо /
				double d=primary();//зчитуємо наступне число
				if (d==0)//перевіряєм чи не 0 
					error(" NA 0 !mogjna");//якщо так виводм помилку
				left/=d;//виконуєм ділення
				//t=ts.get();//зчитуємо наступний
				break;//закінчуєм перевірку
			}
		case '%':
			{//якщо %
				int i1=narrow_cast<int>(left);//перевіряєм попередне чи воно є цілим числом
				int i2=narrow_cast<int>(primary());//перевіряєм наступне чи воно є цілим числом
				if (i2==0)//перевіряємо чи наступне не є нулем 
					error(" NA 0 !mogjna");//якщо нуль виводим помилку
				left=i1%i2;//вираховуємо остачу з ділення
				//t=ts.get();//зчитуємо наступне число
				break;//закінчуємо перевірку
			}
		case '!':
			{//якщо факторіал 
				double zm1=left;//зчитуємо число 
				left=fakt(zm1);//викликаємо функцію факторіала
				//t=ts.get();//зчитуємо наступну лексему
				break;//закінчуємо перевірку
			}
		default://якщо не знайдено 
			ts.unget(t);//повертаємо символ в потік
			return left;//повертаєм наступне число
		}
	}
}

double primary()
{
	Token t=ts.get();//зчитуємо лексему
	switch (t.kind)//перевіряєм чи це:
	{
	case '(':
		{//якщо це '('
			double d=expression();//викликаємео функцію обрахунків виразу в дужках
			t=ts.get();
			if (t.kind!=')')//якщо це != ')'
				error("')' expected");//виводим помилку
			return d;
		}
	case 'p':
		{
			t=ts.get();
			if (t.kind!='{')//якщо це != '{'
				error("'{' expected");//виводим помилку
			double d=expression();//викликаємео функцію обрахунків виразу в дужках
			t=ts.get();
			if (t.kind!='^')//якщо це != ','
				error("'^' expected");//виводим помилку*/
			double p=expression();//викликаємео функцію обрахунків виразу в дужках
			t=ts.get();
			if (t.kind!='}')//якщо це != '}'
				error("'}' expected");//виводим помилку
			if (p<=0)//перевіряємо чи наступне не менше рівне нулю 
				error(" NA 0 !mogjna");//якщо нуль виводим помилку
			return pow(d, p);
		}
	/*case '{':
		{//якщо це '('
			double d=expression();//викликаємео функцію обрахунків виразу в дужках
			t=ts.get();
			if (t.kind!='}')//якщо це != ')'
				error("')' expected");//виводим помилку
			return d;
		}*/
	case 'k':
		{
			double d=expression();
			if (d<=0)//перевіряємо чи наступне не менше рівне нулю 
				error(" NA 0 !mogjna");//якщо нуль виводим помилку
			return sqrt(d);
		}
	case number:
		return t.value;//повертаємо число
	case '-': 
		return -primary();//повертаєм функцію primary зі знаком мінус
	case '+': 
		return primary();//повертаєм функцію primary
	case name:
		return get_value(t.name);
	default:
		error("primary expected");//виводм помилку
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
