/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 8;       /* snap pixel */
static const unsigned int gappih    = 13;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 13;       /* vert inner gap between windows */
static const unsigned int gappoh    = 13;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 13;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 1;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "dylex terminal:size=9", "Material Design Icons:size=11" };
static const char dmenufont[]       = "dylex terminal:size=9";
#include "theme.h"
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_normfg, col_normbg, col_normbord },
	[SchemeSel]  = { col_selfg, col_main,  col_main  },
};

/* tagging */
static const char *tags[] = { "I", "II", "III", "IV", "V"};

static const Rule rules[] = {
	//{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	//{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     		instance    title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Alacritty",		  NULL,     NULL,           0,         0,          1,           0,        -1,		680,	 270,	 1200,	 900,	 -1 },
	{ NULL,			"sxiv",     NULL,           0,         1,          1,           0,        -1, 		800,	 460,	 -1,	 -1,	 -1 },
	{ NULL,			 "mpv",     NULL,           0,         1,          1,           0,        -1, 		1000,	 560,	 -1,	 -1,	 -1 },
	{ NULL,			  NULL,     "spud",         0,         1,          1,           0,        -1, 		880,	 420,	 800,	 600,	 -1 },
	{ NULL,			  NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, 
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	//{ "[\\]",     dwindle },
	{ "H[]",      deck },
	//{ "TTT",      bstack },
	//{ "===",      bstackhoriz },
	//{ "HHH",      grid },
	//{ "###",      nrowgrid },
	//{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	//{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
	{ MOD, XK_semicolon,ACTION##stack, {.i = 0 } }, \
	{ MOD, XK_apostrophe, ACTION##stack, {.i = PREVSEL } }, \
	//{ MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	//{ MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	//{ MOD, XK_x,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_normbg, "-nf", col_normfg, "-sb", col_main, "-sf", col_selfg, NULL };
static const char *closercmd[] = { "closer", dmenufont, col_normbg, col_normfg, col_main, col_selfg, NULL };
static const char *termcmd[]  = { "alacritty", NULL};
static const char *browsercmd[] = { "chromium", NULL };
static const char *torsettercmd[] = { "torsetter", NULL };
static const char scratchpadname[] = "spud";
static const char *spadtermcmd[] = { "alacritty", "-t", scratchpadname,  NULL };
static const char *addtospadcmd[] = { "addtospad", NULL };
static const char *fzfpasscmd[] = { "fzfpass", "-c", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_b,      spawn,          {.v = dmenucmd } },
	{ MODKEY,             		XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,			XK_g,	   spawn,	   {.v = browsercmd } },
	{ MODKEY|ShiftMask,		XK_f,	   spawn,	   {.v = torsettercmd } },
	//{ MODKEY,			XK_s,	   togglescratch,  {.v = spadtermcmd } },
	{ MODKEY,			XK_v,	   spawn,	   {.v = addtospadcmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY|ShiftMask,             XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	//{ MODKEY,                    XK_semicolon, zoom,           {0} },
	{ MODKEY|Mod1Mask,              XK_i,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrgaps,       {.i = -1 } },
	//{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	//{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	//{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	//{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	//{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	//{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	//{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	//{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	//{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	//{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	//{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	//{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,             		XK_minus,      killclient,     {0} },
	/* Layouts */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, // tile
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[1]} }, // monocle
	{ MODKEY,                       XK_i,      setlayout,      {.v = &layouts[6]} }, // floating
	{ MODKEY,			XK_o,	   setlayout,	   {.v = &layouts[5]} }, // centeredmaster
	{ MODKEY,			XK_p,	   setlayout,	   {.v = &layouts[3]} }, // deck
	{ MODKEY,			XK_n,	   setlayout,	   {.v = &layouts[2]} }, // spiral
	{ MODKEY,			XK_m, 	   setlayout,	   {.v = &layouts[4]} }, // gaplessgrid

	//{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY,            		XK_space,  togglefloating, {0} },
	//{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY,           		XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_BackSpace, spawn,        {.v = closercmd} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

