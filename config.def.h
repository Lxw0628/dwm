/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int default_border = 0;   /* to switch back to default border after dynamic border resizing via keybinds */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 8;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 8;       /* vert inner gap between windows */
static const unsigned int gappoh    = 8;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 8;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails,display systray on the 1st monitor,False: display systray on last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int showtab            = showtab_never;
static const int toptab             = 0;        /* 0 means bottom tab */
static const int floatbar           = 0;        /* 1 means the bar will float(don't have padding),0 means the bar have padding */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 24;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const int horizpadbar        = 5;
static const int vertpadbar         = 11;
static const int vertpadtab         = 35;
static const int horizpadtabi       = 15;
static const int horizpadtabo       = 15;
static const int scalepreview       = 4;
static const int tag_preview        = 0;        /* 1 means enable, 0 is off */
static const int colorfultag        = 1;        /* 0 means use SchemeSel for selected non vacant tag */
static const int new_window_attach_on_end = 0; /*  1 means the new window will attach on the end; 0 means the new window will attach on the front,default is front */
static const int title_center       = 0;        /* 1 means enable, 0 is off*/
#define ICONSIZE user_bh ? (user_bh - 4) : (bh - 4) /* icon size */
#define ICONSPACING 5 /* space between icon and title */

// static const char *fonts[]          = {"Iosevka:style:medium:size=12" ,"JetBrainsMono Nerd Font Mono:style:medium:size=19" };
static const char *fonts[]          = { "Maple UI:style:Regular:size=12" };

// theme
#include "themes/catppuccin.h"

static const char *colors[][3]      = {
    /*                     fg       bg      border */
    [SchemeNorm]       = { gray3,   black,  gray2 },
    [SchemeSel]        = { gray4,   blue,   blue  },
    [SchemeTitle]      = { white,   black,  black }, // active window title
    [TabSel]           = { blue,    gray2,  black },
    [TabNorm]          = { gray3,   black,  black },
    [SchemeTag]        = { gray3,   black,  black },
    [SchemeTag1]       = { blue,    black,  black },
    [SchemeTag2]       = { red,     black,  black },
    [SchemeTag3]       = { orange,  black,  black },
    [SchemeTag4]       = { green,   black,  black },
    [SchemeTag5]       = { pink,    black,  black },
    [SchemeTag6]       = { blue,    black,  black },
    [SchemeTag7]       = { red,     black,  black },
    [SchemeTag8]       = { orange,  black,  black },
    [SchemeTag9]       = { green,   black,  black },
    [SchemeTag10]      = { pink,    black,  black },
    [SchemeLayout]     = { green,   black,  black },
    [SchemeBtnPrev]    = { green,   black,  black },
    [SchemeBtnNext]    = { yellow,  black,  black },
    [SchemeBtnClose]   = { red,     black,  black },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"kitty", "--name", "spterm", NULL };
const char *spcmd2[] = {"kitty", "--name", "spfm", "-e", "yazi", NULL };
// const char *spcmd3[] = {"keepassxc", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spfm",        spcmd2},
	// {"keepassxc",   spcmd3},
};

/* tagging */
// static char *tags[] = {"", "", "", "", ""};
static char *tags[] = {"一", "二", "三", "四", "五", "六", "七", "八", "九", "零"};

// static const char* eww[] = { "eww", "open" , "eww", NULL };

// static const Launcher launchers[] = {
//     /* command     name to display */
//     { eww,         "" },
// };

static const int tagschemes[] = {
    SchemeTag1, SchemeTag2, SchemeTag3, SchemeTag4, SchemeTag5, SchemeTag6, SchemeTag7, SchemeTag8, SchemeTag9, SchemeTag10
};

