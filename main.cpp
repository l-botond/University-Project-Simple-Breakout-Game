#include "graphics.hpp"
#include <vector>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace genv;
using namespace std;

const int szelesseg = 1000;
const int magassag = 800;

int elet = 3;

void loser();
void winner();

struct Uto
{
private:
    int _x, _y, _szel, _mag;

public:
    Uto(int x, int y, int szel, int mag) : _x(x), _y(y), _szel(szel), _mag(mag)
    {

    }

    void valtoztat()
    {
        if(_x < 0)
            _x = 0;

        if(_szel + _x > szelesseg)
            _x = szelesseg - _szel;
    }

    void mozgat(int mx)
    {
        _x = mx - (szelesseg / 2 - 410);
    }

    int x_koord()                       //getterek
    {
        return _x;
    }

    int y_koord()
    {
        return _y;
    }

    int szel_nagysag()
    {
        return _szel;
    }

    void kirajzol()
    {
        gout << move_to(_x, _y)
             << color(255, 255, 255)
             << box(_szel, _mag);
    }

};

struct Labda
{
private:
    int _x, _y, _meret, _dx, _dy;
    float _seb;

public:
    Labda(int x, int y, int meret) : _x(x), _y(y), _meret(meret), _dx(3), _dy(-3), _seb(1.5)
    {

    }

    void mozog()
    {
        _x += _dx * _seb;
        _y += _dy * _seb;

        if(_x <= 0 || _x + _meret >= szelesseg)
            _dx = - _dx;
        if(_y <= 0)
            _dy = - _dy;
    }

    void visszapattan()
    {
        _dy = - _dy;
    }

    int x_koord()
    {
        return _x;
    }

    int y_koord()
    {
        return _y;
    }

    int meret_nagysag()
    {
        return _meret;
    }

    void pozicio(int x, int y)                          //setterek
    {
        _x = x;
        _y = y;
    }

    void irany(int lx, int ly)
    {
        _dx = lx;
        _dy = ly;
    }

    void kirajzol()
    {
        gout << move_to(_x, _y)
             << color(255, 255, 255)
             << box(_meret, _meret);
    }
};

struct Tegla
{
private:
    int _x, _y, _szel, _mag;
    bool _elo;
    int _r, _g, _b;

public:
    Tegla(int x, int y, int szel, int mag, int r, int g, int b)
    : _x(x), _y(y), _szel(szel), _mag(mag), _elo(true), _r(r), _g(g), _b(b)
    {

    }

    bool el()
    {
        return _elo;
    }

    bool nem_el()
    {
        return _elo = false;
    }

    int x_koord()
    {
        return _x;
    }

    int y_koord()
    {
        return _y;
    }

    int szel_nagysag()
    {
        return _szel;
    }

    int mag_nagysag()
    {
        return _mag;
    }

    void kirajzol()
    {
        if(_elo)
        {
            gout << move_to(_x, _y)
                 << color(_r, _g, _b)
                 << box(_szel, _mag);
        }
    }
};

void jatek()
{
    gout.open(szelesseg, magassag);

    vector<Tegla*> t;

    Uto* u = new Uto(szelesseg / 2 - 50, magassag - 30, 200, 10);
    Labda* l = new Labda(rand() % szelesseg, magassag / 2, 10);

    int szinek[][3] =
    {
        {255, 0, 0},
        {255, 165, 0},
        {255, 255, 0},
        {0, 0, 255},
        {75, 0, 130}

    };

    int osszes_tegla = 0;
    int sor = 0;
    for(int i = 50; i < 200; i+= 30)
    {
        int r = szinek[sor % 5][0];
        int g = szinek[sor % 5][1];
        int b = szinek[sor % 5][2];

        for(int j = 25; j + 50 <= szelesseg - 25; j += 60)
        {
            t.push_back(new Tegla(j, i, 50, 20, r, g, b));
            osszes_tegla++;
        }
        sor++;
    }

    gin.timer(20);

    event ev;

    while(gin >> ev && ev.keycode != key_escape && elet != 0)
    {
           if(ev.type == ev_mouse && ev.button == 0)
           {
                u->mozgat(ev.pos_x);
                u->valtoztat();
           }
           if(ev.type == ev_timer)
            {
                l->mozog();

                if(l->y_koord() + l->meret_nagysag() >= u->y_koord() &&
                    l->x_koord() + l->meret_nagysag() >= u->x_koord() &&
                    l->x_koord() <= u->x_koord() + u->szel_nagysag())
                {
                    l->visszapattan();
                }

                for(Tegla *teglak : t)
                {
                    if(teglak->el() &&
                        l->x_koord() + l->meret_nagysag() >= teglak->x_koord() &&
                        l->x_koord() <= teglak->x_koord() + teglak->szel_nagysag() &&
                        l->y_koord() + l->meret_nagysag() >= teglak->y_koord() &&
                        l->y_koord() <= teglak->y_koord() + teglak->mag_nagysag())
                    {
                        l->visszapattan();
                        teglak->nem_el();
                        osszes_tegla--;
                    }
                }

                vector<Tegla*> uj_tegla;
                int uj_osszes_tagla = 0;

                for (Tegla* teglak : t)
                {
                    if (teglak->el())
                    {
                        uj_tegla.push_back(teglak);
                        uj_osszes_tagla++;
                    }
                    else
                    {
                        delete teglak;
                    }
                }
                t = uj_tegla;
                osszes_tegla = uj_osszes_tagla;

                if(osszes_tegla == 0)
                {
                    winner();
                    return;
                }

                if(l->y_koord() > magassag)
                {
                    elet--;

                    if(elet == 0)
                    {
                        loser();
                        return;
                    }

                    l->pozicio(rand() % szelesseg, magassag / 2);
                    l->irany(3, - 3);

                }
            }

            gout << move_to(0, 0)
                 << color(0, 0, 0)
                 << box(szelesseg, magassag)
                 << move_to(20, 15)
                 << color(255, 255, 255)
                 << font("LiberationSans-Regular.ttf", 30)
                 << text("Lives remaining: " + to_string(elet) + "  Bricks remaining: " + to_string(osszes_tegla));

            u->kirajzol();
            l->kirajzol();

            for(Tegla *teglak : t)
                teglak->kirajzol();

            gout <<refresh;
    }
}



