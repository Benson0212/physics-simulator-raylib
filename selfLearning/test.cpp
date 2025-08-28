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
float aR = 1; // air resistance
bool gravButtom = true;
bool airResistanceButtom = false;
bool showTrail = true;
bool energyLoss = true;// �u�I�}�
bool wallEnabled = true;

using namespace std;

////-----------------------------------�H�U�O�B�ʰ϶�-----------------------------------
class ball
{
public:
    ball(Vector2 initpos, float v0, double radian) : _initpos(initpos), _v0(v0), _rad(radian), _mass(1.0f) {}
    void init()
    {
        _pos = _initpos;
        _v = { _v0 * cos(_rad), -_v0 * sin(_rad) };
        trail.clear();
    }
    void move(vector<ball>& balls, const int num)
    {
        float dt = GetFrameTime();
        _pos.x += _v.first * dt;
        _v.second += g * dt;
        _pos.y += _v.second * dt;

        // �s�y�
        trail.push_back(_pos);
        if (trail.size() > 100) trail.erase(trail.begin());

        loop(i, 0, balls.size())// �y�鶡�I�B�z
        {
            Vector2 c1 = { _pos.x, _pos.y };
            if (i != num)
            {
                Vector2 c2 = { balls[i].getPos() };
                if (CheckCollisionCircles(c1, 25, c2, 25))
                {
                    // �u�ʸI�
                    pff c2v = balls[i].getV();
                    float distance = sqrt(pow(_pos.x - c2.x, 2) + pow(_pos.y - c2.y, 2));
                    float Cos = (c2.x - _pos.x) / distance;
                    float Sin = (c2.y - _pos.y) / distance;
                    pff V1_nt = { (_v.first * Cos + _v.second * Sin), (_v.first * -Sin + _v.second * Cos) };
                    pff V2_nt = { (c2v.first * Cos + c2v.second * Sin), (c2v.first * -Sin + c2v.second * Cos) };
                    swap(V1_nt.second, V2_nt.second);
                    _v.first = V1_nt.first * Cos - V1_nt.second * Sin;
                    _v.second = V1_nt.first * Sin + V1_nt.second * Cos;
                    balls[i].modifyV({ V2_nt.first * Cos - V2_nt.second * Sin, V2_nt.first * Sin + V2_nt.second * Cos });
                }
            }
        }
        if(wallEnabled)
        {
            if (_pos.y > GetScreenHeight() - 100)
            {
                _pos.y = GetScreenHeight() - 100;
                _v.second *= (energyLoss ? -0.8f : -1.0f);
            }
            else if (_pos.y < 0)
            {
                _pos.y = 0;
                _v.second *= (energyLoss ? -0.8f : -1.0f);
            }
            if (_pos.x > GetScreenWidth() - 25)
            {
                _pos.x = GetScreenWidth() - 25;
                _v.first *= (energyLoss ? -0.8f : -1.0f);
            }
            else if (_pos.x < 0)
            {
                _pos.x = 0;
                _v.first *= (energyLoss ? -0.8f : -1.0f);
            }
        }
        
    }
    void draw()
    {
        if (showTrail)
        {
            loop(i, 0, trail.size())
            {
                float alpha = (float)i / trail.size();
                DrawCircleV(trail[i], 4, Fade(BLUE, alpha));
            }
        }
        DrawCircleV(_pos, 25, BLACK);
        DrawText(gravButtom ? "Gravity : ON" : "Gravity : OFF", 50, 50, 24, gravButtom ? GREEN : RED);
        DrawText(airResistanceButtom ? "AirResistance : ON" : "AirResistance : OFF", 50, 80, 24, airResistanceButtom ? GREEN : RED);
        DrawText("PRESS [B] to add ball", 50, 110, 24, BLUE);
        DrawText("PRESS [T] to toggle trail", 50, 140, 24, BLUE);
        DrawText(energyLoss ? "Collision: Energy Loss" : "Collision: Perfect Elastic", 50, 170, 24, energyLoss ? RED : GREEN);
        DrawText(wallEnabled ? "[W] wallEnabled" : "[W] wallUnabled", 50, 230, 24, wallEnabled ? GREEN : RED);
    }
    void modifyV(const pff& newV) { _v = newV; }
    pff getV() const { return _v; }
    Vector2 getPos() const { return _pos; }
    float getMass() const { return _mass; }
private:
    float _v0, _rad;
    float _mass;
    pff _v;
    Vector2 _pos, _initpos;
    std::vector<Vector2> trail;
};

void addBall(vector<ball>& balls, const float& v0, const float& rad)
{
    float x = rand() % GetScreenWidth();
    float y = rand() % GetScreenHeight();
    balls.pb(ball({ x, y }, v0, rad));
    balls.back().init();
}

void gravCtrl() { gravButtom = !gravButtom; g = gravButtom ? 500.0f : 0.0f; }
void arCtrl() { airResistanceButtom = !airResistanceButtom; aR = airResistanceButtom ? 0.5f : 1.0f; }
void energyCtrl() { energyLoss = !energyLoss; }
void wall() { wallEnabled = !wallEnabled; }

////--------------------------�H�W�O�B�ʰ϶�--------------------------
////--------------------------�H�U�O�ޤO�϶�--------------------------

// Planet Mass: 0.1 ~ 10
// Planet Radius: 3 ~ 8
// Planet Position: Random
// Planet Initial Velocity: 100 ~ 150.0