static const unsigned int ulinepad      = 5; /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke   = 2; /* thickness / height of the underline */
static const unsigned int ulinevoffset  = 0; /* how far above the bottom of the bar the line should appear */
static const int ulineall               = 0; /* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     iscentered   isfloating   monitor */
    { "Gimp",     NULL,       NULL,       0,            0,           1,          -1 },
    { "Firefox",  NULL,       NULL,       1 << 8,       0,           0,          -1 },
    { NULL,       "spterm",   NULL,       SPTAG(0),     0,           1,          -1 },
    { NULL,       "spfm",     NULL,       SPTAG(1),     0,           1,          -1 },
    { NULL,       "keepassxc",NULL,       SPTAG(2),     0,           0,          -1 },
    // { "eww",      NULL,       NULL,       0,            0,           1,          -1 },

    { NULL,       NULL,       "保存文件",        0,     0,           1,          -1}, // 浏览器保存文件      浮动
    { NULL,       NULL,       "图片查看器",      0,     0,           1,          -1}, // qq图片查看器        浮动
    { NULL,       NULL,       "图片查看",        0,     0,           1,          -1}, // 微信图片查看器      浮动
    { NULL,       NULL,       "图片预览",        0,     0,           1,          -1}, // 企业微信图片查看器  浮动
    { NULL,       NULL,       "Media viewer",    0,     0,           1,          -1}, // tg图片查看器        浮动
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "functions.h"