struct Gomb
{
protected:
    float _x, _y, _sx, _sy;
    bool _kijelolve;

public:
    Gomb(int x, int y, int sx, int sy) : _x(x), _y(y), _sx(sx), _sy(sy), _kijelolve(0)
    {

    }

    virtual void megrajzol() = 0;
    virtual void jatekmeghivas() = 0;

    bool egerhelye(int mx, int my)
    {
        return mx >= _x && mx < _x + _sx &&
               my >= _y && my < _y + _sy;
    }

    void kijelol()
    {
        _kijelolve = 1;
    }

    void elfelejt()
    {
        _kijelolve = 0;
    }

    bool kijelolve()
    {
        return _kijelolve;
    }
};

struct StartGomb: public Gomb
{
    StartGomb(int x, int y, int sx, int sy): Gomb(x, y, sx, sy)
    {

    }

    virtual void megrajzol()
    {
        int menu_kozep_x = _x + _sx / 2 - 30;
        int menu_kozep_y = _y + _sy / 2 - 12;

        if(_kijelolve)
            gout << move_to(_x, _y)
                 << color(255, 255, 255)
                 << box(_sx, _sy)
                 << move_to(_x + 5, _y + 5)
                 << color(100, 100, 100)
                 << box( _sx - 10, _sy - 10)
                 << move_to(menu_kozep_x, menu_kozep_y);

        else
            gout << move_to(_x, _y)
                 << color(255, 255, 255)
                 << box(_sx, _sy)
                 << move_to(_x + 3, _y + 3)
                 << color(51, 255, 255)
                 << box( _sx - 6, _sy - 6)
                 << move_to(menu_kozep_x, menu_kozep_y);

            gout << color(0, 0, 0)
                 << text("START");
    }

    virtual void jatekmeghivas()
    {
        jatek();
    }
};

struct RestartGomb: public Gomb
{
public:
    RestartGomb(int x, int y, int sx, int sy): Gomb(x, y, sx, sy)
    {

    }

    virtual void megrajzol()
    {
        _x = (1000 - _sx) / 2;
        _y = (800 - _sx) / 2 + 200;

        int menu_kozep_x = _x + (_sx / 2) - 40;
        int menu_kozep_y = _y + (_sy / 2) - 12;

        if(_kijelolve)
            gout << move_to(_x, _y)
                 << color(255, 255, 255)
                 << box(_sx, _sy)
                 << move_to(_x + 5, _y + 5)
                 << color(100, 100, 100)
                 << box( _sx - 10, _sy - 10)
                 << move_to(menu_kozep_x, menu_kozep_y);

        else
            gout << move_to(_x, _y)
                 << color(255, 255, 255)
                 << box(_sx, _sy)
                 << move_to(_x + 3, _y + 3)
                 << color(51, 255, 255)
                 << box( _sx - 6, _sy - 6)
                 << move_to(menu_kozep_x, menu_kozep_y);

            gout << color(0, 0, 0)
                 << text("RESTART");
    }

    virtual void jatekmeghivas()
    {
        elet = 3;
        jatek();
    }
};

