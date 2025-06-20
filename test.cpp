#include <iostream>
#include <cmath>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include "raylib.h"
#include "raymath.h"
#define ull unsigned long long
#define ll long long
#define ui unsigned int
#define loop(i,a,b) for(int i = a ; i < b ; ++i )
#define pff pair<float,float>
#define pb push_back

int ScreenWidth = 2560;
int ScreenHeight = 1600;
float g = 500.0f;
float G = 400.0f;
float aR = 1;//air resistance
bool gravButtom = true;
bool airResistanceButtom = false;
using namespace std;

//-----------------------------------以下是拋體運動區塊-----------------------------------
class ball
{
public:
	ball(Vector2 initpos, float v0, double radian) : _initpos(initpos), _v0(v0), _rad(radian) {}
	void init()
	{
		_pos = _initpos;
		_v = { _v0 * cos(_rad), -_v0 * sin(_rad) };
	}
	void move(vector<ball> &balls,const int num)
	{
		float dt = GetFrameTime();
		_pos.x += _v.first * dt;
		_v.second += g * dt;
		_pos.y += _v.second * dt;
		loop(i, 0, balls.size())// 球體間碰撞處理 (好像不適合用loop()處理 但我也想不到啥好辦法
		{
			Vector2 c1 = { _pos.x, _pos.y };
			if (i != num)
			{
				Vector2 c2 = { balls[i].getPos() };
				if (CheckCollisionCircles(c1, 25, c2, 25))
				{
					// 彈性碰撞邏輯...好像寫爛了:(
					pff c2v = balls[i].getV();
					float distance = sqrt(pow(_pos.x - c2.x, 2) + pow(_pos.y - c2.y,2));
					float Cos = (c2.x - _pos.x) / distance;
					float Sin = (c2.y - _pos.y) / distance;
					pff V1_nt = { (_v.first * Cos + _v.second * Sin),(_v.first * Sin * -1 + _v.second * Cos) };//V切線 V法線
					pff V2_nt = { (c2v.first * Cos + c2v.second * Sin),(c2v.first * Sin * -1 + c2v.second * Cos) };
					swap(V1_nt.second, V2_nt.second);
					_v.first = V1_nt.first * Cos - V1_nt.second * Sin;
					_v.second = V1_nt.first * Sin + V1_nt.second * Cos;
					balls[i].modifyV({ V2_nt.first * Cos - V2_nt.second * Sin,V2_nt.first * Sin + V2_nt.second * Cos });
				}
			}
		}
		if (_pos.y > GetScreenHeight() - 100)
		{
			_pos.y = GetScreenHeight() - 100;
			_v.second *= -0.8f;
		}
		else if (_pos.y < 0)
		{
			_pos.y = 0;
			_v.second *= -0.8f;
		}
		if (_pos.x > GetScreenWidth() - 25)
		{
			_pos.x = GetScreenWidth() - 25;
			_v.first *= -0.8f;
		}
		else if (_pos.x < 0)
		{
			_pos.x = 0;
			_v.first *= -0.8f;
		}
	}
	void draw()
	{
		const auto size_of_text = MeasureTextEx(GetFontDefault(), "FuncTest", 72, 0);
		DrawText("FuncTest", (GetScreenWidth() - size_of_text.x) / 2, (GetScreenHeight() - size_of_text.y) / 2, 72, BLACK);
		if(gravButtom) DrawText("Gravity : ON", 50, 50, 24, GREEN);
		else DrawText("Gravity : OFF", 50, 50, 24, RED);
		if (airResistanceButtom) DrawText("AirResistance : ON", 50, 80, 24, GREEN);
		else DrawText("AirResistance : OFF", 50, 80, 24, RED);
		DrawCircleV(_pos, 25, BLACK);
	}
	void modifyV(const pff &newV)
	{
		_v.first = newV.first;
		_v.second = newV.second;
	}
	pff getV() const { return _v;}
	Vector2 getPos() const { return _pos;}
private:
	float _v0, _rad;
	pff _v;//vx vy
	Vector2 _pos;
	Vector2 _initpos;
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
void arCtrl()// 這函數好像沒啥影響
{
	if (airResistanceButtom)
	{
		airResistanceButtom = false;
		aR = 1.0;
	}
	else
	{
		airResistanceButtom = true;
		aR = 0.5;
	}
}
//--------------------------以上是拋體運動區塊--------------------------
//--------------------------以下是引力模擬區塊--------------------------
/*
*  行星質量 : 0.1 ~ 10
*  行星半徑 : 3 ~ 8
*  行星位置 : random
*  行星初速 : 100 ~ 150.0
*/
class star
{
public:
	star(float mass, float radius, Vector2 pos, Vector2 v) : _mass(mass), _radius(radius), _pos(pos), _v(v){}
	float GetMass() const { return _mass; }
	float GetR() const { return _radius; }
	Vector2 GetPos() const { return _pos; }
	void draw() const { DrawCircleV(_pos, _radius, WHITE);}
	void move(const star &sun)
	{
		float dt = GetFrameTime();
		float r = Vector2Distance(sun.GetPos(), _pos);
		float a = G * sun.GetMass() / (r * r);
		float x = sun.GetPos().x - _pos.x;
		float y = sun.GetPos().y - _pos.y;
		float srt = sqrt(x * x + y * y);
		_v.x += a * dt * x / srt;
		_v.y += a * dt * y / srt;
		_pos.x += _v.x * dt;
		_pos.y += _v.y * dt;
	}
private:
	float _mass, _radius;
	Vector2 _pos, _v;
};
star sun(50000.0f, 25.0f, { (float)(ScreenWidth / 2), (float)(ScreenHeight / 2)}, { 0, 0});//50000

void addStar(vector<star> &stars)
{
	float radium = 5 * rand() / ( RAND_MAX + 1.0) + 3;//rand()用法 -> https://blog.gtwang.org/programming/c-cpp-rand-random-number-generation-tutorial-examples/
	float x = rand() % GetScreenWidth();
	float y = rand() % GetScreenHeight();
	int result = (std::rand() % 2 == 0) ? 1 : -1;
	float Vx = (50 * rand() / (RAND_MAX + 1.0) + 100.0) * result;
	result = (std::rand() % 2 == 0) ? 1 : -1;
	float Vy = (50 * rand() / (RAND_MAX + 1.0) + 100.0) * result;
	float mass = 9.9 * rand() / (RAND_MAX + 1.0) + 0.1;
	stars.pb(star(mass, radium, { x,y }, { Vx, Vy}));
}
//-----------------------以上是引力模擬區塊--------------------------------

char selectScreen()
{
	char result = 'X';
	InitWindow(ScreenWidth, ScreenHeight, "selectScreen");//width,height,title
	while (! WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		const auto size_of_text1 = MeasureTextEx(GetFontDefault(), "Projectile motion : PRESS [P]", 72, 0);
		const auto size_of_text2 = MeasureTextEx(GetFontDefault(), "Gravity simulation : PRESS [G]", 72, 0);
		DrawText("Projectile motion : PRESS [P]", (GetScreenWidth() - size_of_text1.x) / 2, (GetScreenHeight() - size_of_text1.y) / 2 - 40, 72, WHITE);
		DrawText("Gravity simulation : PRESS [G]", (GetScreenWidth() - size_of_text2.x) / 2, (GetScreenHeight() - size_of_text2.y) / 2 + 40, 72, WHITE);
		EndDrawing();
		if (IsKeyPressed(KEY_P)) { result = 'P'; break;}
		else if (IsKeyPressed(KEY_G)) { result = 'G'; break;}
	}
	CloseWindow();
	return result;
}
int main()
{
	srand(time(nullptr));
	SetTargetFPS(60);
	const char c = selectScreen();
	if (c == 'P')
	{
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
		InitWindow(ScreenWidth, ScreenHeight,"Projectile motion");//width,height,title
		ScreenWidth = GetMonitorWidth(0);
		ScreenHeight = GetMonitorHeight(0);
		while (!WindowShouldClose())
		{
			if (IsKeyPressed(KEY_B)) addBall(balls, v0, rad);
			if (IsKeyPressed(KEY_G)) gravCtrl();
			if (IsKeyPressed(KEY_A)) arCtrl();
			BeginDrawing();
			ClearBackground(BROWN);
			loop(i, 0, balls.size())
			{
				balls[i].move(balls,i);
				balls[i].draw();
			}
			EndDrawing();
		}
	}
	else if (c == 'G')
	{
		vector <star> stars;
		InitWindow(ScreenWidth, ScreenHeight, "Gravity simulation");
		ScreenWidth = GetMonitorWidth(0);
		ScreenHeight = GetMonitorHeight(0);
		while (!WindowShouldClose())
		{
			BeginDrawing();
			ClearBackground(BLACK);
			DrawText("PRESS [A] to add planet", 50, 50, 24, BLUE);
			if (IsKeyPressed(KEY_A)) addStar(stars);
			loop(i, 0, stars.size())
			{
				stars[i].move(sun);
				stars[i].draw();
			}
			sun.draw();
			EndDrawing();
		}
	}
	CloseWindow();
	return 0;
}
