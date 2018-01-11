#include <rsys/wind.h>
#include <rsys/vdriver.h>
#include "MenuMgr.h"

#include <iostream>
#include <vector>

using namespace Executor;


static std::vector<Rect> rootlessMenus;

void Executor::ROMlib_rootless_update()
{
    std::cout << "ROMlib_rootless_update\n";
#ifdef VDRIVER_ROOTLESS
    RgnHandle rgn = NewRgn();//MR(LM(GrayRgn));
    SetRectRgn(rgn, 0,0, CW(screenBitsX.bounds.right), CW(LM(MBarHeight)));

    for(WindowPeek wp = MR(LM(WindowList)); wp; wp = WINDOW_NEXT_WINDOW(wp))
    {
        if(WINDOW_VISIBLE_X(wp))
        {
            UnionRgn(rgn, WINDOW_STRUCT_REGION(wp), rgn);
        }
    }
    RgnHandle tmp = NewRgn();
    for(Rect& r : rootlessMenus)
    {
        RectRgn(tmp, &r);
        UnionRgn(rgn, tmp, rgn);
    }
    vdriver_set_rootless_region(rgn);
    DisposeRgn(tmp);
    DisposeRgn(rgn);
#endif
}

void Executor::ROMlib_rootless_openmenu(Rect r)
{
    std::cout << "ROMlib_rootless_openmenu\n";
#ifdef VDRIVER_ROOTLESS
    r.left   = CW(CW(r.left  ) - 1);
    r.top    = CW(CW(r.top   ) - 1);
    r.right  = CW(CW(r.right ) + 2);
    r.bottom = CW(CW(r.bottom) + 2);
    rootlessMenus.push_back(r);
    ROMlib_rootless_update();
#endif
}
void Executor::ROMlib_rootless_closemenu()
{
    std::cout << "ROMlib_rootless_closemenu\n";
#ifdef VDRIVER_ROOTLESS
    rootlessMenus.pop_back();
    ROMlib_rootless_update();
#endif
}
