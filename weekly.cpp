#include<iostream>
#include<wInDoWs.H>
#include<ctime>
using namespace std;


class Heart
{
public:

	Heart();
	int Execute();
	enum enum_key
	{
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		EXIT,
	};
private:
	enum_key Pressed();
	enum_key m_Key;
	void Initialize();
	bool m_bGo;
	void Update();
	const static int sm_iInterval = 50;
	void CursorTo(short x, short y);
	const static int sm_iScreenX = 84, sm_iScreenY = 22;
	static bool m_ggbGrid[sm_iScreenY][sm_iScreenX];//记录整个显示区域当前状态
	static bool m_ggbGridSave[sm_iScreenY][sm_iScreenX];//记录上一帧状态
	int m_iX, m_iY;//记录左上角点的位置
	const static int sm_iAreaX = 29, sm_iAreaY = 11;
};

bool Heart::m_ggbGrid[sm_iScreenY][sm_iScreenX] = {
{ 0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1 },
{ 0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1 },
{ 0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},
{ 0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
};

bool Heart::m_ggbGridSave[sm_iScreenY][sm_iScreenX] = {};

Heart::enum_key Heart::Pressed()
{
	enum_key key = NONE;
	if (GetKeyState('S') < 0 || GetKeyState(VK_DOWN) < 0)
		key = DOWN;
	else if (GetKeyState('W') < 0 || GetKeyState(VK_UP) < 0)
		key = UP;
	else if (GetKeyState('A') < 0 || GetKeyState(VK_LEFT) < 0)
		key = LEFT;
	else if (GetKeyState('D') < 0 || GetKeyState(VK_RIGHT) < 0)
		key = RIGHT;
	else if (GetKeyState(VK_ESCAPE))
		key = EXIT;
	return key;
}

void Heart::Initialize()
{
	//获取标准输出句柄
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//设置字色（亮白色背景亮红色前景）
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_RED |
		BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
	//设定窗口全屏显示
	SetConsoleDisplayMode(hConsole, 1, NULL);
	//声明一个记录窗口缓冲信息的变量并用它获取信息
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(hConsole, &bInfo);
	//设定新的窗口大小和窗口信息
	SetConsoleScreenBufferSize(hConsole, { 100,100 });
	SetConsoleWindowInfo(hConsole, true, &SMALL_RECT({ 0,0,99,99 }));
	//储存光标信息并获取
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hConsole, &CursorInfo);
	//设定光标不可见，并将新的信息改为标准输出设置信息
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(hConsole, &CursorInfo);
	//设定新的字体大小
	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi),0,0,44,FF_DONTCARE,FW_NORMAL,NULL };
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
	cout << R"(**************************************************************************************
*      *******   *******                                                             *
*    ********** **********                                                           *
*  *************************                                                         *
******************************                                                       *
*  *************************                                                         *
*    *********************                                                           *
*      *****************                                                             *
*        *************                                                               *
*          *********                                                                 *
*            *****                                                                   *
*              *                                                                     *
*                                                                                    *
*                                                                                    *
*                                                                                    *
*                                                                                    *
*                                                                                    *
*                                                                                    *
*                                                                                    *
*                                                                                    *
*                                                                                    *
*                                                                                    *
*                                                                                    *
**************************************************************************************
)";
}


Heart::Heart() :m_bGo(true),m_iX(0),m_iY(0),m_Key(NONE)
{
	memcpy(m_ggbGridSave, m_ggbGrid, sizeof(m_ggbGrid));
}

int Heart::Execute()
{
	Initialize();
	while (m_bGo)
	{
		auto finish = clock() + sm_iInterval;
		while ((m_Key=Pressed())==NONE&&finish > clock())
		{
			Sleep(1);
		}
		if ((finish -= clock()) > 0)
			Sleep(finish);
		Update();
	}
	return 0;
}

void Heart::Update()
{
	//不动/上/下/左/右
	static int dx[] = {0,0,0,-1,1}, dy[] = {0,-1,1,0,0};
	if(m_Key)
	{
		if (m_Key == EXIT)
		{
			m_bGo = false;
			return;
		}
		int x = m_iX + dx[m_Key], y = m_iY + dy[m_Key];
		//检查是否到达边界
		if (x<0 || x>sm_iScreenX-sm_iAreaX || y<0 || y>sm_iScreenY-sm_iAreaY)
			return;
		memset(m_ggbGrid, 0, sizeof(m_ggbGrid));
		for (int i = 0; i < sm_iAreaY; i++)
			for (int j = 0; j < sm_iAreaX; j++)
				m_ggbGrid[y + i][x + j] = m_ggbGridSave[m_iY + i][m_iX + j];
		m_iX += dx[m_Key];
		m_iY += dy[m_Key];
	}
	for (short y = 0; y < sm_iScreenY; y++)
		for (short x = 0; x < sm_iScreenX; x++)
			if (m_ggbGrid[y][x] != m_ggbGridSave[y][x])
			{
				CursorTo(x+1, y+1);
				putchar(m_ggbGrid[y][x] ? '*' : ' ');
			}
	memcpy(m_ggbGridSave, m_ggbGrid, sizeof(m_ggbGrid));
	m_Key = NONE;
}

void Heart::CursorTo(short x, short y)
{
	//获取窗口句柄
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, { x,y });
}

int main()
{
	Heart MyHeart;
	return MyHeart.Execute();
}