class star
{
public:
    star(float mass, float radius, Vector2 pos, Vector2 v) : _mass(mass), _radius(radius), _pos(pos), _v(v) {}
    float GetMass() const { return _mass; }
    float GetR() const { return _radius; }
    Vector2 GetPos() const { return _pos; }
    Vector2 GetV() const { return _v; }
    void move(const star& sun) {
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
        trail.push_back(_pos);
        if (trail.size() > 200) trail.erase(trail.begin());
    }
    void draw() const
    {
        if (showTrail)
        {
            loop(i, 0, trail.size())
            {
                float alpha = (float)i / trail.size();
                DrawCircleV(trail[i], 2, Fade(SKYBLUE, alpha));
            }
        }
        DrawCircleV(_pos, _radius, WHITE);
    }
private:
    float _mass, _radius;
    Vector2 _pos, _v;
    std::vector<Vector2> trail;
};

star sun(50000.0f, 25.0f, { (float)(ScreenWidth / 2), (float)(ScreenHeight / 2) }, { 0, 0 });

void addStar(vector<star>& stars)
{
    float radium = 5 * rand() / (RAND_MAX + 1.0f) + 3;
    float x = rand() % GetScreenWidth();
    float y = rand() % GetScreenHeight();
    float Vx = (50 * rand() / (RAND_MAX + 1.0f) + 100.0f) * (rand() % 2 ? 1 : -1);
    float Vy = (50 * rand() / (RAND_MAX + 1.0f) + 100.0f) * (rand() % 2 ? 1 : -1);
    float mass = 9.9 * rand() / (RAND_MAX + 1.0f) + 0.1f;
    stars.pb(star(mass, radium, { x, y }, { Vx, Vy }));
}

////-----------------------�H�W�O�ޤO�϶�--------------------------------

// ========== �s�W�\�G�z�q�p�Ҳ� ==========
Vector2 ComputeMomentumBall(const ball& b) {
    return { b.getV().first * b.getMass(), b.getV().second * b.getMass() };
}
Vector2 ComputeMomentumStar(const star& s) {
    return { s.GetV().x * s.GetMass(), s.GetV().y * s.GetMass() };
}
Vector2 ComputeTotalMomentumBalls(const vector<ball>& balls) {
    Vector2 total{ 0,0 };
    for (auto& b : balls) {
        total = Vector2Add(total, ComputeMomentumBall(b));
    }
    return total;
}
Vector2 ComputeTotalMomentumStars(const vector<star>& stars) {
    Vector2 total{ 0,0 };
    for (auto& s : stars) {
        total = Vector2Add(total, ComputeMomentumStar(s));
    }
    return total;
}

// =============================================

char selectScreen()
{
    char result = 'X';
    InitWindow(ScreenWidth, ScreenHeight, "selectScreen");
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Projectile motion : PRESS [P]", 700, 700, 72, WHITE);
        DrawText("Gravity simulation : PRESS [G]", 700, 800, 72, WHITE);
        EndDrawing();
        if (IsKeyPressed(KEY_P)) { result = 'P'; break; }
        else if (IsKeyPressed(KEY_G)) { result = 'G'; break; }
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
        Vector2 initpos = { 0, GetScreenHeight() - 50 };
        vector<ball> balls;
        double deg, rad;
        float v0;
        cout << "speed : \n"; cin >> v0;
        cout << "degree : \n"; cin >> deg;
        rad = deg * DEG2RAD;
        balls.pb(ball(initpos, v0, rad));
        balls[0].init();
        InitWindow(ScreenWidth, ScreenHeight, "Projectile motion");
        ScreenWidth = GetMonitorWidth(0);
        ScreenHeight = GetMonitorHeight(0);
        while (!WindowShouldClose())
        {
            if (IsKeyPressed(KEY_B)) addBall(balls, v0, rad);
            if (IsKeyPressed(KEY_G)) gravCtrl();
            if (IsKeyPressed(KEY_A)) arCtrl();
            if (IsKeyPressed(KEY_T)) showTrail = !showTrail;
            if (IsKeyPressed(KEY_C)) energyCtrl();
            if (IsKeyPressed(KEY_W)) wall();
            BeginDrawing();
            ClearBackground(WHITE);
            loop(i, 0, balls.size())
            {
                balls[i].move(balls, i);
                balls[i].draw();
            }
            // �`�ʶq
            Vector2 totalP = ComputeTotalMomentumBalls(balls);
            DrawText(TextFormat("Total Momentum: (%.2f, %.2f)", totalP.x, totalP.y), 50, 210, 24, PURPLE);
            EndDrawing();
        }
    }
    else if (c == 'G')
    {
        vector<star> stars;
        InitWindow(ScreenWidth, ScreenHeight, "Gravity simulation");
        ScreenWidth = GetMonitorWidth(0);
        ScreenHeight = GetMonitorHeight(0);
        while (!WindowShouldClose()) {
            if (IsKeyPressed(KEY_A)) addStar(stars);
            if (IsKeyPressed(KEY_T)) showTrail = !showTrail;
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("PRESS [A] to add planet", 50, 50, 24, BLUE);
            DrawText("PRESS [T] to toggle trail", 50, 80, 24, BLUE);
            for (auto& s : stars)
            {
                s.move(sun);
                s.draw();
            }
            sun.draw();

            // �`�ʶq
            Vector2 totalP = ComputeTotalMomentumStars(stars);
            DrawText(TextFormat("Total Momentum: (%.2f, %.2f)", totalP.x, totalP.y), 50, 120, 24, PURPLE);

            EndDrawing();
        }
    }
    CloseWindow();
    return 0;
}
