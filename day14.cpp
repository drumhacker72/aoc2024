#include <iostream>
#include <vector>

#include <lexy/action/parse.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/input/file.hpp>
#include <lexy_ext/report_error.hpp>

#include <PixelToaster.h>

using namespace std;
using namespace PixelToaster;

struct Vec2
{
    int x;
    int y;
};

struct Robot
{
    Vec2 p;
    Vec2 v;
};

constexpr int width = 101;
constexpr int height = 103;
constexpr int scale = 8;

namespace grammar
{
    namespace dsl = lexy::dsl;
    struct signed_int
    {
        static constexpr auto rule = dsl::sign + dsl::integer<int>;
        static constexpr auto value = lexy::as_integer<int>;
    };
    struct vec2
    {
        static constexpr auto rule = dsl::p<signed_int> + dsl::lit_c<','> + dsl::p<signed_int>;
        static constexpr auto value = lexy::construct<Vec2>;
    };
    struct robot
    {
        static constexpr auto rule = LEXY_LIT("p=") >> dsl::p<vec2> + LEXY_LIT(" v=") + dsl::p<vec2>;
        static constexpr auto value = lexy::construct<Robot>;
    };
    struct robot_list
    {
        static constexpr auto rule = dsl::list(dsl::p<robot>, dsl::trailing_sep(dsl::newline));
        static constexpr auto value = lexy::as_list<vector<Robot>>;
    };
    struct input
    {
        static constexpr auto rule = dsl::p<robot_list> + dsl::eof;
        static constexpr auto value = robot_list::value;
    };
}

int mod(int k, int n)
{
    int r = k % n;
    return r < 0 ? r+n : r;
}

void step(vector<Robot>& robots, bool backward = false)
{
    for (auto& robot : robots)
    {
        Vec2 p;
        if (!backward)
            p = {robot.p.x + robot.v.x, robot.p.y + robot.v.y};
        else
            p = {robot.p.x - robot.v.x, robot.p.y - robot.v.y};
        robot.p = {mod(p.x, width), mod(p.y, height)};
    }
}

enum RunMode { paused, running, step_forward, step_back, info };

struct State : public Listener
{
    RunMode mode;
    void onKeyDown(DisplayInterface& display, Key key) override
    {
        switch (key)
        {
        case Key::Space:
            mode = mode == running ? paused : running;
            break;
        case Key::Left:
            mode = step_back;
            break;
        case Key::Right:
            mode = step_forward;
            break;
        case Key::I:
            mode = info;
        default:
            break;
        }
    }
};

int main()
{
    auto file = lexy::read_stdin();
    auto result = lexy::parse<grammar::input>(file.buffer(), lexy_ext::report_error);
    vector<Robot> robots = result.value();
    int t = 0;
    for (; t < 100; ++t)
        step(robots);
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    for (auto const& robot : robots)
    {
        if (robot.p.x < width/2 && robot.p.y < height/2)
            ++q1;
        if (robot.p.x > width/2 && robot.p.y < height/2)
            ++q2;
        if (robot.p.x < width/2 && robot.p.y > height/2)
            ++q3;
        if (robot.p.x > width/2 && robot.p.y > height/2)
            ++q4;
    }
    cout << q1 * q2 * q3 * q4 << '\n';

    //found by visual inspection
    for (; t < 7492; ++t)
        step(robots);

    Display display {"AoC 2024 Day 14", width*scale, height*scale, Output::Default, Mode::TrueColor};
    State state;
    display.listener(&state);
    Timer timer;
    while (display.open())
    {
        vector<TrueColorPixel> pixels (width*height*scale*scale, {255, 255, 255});
        for (auto const& robot : robots)
        {
            for (int y = robot.p.y*scale; y < robot.p.y*scale + scale; ++y)
            {
                for (int x = robot.p.x*scale; x < robot.p.x*scale + scale; ++x)
                    pixels[y*width*scale + x] = {0, 0, 0};
            }
        }
        display.update(pixels);
        switch (state.mode)
        {
        case step_forward:
            state.mode = paused;
            [[fallthrough]];
        case running:
            step(robots);
            ++t;
            break;
        case step_back:
            state.mode = paused;
            step(robots, true);
            --t;
            break;
        case info:
            state.mode = paused;
            cout << t << '\n';
            break;
        case paused:
            break;
        }
        timer.wait(0.05f);
    }
    return 0;
}
