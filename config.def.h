/********************************************|
      ██████╗ ███████╗███████╗ █████╗ 's
      ██╔══██╗██╔════╝╚══███╔╝██╔══██╗
      ██████╔╝█████╗    ███╔╝ ███████║
      ██╔══██╗██╔══╝   ███╔╝  ██╔══██║
      ██║  ██║███████╗███████╗██║  ██║
      ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝
    Custom Dynamic Window Manager Build
*********************************************/

/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 1;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const Bool viewontag         = True;     /* Switch view on tag switch */
static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
static const char *alttrayname      = "tray";    /* Polybar tray instance name */
static const char *altbarcmd        = "$HOME/.config/polybar/launch-polybar.sh"; /* Alternate bar launch command */
static const char *fonts[]          = { "Fira Sans Regular:size=10" };
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

static const char *const autostart[] = {
	"powertop", "--auto-tune", NULL,
  /* this is to set the brightness - for in door ffs */
  "xbacklight", "-set", "5%", NULL,
  /* xset configuration for smoother cursor */
  "xset", "r", "rate", "200", "60", NULL,
  /* Backup Packages lists */
  "pacman", "-Qen", ">", "packages-repository.txt", NULL,
  "pacman", "-Qem", ">", "packages-AUR.txt", NULL,
	/* Fonts */
	"xset", "fp+", "/usr/share/fonts", NULL,
	"xset", "fp", "rehash", NULL,
	/* App autostart */
  "bitwarden", NULL,
  "nautilus", "--daemon", NULL,
	/* Keyboard Map setting */
	"setxkbmap", "-layout", "gb", "-option", "ctrl:nocaps", NULL,
	/* Important Module for WMs*/
	"nitrogen", "--restore", NULL,
	"picom", "--experimental-backends", NULL,
	"dunst", NULL,
  "unclutter", "--timeout", "5", "--jitter", "2", "--ignore-scrolling", NULL,
  /* pipewire daemon */
  "pipewire", NULL,
  "pipewire-pulse", NULL,
  "pipewire-media-session", NULL,
  /* System tray configurations */
	"syncthing-gtk", "-m", NULL,
  "redshift-gtk", "-l", "-7.24:112.75", NULL, // nightlight setting based on Surabaya
	"nm-applet", NULL,
	"volumeicon", NULL,
	"blueman-applet", NULL,
	"xfce4-power-manager", NULL, /* also managing its own dpms and power saving setting */
	"parcellite", NULL,
  // "persepolis", "--tray", NULL,
  /* Polkit */
  "/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 *  WM_WINDOW_ROLE(STRING) = role
	 */
	/* class                role  instance    title       tags mask  isterminal  noswallow          isfloating   monitor */
        /* 1 - Terminals */
        { "Alacritty",          NULL, NULL,       NULL,       1,         1,          0,                  0,           -1 },
	{ "kitty",              NULL, NULL,       NULL,       1,         1,          0,                  0,           -1 },
	{ "URxvt",              NULL, NULL,       NULL,       1,         1,          0,                  0,           -1 },
        /* 2 - Web Browser */
	{ "Brave-browser",      NULL, NULL,       NULL,       1 << 1,    0,          0,                  0,           -1 },
	{ "LibreWolf",          NULL, NULL,       NULL,       1 << 1,    0,          0,                  0,           -1 },
        /* 3 - Note-taking */
	{ "Notion",             NULL, NULL,       NULL,       1 << 2,    0,          0,                  0,           -1 },
	{ "Joplin",             NULL, NULL,       NULL,       1 << 2,    0,          0,                  0,           -1 },
	{ "Emacs",              NULL, NULL,       NULL,       1 << 2,    0,          0,                  0,           -1 },
        /* 4 - File Browser */
	{ "Org.gnome.Nautilus", NULL, NULL,       NULL,       1 << 3,    0,          0,                  0,           -1 },
	{ "dolphin",            NULL, NULL,       NULL,       1 << 3,    0,          0,                  0,           -1 },
	{ "Pcmanfm",            NULL, NULL,       NULL,       1 << 3,    0,          0,                  0,           -1 },
	{ "Nemo",               NULL, NULL,       NULL,       1 << 3,    0,          0,                  0,           -1 },
        /* 5 - IDE/Development */
	{ "VSCodium",           NULL, NULL,       NULL,       1 << 4,    0,          0,                  0,           -1 },
	{ "Chromium",           NULL, NULL,       NULL,       1 << 4,    0,          0,                  0,           -1 },
	{ "firefoxdeveloperedition",           NULL, NULL,       NULL,       1 << 4,    0,               1,             0,           -1 },
        /* 6 - Text editing/BibTex Management/Book Management */
	{ "TeXstudio",          NULL, NULL,       NULL,       1 << 5,    0,          0,                  0,           -1 },
	{ "calibre",            NULL, NULL,       NULL,       1 << 5,    0,          0,                  0,           -1 },
	{ "Zotero",             NULL, NULL,       NULL,       1 << 5,    0,          0,                  0,           -1 },
        /* 7 - Communication/Downloader */
	{ "All-in-One Messenger",             NULL, NULL,       NULL,       1 << 6,    0,               1,             0,           -1 },
	{ "zoom",               NULL, NULL,       NULL,       1 << 6,    0,          0,                  0,           -1 },
	{ "Transmission-gtk",   NULL, NULL,       NULL,       1 << 6,    0,          0,                  0,           -1 },
	{ "persepolis",         NULL, NULL,       NULL,       1 << 6,    0,          0,                  0,           -1 },
        /* 8 - Graphic Editing */
	{ "kdenlive",           NULL, NULL,       NULL,       1 << 7,    0,          0,                  0,           -1 },
	{ "krita",              NULL, NULL,       NULL,       1 << 7,    0,          0,                  0,           -1 },
	{ "Gimp",               NULL, NULL,       NULL,       1 << 7,    0,          0,                  0,           -1 },
	{ "Inkscape",           NULL, NULL,       NULL,       1 << 7,    0,          0,                  0,           -1 },
        /* 9 - Video and Media */
	{ "Steam",              NULL, NULL,       NULL,       1 << 8,    0,          0,                  0,           -1 },
	{ "Lutris",             NULL, NULL,       NULL,       1 << 8,    0,          0,                  0,           -1 },
	{ "Leonflix",           NULL, NULL,       NULL,       1 << 8,    0,          0,                  0,           -1 },
	{ "vlc",                NULL, NULL,       NULL,       1 << 8,    0,          0,                  0,           -1 },
	{ "Popcorn-Time-Ru",    NULL, NULL,       NULL,       1 << 8,    0,          0,                  0,           -1 },
	{ "Player",             NULL, NULL,       NULL,       1 << 8,    0,          0,                  0,           -1 },
	{ "mpv",                NULL, NULL,       NULL,       1 << 8,    0,          0,                  0,           -1 },
	{ "QMLPlayer",          NULL, NULL,       NULL,       1 << 8,    0,          0,                  0,           -1 },
	{ "Audacious",          NULL, NULL,       NULL,       1 << 8,    0,          0,                  0,           -1 },
	{ "Spotify",            NULL, NULL,       NULL,       1 << 8,    0,          0,                  0,           -1 },
	{ "elisa",              NULL, NULL,       NULL,       1 << 8,    0,          0,                  0,           -1 },
        /* Setting for programs that is not allowed to be swallowed */
	{ NULL,                 NULL, NULL,       "Event Tester",  0,    0,          0,                  0,           -1 },
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
static const char *termcmd[]  = { "alacritty", NULL };

// static const char *roficmd[] = { "rofi", "-show", "drun", NULL };
static const char *roficmd[] = { "./.config/rofi/launchers/misc/launcher.sh", NULL };
static const char *rofimojicmd[] = { "rofimoji", NULL };
static const char *emaevcmd[] = { "emacsclient", "--eval", "(emacs-everywhere)", NULL };
/* pulseaudio volume-control */
// static const char *mutecmd[] = { "./.suckless/dwm/scripts/pactl_volume.sh", "M", NULL };
// static const char *volupcmd[] = { "./.suckless/dwm/scripts/pactl_volume.sh", "+5%", NULL };
// static const char *voldowncmd[] = { "./.suckless/dwm/scripts/pactl_volume.sh", "-5%", NULL };
static const char *mutecmd[] = { "pactl", "set-sink-input-mute", "$current_sink_num", "toggle", NULL };
static const char *volupcmd[] = {  "pactl", "set-sink-input-volume", "$current_sink_num", "+5%", NULL };
static const char *voldowncmd[] = { "pactl", "set-sink-input-volume", "$current_sink_num", "-5%", NULL };
/* pulseaudio media button*/
static const char *plpacmd[] = { "playerctl", "play-pause", NULL };
static const char *nextcmd[] = { "playerctl", "next", NULL };
static const char *prevcmd[] = { "playerctl", "previous", NULL };
/* xbacklight control */
static const char *brupcmd[] = { "xbacklight", "-inc", "5", NULL };
static const char *brdowncmd[] = { "xbacklight", "-dec", "5", NULL };
/* Screenshot */
static const char *prn_scr_cmd[] = { "scrot", "-q100", "'$HOME/Pictures/Screenshots/'%Y-%m-%d_%H:%M:%S-Screenshot.png'", NULL };
/* lock menu */
// static const char *i3lockcmd[] = { "./.suckless/dwm/scripts/i3lock-fancy/i3lock-fancy", NULL };
static const char *lockcmd[] = { "dm-tools", "lock", NULL };

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = roficmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_e,      spawn,          {.v = rofimojicmd } },
	{ MODKEY|ShiftMask,             XK_e,      spawn,          {.v = emaevcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_p,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_k,      shiftviewclients, { .i = +1 } },
	{ MODKEY|ShiftMask,             XK_j,      shiftviewclients, { .i = -1 } },
	{ MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,		XK_comma,  cyclelayout,    {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period, cyclelayout,    {.i = +1 } },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_y,      togglefakefullscreen, {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscreen, {0} },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
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
	{ MODKEY|ControlMask|ShiftMask, XK_c,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_r,      quit,           {1} },
	{ MODKEY,                       XK_minus, scratchpad_show, {0} },
	{ MODKEY|ShiftMask,             XK_minus, scratchpad_hide, {0} },
	{ MODKEY,                       XK_equal,scratchpad_remove,{0} },
	{ 0, 				XF86XK_AudioMute,              spawn, {.v = mutecmd } },
	{ 0, 				XF86XK_AudioLowerVolume, spawn, {.v = voldowncmd } },
	{ 0, 				XF86XK_AudioRaiseVolume, spawn, {.v = volupcmd } },
	{ 0, 				XF86XK_MonBrightnessUp, spawn, {.v = brupcmd} },
	{ 0, 				XF86XK_MonBrightnessDown, spawn, {.v = brdowncmd} },
	{ 0, 				XF86XK_AudioPlay, spawn, {.v = plpacmd} },
	{ 0, 				XF86XK_AudioNext, spawn, {.v = nextcmd} },
	{ 0, 				XF86XK_AudioPrev, spawn, {.v = prevcmd} },
	{ 0, 				XK_Print, spawn, {.v = prn_scr_cmd} },
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

static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

/* On Empty Keys - run commands on empty tags using predetermined keys */
/* Programs */
static const char* bravecmd[] = {"brave", NULL};
static const char* chromiumcmd[] = {"chromium", NULL};
static const char* librewolfcmd[] = {"librewolf", NULL};
static const char* firefoxdevcmd[] = {"firefox-developer-edition", NULL};
static const char* nemocmd[] = {"nemo", NULL};
static const char* dolphincmd[] = {"dolphin", NULL};
static const char* emacscmd[] = {"zsh", "-i", "-c", "emacs", NULL};
static const char* zoterocmd[] = {"zotero", NULL};
static const char* spotifyadblockcmd[] = {"env", "LD_PRELOAD=/usr/local/lib/spotify-adblock.so", "spotify", "%U", NULL};
/* Power Menu */
static const char* poweroffcmd[] = {"loginctl", "poweroff", NULL};
static const char* rebootcmd[] = {"loginctl", "reboot", NULL};
static const char* hsleepcmd[] = {"./.suckless/dwm/scripts/i3lock-fancy/i3lock-fancy", "&&", "loginctl", "hybrid-sleep", NULL};
static const char* logoutcmd[] = {"pkill", "dwm", NULL};
/* Executions */
static Key on_empty_keys[] = {
	/* modifier key            function                argument */
	{ 0,        XK_a,          spawn,                  {.v = termcmd } },
	{ 0,        XK_b,          spawn,                  {.v = bravecmd } },
	{ 0,        XK_c,          spawn,                  {.v = chromiumcmd } },
	{ 0,        XK_w,          spawn,                  {.v = librewolfcmd } },
	{ 0,        XK_f,          spawn,                  {.v = firefoxdevcmd } },
	{ 0,        XK_n,          spawn,                  {.v = nemocmd } },
	{ 0,        XK_d,          spawn,                  {.v = dolphincmd } },
	{ 0,        XK_e,          spawn,                  {.v = emacscmd } },
	{ 0,        XK_z,          spawn,                  {.v = zoterocmd } },
	{ 0,        XK_s,          spawn,                  {.v = spotifyadblockcmd } },
	{ 0,        XK_h,          spawn,                  {.v = hsleepcmd } },
	{ 0,        XK_p,          spawn,                  {.v = poweroffcmd } },
	{ 0,        XK_r,          spawn,                  {.v = rebootcmd } },
	{ 0,        XK_k,          spawn,                  {.v = lockcmd } },
	{ 0,        XK_l,          spawn,                  {.v = logoutcmd } },
};
