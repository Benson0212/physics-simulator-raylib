# raylib 自主學習筆記

## 前言

因為在做自主學習時一直上網查各種raylib函數**很麻煩**，然後又**會忘記** ( Raylib內建函數的名稱都挺長的)，所以在這邊整理了一些**常用**的raylib函數。內容**有誤**或有**不完整**的地方**歡迎留言告知** :slightly_smiling_face: 

## 1. raylib 簡介

raylib 是一個 C 語言遊戲開發庫，適合學習圖形編程和遊戲開發。

- 官方網站：[https://www.raylib.com/](https://www.raylib.com/)
- GitHub：[https://github.com/raysan5/raylib](https://github.com/raysan5/raylib)
- 支援平台：Windows、Linux、Mac、Raspberry Pi

## 2. 安裝與環境設置
:::info
***請先自行安裝 visual studio***
:::
### 2.1 Windows

1. 下載 raylib 官方提供的 Windows 安裝包
2. 參考此影片教學進行 Visual Studio 設置：[Windows 下載教學 + Visual Studio 設置](https://www.youtube.com/watch?v=q0gxwjBSmX4)
3. 設置 Visual Studio / MinGW 環境
4. 測試安裝是否成功

:::danger
※ 註 : **不建議**使用**命令提示字元**安裝Raylib，因為這麼做似乎還需要安裝額外的套件，挺麻煩的 (我當時花了兩個小時還搞不定 🤡)
:::

### 2.2 Linux (Ubuntu/Debian)

```bash
sudo apt install libglfw3-dev libopenal-dev
```

或者從源碼編譯。

### 2.3 macOS

使用 Homebrew 安裝：

```bash
brew install raylib
```

## 3. 基本功能

### 3.0 建立一個基本的窗口

```cpp=
#include "raylib.h"

int main()
{
    InitWindow(800, 600, "Hello, raylib!");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, raylib!", 350, 280, 20, DARKGRAY);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
```
執行結果![image](https://hackmd.io/_uploads/HJoDcyc_Jg.png)

### 3.1 基本函數介紹

以下是一些 raylib 的基本函數：

- `InitWindow(int width, int height, const char *title)`：初始化視窗。
- `BeginDrawing()`：開始繪製畫面。
- `GetMonitorWidth(int monitor)`：獲取指定螢幕的寬度。
- `GetMonitorHeight(int monitor)`：獲取指定螢幕的高度。
- `ClearBackground(Color color)`：清除畫面並設置背景顏色。
- `MeasureTextEx(Font font, const char *text, float fontSize, float spacing)`：計算指定文字的寬高。
- `GetFontDefault()`：獲取預設字型。
- `DrawText(const char *text, int posX, int posY, int fontSize, Color color)`：在畫面上繪製文字。
- `GetScreenWidth()`：獲取視窗的當前寬度。
- `GetScreenHeight()`：獲取視窗的當前高度。
- `EndDrawing()`：結束繪製畫面。
- `CloseWindow()`：關閉視窗。

#### 範例程式

```cpp=
#include "raylib.h"

int main()
{
    const int ScreenWidth = GetMonitorWidth(0); // 長寬設為 MAX
    const int ScreenHeight = GetMonitorHeight(0);
    InitWindow(ScreenWidth, ScreenHeight, "FuncTesting"); // width, height, title
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        const auto size_of_text = MeasureTextEx(GetFontDefault(), "you win", 72, 0);
        DrawText("you win!", (GetScreenWidth() - size_of_text.x) / 2, (GetScreenHeight() - size_of_text.y) / 2, 72, RED);
        EndDrawing();
    }
    CloseWindow();
}
```
執行結果![image](https://hackmd.io/_uploads/By_ct19ukl.png)

### 3.2 繪製基本圖形

- `DrawCircle(int centerX, int centerY, float radius, Color color)`
- `DrawCircleV(Vector2 center, float radius, Color color)`
- `DrawCircleLines(int centerX, int centerY, float radius, Color color)`
- `DrawEllipse(int centerX, int centerY, float radiusH, float radiusV, Color color)` : 橢圓
- `DrawPoly(Vector2 center, int sides, float radius, float rotation, Color color)` : 正多邊形
- `DrawRectangle(int posX, int posY, int width, int height, Color color)`
- `DrawRectangleLines(int posX, int posY, int width, int height, Color color)` : 矩形邊框
- `DrawRectangleRounded(Rectangle rec, float roundness, int segments, Color color)` : 圓角矩形
- `DrawTriangle(Vector2 v1, Vector2 v2, Vector2 v3, Color color)`
- `DrawTriangleLines(Vector2 v1, Vector2 v2, Vector2 v3, Color color)`
- `DrawLine(int x1, int y1, int x2, int y2, Color color)`
- `DrawLineEx(Vector2 start, Vector2 end, float thick, Color color)`
- `DrawPixel(int x, int y, Color color)` : 單個像素
:::info
順帶一提，raylib的座標系統 (0,0) 是**左上角**
- X 軸**向右增加**（往右的數值變大）
- Y 軸**向下增加**（往下的數值變大）
:::
### 3.3 鍵盤 & 滑鼠 輸入處理

#### 3.3.1 鍵盤
- `IsKeyPressed(KEY_X)` : 當按鍵 X **剛被按下**時返回 `true`（**僅觸發一次**）
- `IsKeyDown(KEY_X)` : 當按鍵 X **持續被按住**時返回 `true`（**每幀都會觸發**）
- `IsKeyReleased(KEY_X)` : 當按鍵 X **剛被釋放**時返回 `true`（**僅觸發一次**）
- `IsKeyUp(KEY_X)` : 當按鍵 X **沒有被按下**時返回 `true`
- `GetKeyPressed()` / `GetCharPressed()` : 獲取當前按下的鍵（ASCII 值），如果沒有則返回 0
#### 範例程式
```cpp=
// 有玩過minecraft的人都知道 按住W是前進 加上ctrl就會變成疾走😏
if (IsKeyDown(KEY_W))
{
    float moveSpeed = IsKeyDown(KEY_LEFT_CTRL) ? 10.0f : 5.0f;
    position.y -= moveSpeed;
}
```
---
#### 3.3.2 滑鼠
- `IsMouseButtonPressed(MOUSE_BUTTON_LEFT)` : 當按下滑鼠左鍵的**瞬間**回傳 `true` （**只觸發一次**）
- `IsMouseButtonDown(MOUSE_BUTTON_LEFT)` : 當滑鼠左鍵**保持按下**時，**每幀**都回傳 `true`
- `GetMousePosition()` : 用於獲取滑鼠當前的螢幕座標，回傳值為`Vector2` (適用於偵測**滑鼠點擊**、**拖曳**、**物件選取**)
- `GetMouseDelta()` : 用於獲取滑鼠**自上一幀以來的移動量**，回傳值為`Vector2` (適用於偵測**滑鼠拖曳**)
### 3.4 時間相關函數
- `GetTime()` : 回傳程式**啟動到現在**經過的秒數。**回傳值的型態是double**
- `SetTargetFPS(int fps)` : 限制主迴圈運行的最大 FPS（幀率），防止 CPU 資源浪費
- `GetFPS()` : 回傳當前的幀率
- `GetFrameTime()` : 回傳**上一幀的持續時間**（秒），適合用來做 時間同步（Δt），確保動畫或移動速度不受 FPS 影響
- `WaitTime(double seconds)` : 讓程式**暫停指定秒數**，適用於需要暫停但不影響主迴圈的場景
## 4. 進階應用(未完成)

### 4.1 音效與音樂

- 使用 `InitAudioDevice()` 初始化音訊系統
- `LoadSound()` 載入音效檔案
- `PlaySound()` 播放音效

### 4.2 碰撞偵測

- `CheckCollisionRecs()` : 矩形和矩形之間有碰撞就回傳`true`
- `CheckCollisionCircles(Vector2 center1, float radius1, Vector2 center2, float radius2)` : 圓形和圓形之間有碰撞就回傳`true`
:::success
其實還有一些**不同圖形間**的碰撞偵測函數，但~~我好累我不想打~~了 :tired_face: 
有需要就自己去查吧 :+1:
:::

### 4.3 使用 $Shader$

- `LoadShader()` 加載 Shader
- `SetShaderValue()` 設定 Shader 參數

## 5. 小專案練習(未完成)


### 5.1 拋體運動 + 邊框反彈
**單一球體**
```cpp=
#include <iostream>
#include <cmath>
#include <utility>
#include "raylib.h"
#define pff pair<float,float>
const float g = 500.0;
const float pi = 3.14159;
using namespace std;

int main()
{
	//ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
	const int ScreenWidth = GetMonitorWidth(0);// 長寬設為MAX
	const int ScreenHeight = GetMonitorHeight(0);
	InitWindow(ScreenWidth, ScreenHeight,"FuncTesting");//width,height,title
	Vector2 initpos = { 0, GetScreenHeight() - 50};// initial position
	Vector2 pos = initpos;
	pff v;// vx,vy
	double deg,rad;
	float v0;
	cout << "初速 : \n";
	cin >> v0;
	cout << "角度 : \n";
	cin >> deg;
	rad = deg * DEG2RAD;//rad = deg * M_PI / 180;
	v = { v0 * cos(rad), -v0 * sin(rad)};
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		float dt = GetFrameTime();
		pos.x += v.first * dt;
		v.second += g * dt;
		pos.y += v.second * dt;
		if (pos.y > GetScreenHeight() - 100)
		{
			pos.y = GetScreenHeight() - 100;
			v.second *= -0.8f;
		}
		else if (pos.y < 0)
		{
			pos.y = 0;
			v.second *= -0.8f;
		}
		if (pos.x > GetScreenWidth() - 25)
		{
			pos.x = GetScreenWidth() - 25;
			v.first *= -0.8f;
		}
		else if (pos.x <  0)
		{
			pos.x = 0;
			v.first *= -0.8f;
		}
		cout << "x : " << pos.x << " y : " << pos.y << "\n";
		BeginDrawing();
               ClearBackground(WHITE);
		const auto size_of_text = MeasureTextEx(GetFontDefault(), "FuncTest", 72, 0);
		DrawText("FuncTest", (GetScreenWidth() - size_of_text.x) / 2, (GetScreenHeight() - size_of_text.y) / 2, 72, BLACK);
		DrawCircleV(pos,25,BLACK);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
```
---
**新增 加球功能 & 重力開關**
```cpp=
#include <iostream>
#include <cmath>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "raylib.h"
#define loop(i,a,b) for(int i = a ; i < b ; ++i )
#define pff pair<float,float>
#define pb push_back
float g = 500.0;
bool gravButtom = true;
using namespace std;

class ball
{
public:
	ball(Vector2 initpos, float v0, double radian) : initpos_(initpos), v0_(v0), rad_(radian) {}
	void init()
	{
		pos_ = initpos_;
		v_ = { v0_ * cos(rad_), -v0_ * sin(rad_) };
	}
	void move()
	{
		float dt = GetFrameTime();
		pos_.x += v_.first * dt;
		v_.second += g * dt;
		pos_.y += v_.second * dt;
		if (pos_.y > GetScreenHeight() - 100)
		{
			pos_.y = GetScreenHeight() - 100;
			v_.second *= -0.8f;
		}
		else if (pos_.y < 0)
		{
			pos_.y = 0;
			v_.second *= -0.8f;
		}
		if (pos_.x > GetScreenWidth() - 25)
		{
			pos_.x = GetScreenWidth() - 25;
			v_.first *= -0.8f;
		}
		else if (pos_.x < 0)
		{
			pos_.x = 0;
			v_.first *= -0.8f;
		}
		//cout << "x : " << pos_.x << " y : " << pos_.y << "\n";
	}
	void draw()
	{
		const auto size_of_text = MeasureTextEx(GetFontDefault(), "FuncTest", 72, 0);
		DrawText("FuncTest", (GetScreenWidth() - size_of_text.x) / 2, (GetScreenHeight() - size_of_text.y) / 2, 72, BLACK);
		if(gravButtom) DrawText("Gravity : ON", 50, 50, 24, GREEN);
		else DrawText("Gravity : OFF", 50, 50, 24, RED);
		DrawCircleV(pos_, 25, BLACK);
	}
private:
	float v0_, rad_;
	pff v_;//vx vy
	Vector2 pos_;
	Vector2 initpos_;
};

void addBall(vector <ball> &balls,const float &v0,const float &rad)
{
	float x = rand() % GetScreenWidth();
	float y = rand() % GetScreenHeight();
	balls.pb(ball({ x, y }, v0, rad));
	balls[balls.size() - 1].init();
}
void gravCtrl()
{
	if (gravButtom)
	{
		gravButtom = false;
		g = 0.0;
	}
	else
	{
		gravButtom = true;
		g = 500.0;
	}
}

int main()
{
	srand(time(NULL));
	const int ScreenWidth = GetMonitorWidth(0);
	const int ScreenHeight = GetMonitorHeight(0);
	InitWindow(ScreenWidth, ScreenHeight,"FuncTesting");//width,height,title
	Vector2 initpos = { 0, GetScreenHeight() - 50};// initial position
	Vector2 pos = initpos;
	vector <ball> balls;
	double deg,rad;
	float v0;
	cout << "初速 : \n";
	cin >> v0;
	cout << "角度 : \n";
	cin >> deg;
	rad = deg * DEG2RAD;//rad = deg * M_PI / 180;
	balls.pb(ball(initpos, v0, rad));
	balls[0].init();
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_P)) addBall(balls, v0, rad);
		if (IsKeyPressed(KEY_G)) gravCtrl();
		BeginDrawing();
		ClearBackground(WHITE);
		loop(i, 0, balls.size())
		{
			balls[i].move();
			balls[i].draw();
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}
```


---
新增 球體間碰撞
```cpp=
#include <iostream>
#include <cmath>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "raylib.h"
#define ull unsigned long long
#define ll long long
#define ui unsigned int
#define loop(i,a,b) for(int i = a ; i < b ; ++i )
#define pff pair<float,float>
#define pb push_back
float g = 500.0;
bool gravButtom = true;
using namespace std;

class ball
{
public:
	ball(Vector2 initpos, float v0, double radian) : initpos_(initpos), v0_(v0), rad_(radian) {}
	void init()
	{
		pos_ = initpos_;
		v_ = { v0_ * cos(rad_), -v0_ * sin(rad_) };
	}
	void move(vector<ball> &balls,const int num)
	{
		float dt = GetFrameTime();
		pos_.x += v_.first * dt;
		v_.second += g * dt;
		pos_.y += v_.second * dt;
		loop(i, 0, balls.size())// 球體間碰撞處理
		{
			Vector2 c1 = { pos_.x, pos_.y };
			if (i != num)
			{
				Vector2 c2 = { balls[i].getPos() };
				if (CheckCollisionCircles(c1, 25, c2, 25))
				{
					// 彈性碰撞邏輯(未完成)
					pff c2v = balls[i].getV();
					float distance = sqrt(pow(pos_.x - c2.x, 2) + pow(pos_.y - c2.y,2));
					float Cos = (c2.x - pos_.x) / distance;
					float Sin = (c2.y - pos_.y) / distance;
					pff V1_nt = { (v_.first * Cos + v_.second * Sin),(v_.first * Sin * -1 + v_.second * Cos) };//V切線 V法線
					pff V2_nt = { (c2v.first * Cos + c2v.second * Sin),(c2v.first * Sin * -1 + c2v.second * Cos) };
					swap(V1_nt.second, V2_nt.second);
					v_.first = V1_nt.first * Cos - V1_nt.second * Sin;
					v_.second = V1_nt.first * Sin + V1_nt.second * Cos;
					balls[i].modifyV({ V2_nt.first * Cos - V2_nt.second * Sin,V2_nt.first * Sin + V2_nt.second * Cos });
				}
			}
		}
		if (pos_.y > GetScreenHeight() - 100)
		{
			pos_.y = GetScreenHeight() - 100;
			v_.second *= -0.8f;
		}
		else if (pos_.y < 0)
		{
			pos_.y = 0;
			v_.second *= -0.8f;
		}
		if (pos_.x > GetScreenWidth() - 25)
		{
			pos_.x = GetScreenWidth() - 25;
			v_.first *= -0.8f;
		}
		else if (pos_.x < 0)
		{
			pos_.x = 0;
			v_.first *= -0.8f;
		}
	}
	void draw()
	{
		const auto size_of_text = MeasureTextEx(GetFontDefault(), "FuncTest", 72, 0);
		DrawText("FuncTest", (GetScreenWidth() - size_of_text.x) / 2, (GetScreenHeight() - size_of_text.y) / 2, 72, BLACK);
		if(gravButtom) DrawText("Gravity : ON", 50, 50, 24, GREEN);
		else DrawText("Gravity : OFF", 50, 50, 24, RED);
		DrawCircleV(pos_, 25, BLACK);
	}
	void modifyV(const pff &newV)
	{
		v_.first = newV.first;
		v_.second = newV.second;
	}
	pff getV() const { return v_;}
	Vector2 getPos() const { return pos_;}
private:
	float v0_, rad_;
	pff v_;//vx vy
	Vector2 pos_;
	Vector2 initpos_;
};

void addBall(vector <ball> &balls,const float &v0,const float &rad)
{
	float x = rand() % GetScreenWidth();
	float y = rand() % GetScreenHeight();
	balls.pb(ball({ x, y }, v0, rad));
	balls[balls.size() - 1].init();
}

void gravCtrl()
{
	if (gravButtom)
	{
		gravButtom = false;
		g = 0.0;
	}
	else
	{
		gravButtom = true;
		g = 500.0;
	}
}

int main()
{
	srand(time(NULL));
	const int ScreenWidth = GetMonitorWidth(0);
	const int ScreenHeight = GetMonitorHeight(0);
	InitWindow(ScreenWidth, ScreenHeight,"FuncTesting");//width,height,title
	Vector2 initpos = { 0, GetScreenHeight() - 50};// initial position
	Vector2 pos = initpos;
	vector <ball> balls;
	double deg,rad;
	float v0;
	cout << "初速 : \n";
	cin >> v0;
	cout << "角度 : \n";
	cin >> deg;
	rad = deg * DEG2RAD;//rad = deg * M_PI / 180;
	balls.pb(ball(initpos, v0, rad));
	balls[0].init();
	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_P)) addBall(balls, v0, rad);
		if (IsKeyPressed(KEY_G)) gravCtrl();
		BeginDrawing();
		ClearBackground(BROWN);
		loop(i, 0, balls.size())
		{
			balls[i].move(balls,i);
			balls[i].draw();
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

```
### 5.2 粒子效果

- 使用 `DrawCircle()` 畫出粒子
- 讓粒子根據速度與重力移動

---
## 6. 參考資料

- raylib 官方文件：[https://www.raylib.com/cheatsheet/cheatsheet.html](https://www.raylib.com/cheatsheet/cheatsheet.html)
- raylib Wiki：[https://github.com/raysan5/raylib/wiki](https://github.com/raysan5/raylib/wiki)
- raylib 教學影片 : [https://www.youtube.com/playlist?list=PLoNTPa4sJuLTO2knkE19hU5WWWkh_xEN7](https://www.youtube.com/playlist?list=PLoNTPa4sJuLTO2knkE19hU5WWWkh_xEN7)

