/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "monospace:size=10", "JoyPixels:pixelsize=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	/*
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
	*/
	NULL
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

/** Function to shift the current view to the left/right
 *
 * @param: "arg->i" stores the number of tags to shift right (positive value)
 *          or left (negative value)
 */
void
shiftview(const Arg *arg) {
	Arg shifted;

	if(arg->i > 0) // left circular shift
		shifted.ui = (selmon->tagset[selmon->seltags] << arg->i)
		   | (selmon->tagset[selmon->seltags] >> (LENGTH(tags) - arg->i));

	else // right circular shift
		shifted.ui = selmon->tagset[selmon->seltags] >> (- arg->i)
		   | selmon->tagset[selmon->seltags] << (LENGTH(tags) + arg->i);

	view(&shifted);
}

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },   /* Dmenu */
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },    /* Terminal */
	{ MODKEY,                       XK_b,      togglebar,      {0} },                /* Hide Bar */
	{ ALTKEY,                       XK_Tab,    focusstack,     {.i = +1 } },         /* Change focused window */
	{ ALTKEY|ShiftMask,             XK_Tab,    focusstack,     {.i = -1 } },         /* Change focused window */
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },         /* Change window division */
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },         /* Change window division */
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },       /* Change window gap */
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },       /* Change window gap */
	{ MODKEY,                       XK_w,      zoom,           {0} },                /* Make master */
	/* { MODKEY,                       XK_Tab,    view,           {0} }, */                /* Toggle tags */
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },                /* Kill window */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, /* Set tiling layout */
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} }, /* Set monocle layout */
	/* { MODKEY|ShiftMask,             XK_s,      setlayout,      {.v = &layouts[2]} }, */ /* Set floating layout */
	{ MODKEY,                       XK_f,      setlayout,      {0} },                /* Toggle last layout */
	{ MODKEY,                       XK_s,      togglefloating, {0} },                /* Make window float */
	/* { MODKEY,                       XK_0,      view,           {.ui = ~0 } }, */        /* Show all tags */
	{ MODKEY,                       XK_p,      togglesticky,   {0} },                /* Pin window */
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },         /* Send to next screen */
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },         /* Send to next screen */
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },         /* Send to next screen */
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },         /* Send to next screen */
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },         /* Increase gaps */
	{ MODKEY,                       XK_plus,   setgaps,        {.i = +1 } },         /* Decrease gaps */
	/* { MODKEY|ShiftMask,             XK_0,      setgaps,        {.i = 0  } }, */         /* No gaps */
	{ MODKEY|ShiftMask,             XK_p,      togglecanfocusfloating,   {0} },      /* Toggle focus for floating windows */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	TAGKEYS(                        XK_0,                      9)
	{ MODKEY,                       XK_Next,   shiftview,      { .i = +1 } },        /* Switch to next tag */
	{ MODKEY,                       XK_Prior,  shiftview,      { .i = -1 } },        /* Switch to previous tag */
	{ MODKEY|ALTKEY,                XK_q,      quit,           {0} },                /* Kill Dwm */
	{ NULL,                     XF86XK_AudioLowerVolume,      spawn,           SHCMD("amixer sset Master 5%-") },
	{ NULL,                     XF86XK_AudioRaiseVolume,      spawn,           SHCMD("amixer sset Master 5%+") },
	{ NULL,                     XF86XK_AudioMute,             spawn,           SHCMD("amixer sset Master toggle") },
	{ NULL,                     XF86XK_MonBrightnessUp,       spawn,           SHCMD("xbacklight -inc 10") },
	{ NULL,                     XF86XK_MonBrightnessDown,     spawn,           SHCMD("xbacklight -dec 10") },
	{ ControlMask,                  XK_space,                     spawn,           SHCMD("switchkbd") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	/* { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} }, */
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