static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "[M]",      monocle },
    { "[@]",      spiral },
    { "[\\]",     dwindle },
    { "H[]",      deck },
    { "TTT",      bstack },
    { "===",      bstackhoriz },
    { "HHH",      grid },
    { "###",      nrowgrid },
    { "---",      horizgrid },
    { ":::",      gaplessgrid },
    { "|M|",      centeredmaster },
    { ">M>",      centeredfloatingmaster },
    { "><>",      NULL },    /* no layout function means floating behavior */
    { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(CHAIN,KEY,TAG) \
    { MODKEY,                       CHAIN,      KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           CHAIN,      KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             CHAIN,      KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, CHAIN,      KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { "kitty", NULL };
static const char *upvol[]            = { "/usr/bin/pactl", "set-sink-volume", "0", "+5%",     NULL };
static const char *downvol[]          = { "/usr/bin/pactl", "set-sink-volume", "0", "-5%",     NULL };
static const char *mutevol[]          = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };
// TODO: 使用脚本判断当前所处monitor，调用对应命令调亮度
static const char *light_up[]         = {"/usr/bin/light", "-A", "5", NULL};
static const char *light_down[]       = {"/usr/bin/light", "-U", "5", NULL};
static const char *firefoxcmd[]       = { "firefox-developer-edition", NULL };
static const char *dunstcloseallcmd[] = { "dunstctl", "close-all", NULL };
static const char *thunarcmd[]        = { "thunar", NULL };
static const char *layoutmenu_cmd     = "$HOME/.config/dwm/layoutmenu.sh";

static const Key keys[] = {
    /* modifier                         chain key   key         function        argument */

    // brightness and audio 
    {0,                                 -1,         XF86XK_AudioLowerVolume,        spawn,          {.v = downvol}},
    {0,                                 -1,         XF86XK_AudioMute,               spawn,          {.v = mutevol }},
    {0,                                 -1,         XF86XK_AudioRaiseVolume,        spawn,          {.v = upvol}},
    {0,                                 -1,         XF86XK_MonBrightnessUp,         spawn,          {.v = light_up}},
    {0,                                 -1,         XF86XK_MonBrightnessDown,       spawn,          {.v = light_down}},

    { Mod1Mask,                         -1,         XK_space,   spawn,          SHCMD("$HOME/.config/rofi/bin/launcher") },
    { MODKEY,                           -1,         XK_Return,  spawn,          SHCMD("kitty")},
    { MODKEY|ShiftMask,                 -1,         XK_q,       spawn,          SHCMD("$HOME/.config/rofi/bin/powermenu")},
    { ControlMask|Mod1Mask,             -1,         XK_space,   spawn,          SHCMD("$HOME/.config/rofi/bin/runner")},

    // applications
    { MODKEY,                           -1,         XK_w,       spawn,          {.v = firefoxcmd}},
    { MODKEY,                           -1,         XK_d,       spawn,          {.v = dunstcloseallcmd}},
    { MODKEY|ShiftMask,                 -1,         XK_d,       spawn,          SHCMD("$HOME/Scripts/dunst/toggle_dunst.sh")},
    { MODKEY,                           -1,         XK_e,       spawn,          {.v = thunarcmd}},

    // toggle stuff
    { MODKEY,                           XK_b,         XK_b,       togglebar,      {0} },
    { MODKEY|ControlMask,               -1,         XK_t,       togglegaps,     {0} },
    { MODKEY,                           -1,         XK_f,       togglefloating, {0} },
    { MODKEY,                           -1,         XK_g,       togglefullscr,  {0} },
    // { MODKEY,                           -1,         XK_Tab,     winview,        {0} },

    { MODKEY|ControlMask,               -1,         XK_w,       tabmode,        { -1 } },
    { MODKEY,                           -1,         XK_j,       focusstack,     {.i = +1 } },
    { MODKEY,                           -1,         XK_k,       focusstack,     {.i = -1 } },
    // { MODKEY,                           -1,         XK_i,       incnmaster,     {.i = +1 } },
    // { MODKEY,                           -1,         XK_d,       incnmaster,     {.i = -1 } },

    { MODKEY,                           -1,         XK_u,       togglescratch,  {.ui = 0 } },
    { MODKEY|ShiftMask,                 -1,         XK_e,       togglescratch,  {.ui = 1 } },
    // { MODKEY,                           -1,         XK_x,       togglescratch,  {.ui = 2 } },

    // shift view
    { MODKEY|ControlMask,               -1,         XK_h,       shiftview,      {.i = -1 } },
    { MODKEY|ControlMask,               -1,         XK_l,       shiftview,      {.i = +1 } },

    // change m,cfact sizes 
    { MODKEY,                           -1,         XK_h,       setmfact,       {.f = -0.05} },
    { MODKEY,                           -1,         XK_l,       setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,                 -1,         XK_h,       setcfact,       {.f = +0.25} },
    { MODKEY|ShiftMask,                 -1,         XK_l,       setcfact,       {.f = -0.25} },
    { MODKEY|ShiftMask,                 -1,         XK_o,       setcfact,       {.f =  0.00} },


    { MODKEY|ShiftMask,                 -1,         XK_j,       movestack,      {.i = +1 } },
    { MODKEY|ShiftMask,                 -1,         XK_k,       movestack,      {.i = -1 } },
    { MODKEY|ShiftMask,                 -1,         XK_Return,  zoom,           {0} },
    { Mod1Mask,                         -1,         XK_Tab,     view,           {0} },

    { MODKEY|ControlMask,               -1,         XK_j,       focussame,      {.i = +1 } },
    { MODKEY|ControlMask,               -1,         XK_k,       focussame,      {.i = -1 } },


    // overall gaps
    { MODKEY|ControlMask,               -1,         XK_i,       incrgaps,       {.i = +1 } },
    { MODKEY|ControlMask,               -1,         XK_d,       incrgaps,       {.i = -1 } },

    // inner gaps
    { MODKEY|ShiftMask,                 -1,         XK_i,       incrigaps,      {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,     -1,         XK_i,       incrigaps,      {.i = -1 } },

    // outer gaps
    { MODKEY|ControlMask,               -1,         XK_o,       incrogaps,      {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,     -1,         XK_o,       incrogaps,      {.i = -1 } },

    // inner+outer hori, vert gaps 
    { MODKEY|ControlMask,               -1,         XK_6,       incrihgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,     -1,         XK_6,       incrihgaps,     {.i = -1 } },
    { MODKEY|ControlMask,               -1,         XK_7,       incrivgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,     -1,         XK_7,       incrivgaps,     {.i = -1 } },
    { MODKEY|ControlMask,               -1,         XK_8,       incrohgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,     -1,         XK_8,       incrohgaps,     {.i = -1 } },
    { MODKEY|ControlMask,               -1,         XK_9,       incrovgaps,     {.i = +1 } },
    { MODKEY|ControlMask|ShiftMask,     -1,         XK_9,       incrovgaps,     {.i = -1 } },

    { MODKEY|ControlMask|ShiftMask,     -1,         XK_d,       defaultgaps,    {0} },

    // layout
    // { MODKEY,                           -1,         XK_t,       setlayout,      {.v = &layouts[0]} },
    // { MODKEY|ShiftMask,                 -1,         XK_f,       setlayout,      {.v = &layouts[1]} },
    // { MODKEY,                           -1,         XK_m,       setlayout,      {.v = &layouts[2]} },
    // { MODKEY|ControlMask,               -1,         XK_g,       setlayout,      {.v = &layouts[10]} },
    // { MODKEY|ControlMask|ShiftMask,     -1,         XK_t,       setlayout,      {.v = &layouts[13]} },
    { MODKEY|ShiftMask,                 -1,         XK_space,   setlayout,      {0} },
    { MODKEY|ControlMask,               -1,         XK_comma,   cyclelayout,    {.i = -1 } },
    { MODKEY|ControlMask,               -1,         XK_period,  cyclelayout,    {.i = +1 } },
    { MODKEY,                           -1,         XK_BackSpace,       view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,                 -1,         XK_BackSpace,       tag,            {.ui = ~0 } },
    { MODKEY,                           -1,         XK_comma,   focusmon,       {.i = -1 } },
    { MODKEY,                           -1,         XK_period,  focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,                 -1,         XK_comma,   tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,                 -1,         XK_period,  tagmon,         {.i = +1 } },

    // change border size
    { MODKEY|ShiftMask,                 XK_b,         XK_p,   setborderpx,    {.i = -1 } },
    { MODKEY|ShiftMask,                 XK_b,         XK_n,       setborderpx,    {.i = +1 } },
    { MODKEY|ShiftMask,                 XK_b,         XK_equal,       setborderpx,    {.i = default_border } },

    // kill dwm
    { MODKEY|ShiftMask|ControlMask,     -1,         XK_q,       spawn,        SHCMD("killall bar.sh dwm") },

    // kill window
    { MODKEY,                           -1,         XK_q,       killclient,     {0} },
    { MODKEY,                           -1,         XK_Escape,  forcekillclient,{0} },

    // restart
    { MODKEY|ShiftMask,                 -1,         XK_r,       restart,        {0} },

    // hide & restore windows
    { MODKEY,                           -1,         XK_n,       hidewin,        {0} },
    { MODKEY|ShiftMask,                 -1,         XK_n,       restorewin,     {0} },

    TAGKEYS(                            -1,         XK_1,                       0)
    TAGKEYS(                            -1,         XK_2,                       1)
    TAGKEYS(                            -1,         XK_3,                       2)
    TAGKEYS(                            -1,         XK_4,                       3)
    TAGKEYS(                            -1,         XK_5,                       4)
    TAGKEYS(                            -1,         XK_6,                       5)
    TAGKEYS(                            -1,         XK_7,                       6)
    TAGKEYS(                            -1,         XK_8,                       7)
    TAGKEYS(                            -1,         XK_9,                       8)
    TAGKEYS(                            -1,         XK_0,                       9)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    // { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkLtSymbol,          0,              Button3,        layoutmenu,     {0} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          SHCMD("kitty") },

    /* Keep movemouse? */
    /* { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} }, */

    /* placemouse options, choose which feels more naturalst:
    *    0 - tiled position is relative to mouse cursor
    *    1 - tiled position is relative to window center
    *    2 - mouse pointer warps to window center
    *
    * The moveorplace uses movemouse or placemouse depending on the floating state
    * of the selected client. Set up individual keybindings for the two if you want
    * to control these separately (i.e. to retain the feature to move a tiled window
    * into a floating position).
    */
    { ClkClientWin,         MODKEY,         Button1,        moveorplace,    { .i = 0 } },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkClientWin,         ControlMask|Mod1Mask,    Button1,        dragmfact,      {0} },
    { ClkClientWin,         ControlMask|Mod1Mask,    Button3,        dragcfact,      {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkTabBar,            0,              Button1,        focuswin,       {0} },
    { ClkTagBar,            0,              Button4,        shiftview,      { .i = -1 } },
    { ClkTagBar,            0,              Button5,        shiftview,      { .i = +1 } },
    { ClkTabPrev,           0,              Button1,        movestack,      { .i = -1 } },
    { ClkTabNext,           0,              Button1,        movestack,      { .i = +1 } },
    { ClkTabClose,          0,              Button1,        killclient,     {0} },
};