void kezdokepernyo()
{
    gout.open(szelesseg,magassag);

    vector<Gomb*> v;

    gout << color(120, 100, 200)
         << box(szelesseg - 1, magassag - 1)
         << color(0, 0, 0)
         << move_to(szelesseg / 2 - 150, magassag / 2 - 100)
         << text("******WALLBREAK******")
         << move_to(50, 25)
         << text("Press ESC to exit")
         << refresh;

    v.push_back(new StartGomb(szelesseg / 2 - 115, magassag / 2 - 50,
                             150, 50));

    for(Gomb* b: v)
        b->megrajzol();

    gout << refresh;

    event ev;
    while(gin >> ev && ev.keycode != key_escape)
    {
        if(ev.type == ev_mouse && ev.button == btn_left)
        {
            for(Gomb* g : v)
            {
                if(g->egerhelye(ev.pos_x, ev.pos_y))
                {
                    g->kijelol();
                    g->megrajzol();

                    gout << refresh;

                    g->jatekmeghivas();
                    return;
                }
            }
        }

        gout << move_to(0, 0)
                 << color(120, 100, 200)
                 << box(szelesseg - 1, magassag - 1)
                 << color(0, 0, 0)
                 << font("LiberationSans-Regular.ttf",20)
                 << move_to(szelesseg / 2 - 150, magassag / 2 - 150)
                 << text("******WALLBREAK******")
                 << move_to(50, 25)
                 << text("Press ESC to exit")
                 << box(szelesseg, magassag);

            for(Gomb* g: v)
                g->megrajzol();

            gout << refresh;
    }
}

void loser()
{

    gout.open(szelesseg,magassag);

    vector<Gomb*> v;

    gout << color(250, 0, 100)
         << box(szelesseg - 1, magassag - 1)
         << color(0, 0, 0)
         << move_to(szelesseg / 2 - 175, magassag / 2 + 300)
         << text("******YOU LOST******")
         << move_to(50, 25)
         << text("Press ESC to exit")
         << refresh;

    v.push_back(new RestartGomb(szelesseg / 2 - 115, magassag / 2 - 50,
                             150, 50));

    for(Gomb* b: v)
        b->megrajzol();

    gout << refresh;

    event ev;
    while(gin >> ev && ev.keycode != key_escape)
    {
        if(ev.type == ev_mouse && ev.button == btn_left)
        {
            for(Gomb* g : v)
            {
                if(g->egerhelye(ev.pos_x, ev.pos_y))
                {
                    g->kijelol();
                    g->megrajzol();

                    gout << refresh;

                    g->jatekmeghivas();
                    return;
                }
            }
        }

        gout << move_to(0, 0)
                 << color(100, 100, 100)
                 << box(szelesseg - 1, magassag - 1)
                 << color(0, 0, 0)
                 << font("LiberationSans-Regular.ttf",20)
                 << move_to(szelesseg / 2 - 100, magassag / 2 - 10)
                 << text("******YOU LOST******")
                 << move_to(50, 25)
                 << text("Press ESC to exit")
                 << box(szelesseg, magassag);

            for(Gomb* g: v)
                g->megrajzol();

            gout << refresh;
    }
}

void winner()
{

    gout.open(szelesseg,magassag);

    vector<Gomb*> v;

    gout << color(120, 250, 200)
         << box(szelesseg - 1, magassag - 1)
         << color(0, 0, 0)
         << move_to(szelesseg / 2 - 175, magassag / 2 + 300)
         << text("******YOU WON!!******")
         << move_to(50, 25)
         << text("Press ESC to exit")
         << refresh;

    v.push_back(new RestartGomb(szelesseg / 2 - 115, magassag / 2 - 50,
                             150, 50));

    for(Gomb* b: v)
        b->megrajzol();

    gout << refresh;

    event ev;
    while(gin >> ev && ev.keycode != key_escape)
    {
        if(ev.type == ev_mouse && ev.button == btn_left)
        {
            for(Gomb* g : v)
            {
                if(g->egerhelye(ev.pos_x, ev.pos_y))
                {
                    g->kijelol();
                    g->megrajzol();

                    gout << refresh;

                    g->jatekmeghivas();
                    return;
                }
            }
        }

        gout << move_to(0, 0)
                 << color(120, 250, 200)
                 << box(szelesseg - 1, magassag - 1)
                 << color(0, 0, 0)
                 << font("LiberationSans-Regular.ttf",20)
                 << move_to(szelesseg / 2 - 100, magassag / 2 - 10)
                 << text("******YOU WON!!******")
                 << move_to(50, 25)
                 << text("Press ESC to exit")
                 << box(szelesseg, magassag);

            for(Gomb* g: v)
                g->megrajzol();

            gout << refresh;
    }
}

int main()
{
    kezdokepernyo();

    return 0;
}
