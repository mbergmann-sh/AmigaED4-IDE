/*
 *  File:       schwurblomat.c
 *  Purpose:    AmigaOS 3.2 ReAction GUI for schwurbel you die Ohren up!
 *  Version:    1.0.24
 *  Date:       09.01.2024
 *  Author:     Micha B.
 *
 */
#include <clib/macros.h>
#include <clib/alib_protos.h>
#include <clib/compiler-specific.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/icon.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include <proto/window.h>
#include <proto/layout.h>
#include <proto/button.h>
#include <proto/scroller.h>
#include <proto/label.h>
#include <proto/bitmap.h>
#include <proto/texteditor.h>
#include <proto/requester.h>

#include <libraries/gadtools.h>
#include <reaction/reaction.h>
#include <intuition/gadgetclass.h>
#include <intuition/icclass.h>
#include <reaction/reaction_macros.h>
#include <classes/window.h>
#include <classes/requester.h>
#include <exec/memory.h>

#include "schwurbel.h"

#define PROGRAMNAME     "Schwurb-Lo-Mat"
#define VERSION         "1"
#define SUBVERSION      "0"
#define REVISION        "28"


/* construct version tag */
#if defined(__SASC)
const UBYTE VersionTag[] = "$VER: " PROGRAMNAME " v" VERSION "." SUBVERSION "." REVISION " " __AMIGADATE__ "\n\0";
#endif
char progVersion[256] = "";

#define TextEditor_GetClass() TEXTEDITOR_GetClass()

struct Screen	*gScreen = NULL;
struct DrawInfo	*gDrawInfo = NULL;
APTR gVisinfo = NULL;
struct MsgPort	*gAppPort = NULL;

struct Library  *WindowBase = NULL,
                *ButtonBase = NULL,
                *ScrollerBase = NULL,
                *TextFieldBase = NULL,
                *LabelBase = NULL,
                *BitMapBase = NULL,
                *GadToolsBase = NULL,
                *LayoutBase = NULL,
                *RequesterBase = NULL,
                *IconBase = NULL;

struct IntuitionBase *IntuitionBase = NULL;

/* --- other global variables ------------------------------------------------ */
char antwort[512];  // geschwurbel() answer
//Window_3
enum window_3_idx { layout_5, layout_6, bitmap_7, layout_8, layout_9, texteditor_10,
                    scroller_29, layout_11, button_12, button_13 };
enum window_3_id { layout_5_id = 5, layout_6_id = 19, bitmap_7_id = 21, layout_8_id = 6,
                   layout_9_id = 7, texteditor_10_id = 12, scroller_29_id = 29,
                   layout_11_id = 8, button_12_id = 9, button_13_id = 10 };

/* -- Easy Request structures -- */
struct EasyStruct warnreq =
  {
    sizeof(struct EasyStruct),
    0,
    "Fehler",
    "Schwurblomat benötigt AmigaOS 3.2.x\nProgrammstart nicht möglich.",
    "Ok"
  };

struct EasyStruct nogfxreq =
  {
    sizeof(struct EasyStruct),
    0,
    "Problem",
    "Gfx not found\nApp start abandoned",
    "Ok"
  };
  
/* --- Gadget IDs ------------------------------------------------------------ */
#define GID_BTN_AUTOSCHWURBEL button_12
#define GAD_BTN_AUTOSCHWURBEL button_12_id
#define GID_BTN_SCHWURBEL     button_13
#define GAD_BTN_SCHWURBEL     button_13_id
#define GID_TXT_EDITOR        texteditor_10
#define GAD_TXT_EDITOR        texteditor_10_id

/* --- Menu IDs -------------------------------------------------------------- */
enum menus
{
  /* Menu "Projekt" */
  MN_SCHWURBEL, MN_AUTOSCHWURBEL, MN_TEXTCLEAR,
  MN_ABOUT, MN_QUIT,
  /* Menu "Voreinstellungen" */
  // Verzögerung
  MN_V1, MN_V3, MN_V5, MN_V8, MN_V10,
  // Wiederholungen
  MN_W5, MN_W10, MN_W20, MN_W30, MN_W50, MN_W100,
  // TextEditor löschen
  MN_DELETE,
  // Einleitungssatz anzeigen
  MN_LEADIN,
  // Benachrichtigung nach loop
  MN_REQUEST,
  // Debug-Ausgabe
  MN_DEBUG,
};

/* --- GadgetHelp --- */
struct HintInfo hintinfo[] =
  {
    //{ texteditor_10_id, -1, "Zeig mir den ganzen Unfug!...", 0L},
    { button_12_id, -1, "Serien-Geschwurbel...", 0L},
    { button_13_id, -1, "Denk' dir irgendwelchen geschmacklosen\n Blödsinn aus und erzähl' ihn der Welt!", 0L},
    { -1, -1, NULL, 0L}   // terminate!
  };

/* --- Function Prototypes --------------------------------------------------- */
void runWindow( Object *window_object, int window_id, struct Menu *menu_strip, struct Gadget *win_gadgets[] );
void window_3( void );
STRPTR geschwurbel(void);
int RandomNumber(const int nMin, const int nMax, const int  nNumOfNumsToGenerate);
int doInfoRequest(struct Screen *screen, struct Window *window, const char *title, const char *body,
                  const char *buttons, ULONG image );
void doSchwurbel(struct Window *win, struct Gadget *textgad, Object *window_object);
void doAutoSchwurbel(struct Window *win, struct Gadget *textgad, Object *window_object);


/* --- doAutoSchwurbel() ---*/
void doSchwurbel(struct Window *win, struct Gadget *textgad, Object *window_object)
{
  STRPTR schwurbelanswer;
  
  if(myDebug)
  {
    printf("myClear = %ld\n", myClear);
  }
  
  if(myClear)
  {
    DoGadgetMethod(textgad, win, NULL, GM_TEXTEDITOR_ClearText, NULL);  
  }
  
  // reset loop counter
  count = 0;
  
  // first schwurbel run
  schwurbelanswer = geschwurbel();
  if(!(myClear))
    DoGadgetMethod(textgad, win, NULL, GM_TEXTEDITOR_InsertText, NULL,"\n\n", GV_TEXTEDITOR_InsertText_Bottom);
  /* add generated sentence: */
  DoGadgetMethod(textgad, win, NULL, GM_TEXTEDITOR_InsertText, NULL,
                 schwurbelanswer, GV_TEXTEDITOR_InsertText_Bottom);
                 
  DoGadgetMethod(textgad, win, NULL, GM_TEXTEDITOR_InsertText, NULL,"\n\n", GV_TEXTEDITOR_InsertText_Bottom);
  
}


/* --- doAutoSchwurbel() ---*/
void doAutoSchwurbel(struct Window *win, struct Gadget *textgad, Object *window_object)
{
  int myLoops = loops;
  STRPTR schwurbelanswer;
  
  if(myDebug)
  {
    printf("myClear = %ld\n", myClear);
    printf("\ncount = %ld, loops = %ld, myLoops = %ld\n", count, loops, myLoops);
  } 
  
  if(myClear)
  {
    DoGadgetMethod(textgad, win, NULL, GM_TEXTEDITOR_ClearText, NULL);  
  }
  
  // reset loop counter
  count = 0;
  
  // first schwurbel run
  schwurbelanswer = geschwurbel();
  if(!(myClear))
    DoGadgetMethod(textgad, win, NULL, GM_TEXTEDITOR_InsertText, NULL,"\n\n", GV_TEXTEDITOR_InsertText_Bottom);
  /* add generated sentence: */
  DoGadgetMethod(textgad, win, NULL, GM_TEXTEDITOR_InsertText, NULL,
                 schwurbelanswer, GV_TEXTEDITOR_InsertText_Bottom);
                 
  DoGadgetMethod(textgad, win, NULL, GM_TEXTEDITOR_InsertText, NULL,"\n\n", GV_TEXTEDITOR_InsertText_Bottom);
                 
  // schwurbel loop run
  for (count; count < (loops - 1); count++)
  {
    busyWait(mydelay);   // POLL!

    schwurbelanswer = geschwurbel();

    
    /* add generated sentence: */
    DoGadgetMethod(textgad, win, NULL, GM_TEXTEDITOR_InsertText, NULL,
                   schwurbelanswer, GV_TEXTEDITOR_InsertText_Bottom);
                   
    DoGadgetMethod(textgad, win, NULL, GM_TEXTEDITOR_InsertText, NULL,"\n\n", GV_TEXTEDITOR_InsertText_Bottom);
  }
  /* after loop has finished:*/
  if(myRequest)
  {
    sprintf(progVersion, " %s v%s.%s.%s \n %s ", PROGRAMNAME, VERSION, SUBVERSION, REVISION, "Status: Ausgeschwurbelt");
    SetAttrs(window_object, WA_BusyPointer, TRUE, TAG_DONE);
    doInfoRequest(NULL, win, "Auto-Schwurbel Status", progVersion, "Besser is' das!", REQIMAGE_INFO);
    SetAttrs(window_object, WA_BusyPointer, FALSE, TAG_DONE);
  }
  
}


int setup( void )
{
  if( !(IntuitionBase = (struct IntuitionBase*) OpenLibrary("intuition.library",0L)) )
    return 0;
  if( !(GadToolsBase = (struct Library*) OpenLibrary("gadtools.library",0L) ) )
    return 0;
  if( !(WindowBase = (struct Library*) OpenLibrary("window.class",0L) ) )
    return 0;
  if( !(IconBase = (struct Library*) OpenLibrary("icon.library",0L) ) )
    return 0;
  if( !(LayoutBase = (struct Library*) OpenLibrary("gadgets/layout.gadget",47L) ) )
  {
    EasyRequest(NULL, &warnreq, NULL);
    return 0;
  }
  if( !(ButtonBase = (struct Library*) OpenLibrary("gadgets/button.gadget",0L) ) )
    return 0;
  if( !(RequesterBase = (struct Library*) OpenLibrary("requester.class",0L) ) )
    return 0;
  if( !(BitMapBase = (struct Library*) OpenLibrary("images/bitmap.image",0L) ) )
    return 0;
  if( !(ScrollerBase = (struct Library*) OpenLibrary("gadgets/scroller.gadget",0L) ) )
    return 0;
  if( !(LabelBase = (struct Library*) OpenLibrary("images/label.image",0L) ) )
    return 0;
  if( !(TextFieldBase = (struct Library*) OpenLibrary("gadgets/texteditor.gadget",0L) ) )
    return 0;
  if( !(gScreen = LockPubScreen( 0 ) ) )
    return 0;
  if( !(gVisinfo = GetVisualInfo( gScreen, TAG_DONE ) ) )
    return 0;
  if( !(gDrawInfo = GetScreenDrawInfo ( gScreen ) ) )
    return 0;
  if( !(gAppPort = CreateMsgPort() ) )
    return 0;
    
  if(!(fileExists("PROGDIR:schwurblomat.ilbm")))
  {
    EasyRequest(NULL, &nogfxreq, NULL);
    return 0; 
  }

  return -1;
}

void cleanup( void )
{
  if ( gDrawInfo )
    FreeScreenDrawInfo( gScreen, gDrawInfo);
  if ( gVisinfo )
    FreeVisualInfo( gVisinfo );
  if ( gAppPort )
    DeleteMsgPort( gAppPort );
  if ( gScreen )
    UnlockPubScreen( 0, gScreen );

  if (GadToolsBase)
    CloseLibrary( (struct Library *)GadToolsBase );
  if (IconBase)
    CloseLibrary( (struct Library *)IconBase );
  if (IntuitionBase)
    CloseLibrary( (struct Library *)IntuitionBase );
  if (RequesterBase)
    CloseLibrary( (struct Library *)RequesterBase );
  if (ButtonBase)
    CloseLibrary( (struct Library *)ButtonBase );
  if (BitMapBase)
    CloseLibrary( (struct Library *)BitMapBase );
  if (ScrollerBase)
    CloseLibrary( (struct Library *)ScrollerBase );
  if (LabelBase)
    CloseLibrary( (struct Library *)LabelBase );
  if (TextFieldBase)
    CloseLibrary( (struct Library *)TextFieldBase );
  if (LayoutBase)
    CloseLibrary( (struct Library *)LayoutBase );
  if (WindowBase)
    CloseLibrary( (struct Library *)WindowBase );
}

void runWindow( Object *window_object, int window_id, struct Menu *menu_strip, struct Gadget *win_gadgets[] )
{
  struct Window	*main_window = NULL;
  struct MenuItem *menuitem = NULL;
  int req_result = 0;
  LONG myItem = 0;
  STRPTR schwurbelanswer;
  
  if ( window_object )
  {
    if ( main_window = (struct Window *) RA_OpenWindow( window_object ))
    {
      WORD Code;
      ULONG wait = 0, signal = 0, result = 0, done = FALSE;
      GetAttr( WINDOW_SigMask, window_object, &signal );
      if ( menu_strip)
        SetMenuStrip( main_window, menu_strip );
      while ( !done)
      {
        wait = Wait( signal | SIGBREAKF_CTRL_C );

        if ( wait & SIGBREAKF_CTRL_C )
          done = TRUE;
        else
          while (( result = RA_HandleInput( window_object, &Code )) != WMHI_LASTMSG)
          {
            switch ( result & WMHI_CLASSMASK )
            {
                case WMHI_CLOSEWINDOW:
                SetAttrs(window_object, WA_BusyPointer, TRUE, TAG_DONE);
                req_result = doInfoRequest(NULL, main_window, "Schwurblomat beenden", \
                                           " Möchten Sie wirklich auf weitere \n alternative Fakten verzichten? ",
                                           "_Hau ab!|_Nee, doch nicht", REQIMAGE_QUESTION);
                if (req_result == 1)
                  done = TRUE;
                SetAttrs(window_object, WA_BusyPointer, FALSE, TAG_DONE);
                break;

                case WMHI_MENUPICK:
                menuitem = ItemAddress(menu_strip, result & WMHI_MENUMASK);
                if(myDebug)
                  printf("ItemAdress = %ld\n", menuitem);
                // Check if menuitem equals to Zero. If Zero, an enforcer hit occours,
                // so we do not allow switching on menuitem == 0!
                if(!(menuitem == 0))
                {
                  /* --- ...get menu entries by using GadTools' GTMENUITEM_USERDATA field macro --- */
                  myItem = ((long)GTMENUITEM_USERDATA(ItemAddress(menu_strip, result & WMHI_MENUMASK)));
                  switch(myItem)
                  {
                      /* --- Menu: Schwurbel --- */
                      case MN_SCHWURBEL:
                        doSchwurbel(main_window, win_gadgets[GID_TXT_EDITOR], window_object);
                        done = FALSE;	// Workaround for ReAction stack corruption bug                      
                        break;

                      /* --- Menu: Auto-Schwurbel --- */
                      case MN_AUTOSCHWURBEL:                      
                        // reset loop counter
                        count = 0;
                        doAutoSchwurbel(main_window, win_gadgets[GID_TXT_EDITOR], window_object);
                        done = FALSE;	// Workaround for ReAction stack corruption bug
                        break;

                      case MN_TEXTCLEAR:
                        DoGadgetMethod(win_gadgets[GID_TXT_EDITOR], main_window, NULL, GM_TEXTEDITOR_ClearText, NULL);
                        if(myDebug)
                          puts("textclear!");
                        break;

                      case MN_ABOUT:
                        sprintf(progVersion, "          SCHWURB-LO-MAT v%s.%s.%s \n %s ", VERSION, SUBVERSION, REVISION,
                                " - ein Programm für alternative Fakten - " \
                                " \n\n  geschrieben von Micha B. im Januar 2024\n\n" \
                                " GUI erstellt mit ReBuild 0.10 beta\n (c) Darren Coles "
                               );
                        SetAttrs(window_object, WA_BusyPointer, TRUE, TAG_DONE);
                        doInfoRequest(NULL, main_window, "Über Schwurblomat", progVersion, "Alles klar!", REQIMAGE_INFO);
                        SetAttrs(window_object, WA_BusyPointer, FALSE, TAG_DONE);
                        done = FALSE;	// Workaround for ReAction stack corruption bug
                        break;

                      case MN_QUIT:
                        SetAttrs(window_object, WA_BusyPointer, TRUE, TAG_DONE);
                        req_result = doInfoRequest(NULL, main_window,  "Schwurblomat beenden", \
                                                   " Möchten Sie wirklich auf weitere \n alternative Fakten verzichten? ",
                                                   "_Hau ab!|_Nee, doch nicht", REQIMAGE_QUESTION);
                        if (req_result == 1)
                          done = TRUE;
                        SetAttrs(window_object, WA_BusyPointer, FALSE, TAG_DONE);
                        break;

                      case MN_V1:
                      if(myDebug)
                        puts("V1");
                      mydelay = 1;
                      break;

                      case MN_V3:
                      if(myDebug)
                        puts("V3");
                      mydelay = 3;
                      break;

                      case MN_V5:
                      if(myDebug)
                        puts("V5");
                      mydelay = 5;
                      break;

                      case MN_V8:
                      if(myDebug)
                        puts("V8");
                      mydelay = 8;
                      break;

                      case MN_V10:
                      if(myDebug)
                        puts("V10");
                      mydelay = 10;
                      break;

                      case MN_W5:
                      if(myDebug)
                        puts("W5");
                      loops = 5;
                      break;

                      case MN_W10:
                      if(myDebug)
                        puts("W10");
                      loops = 10;
                      break;

                      case MN_W20:
                      if(myDebug)
                        puts("W20");
                      loops = 20;
                      break;

                      case MN_W30:
                      if(myDebug)
                        puts("W30");
                      loops = 30;
                      break;

                      case MN_W50:
                      if(myDebug)
                        puts("W50");
                      loops = 50;
                      break;

                      case MN_W100:
                      if(myDebug)
                        puts("W100");
                      loops = 100;
                      break;

                      case MN_DELETE:
                      if(myDebug)
                        puts("clear output before?");
                      if(menuitem->Flags & CHECKED)
                      {
                        if(myDebug)
                          puts("Clear!");
                        myClear = TRUE;
                      }
                      else
                      {
                        if(myDebug)
                          puts("Don't clear.");
                        myClear = FALSE;
                      }
                      break;

                      case MN_LEADIN:
                      if(menuitem->Flags & CHECKED)
                      {
                        printLeadIn = TRUE;
                        if(myDebug)
                          puts("LeadIn active!");
                      }
                      else
                      {
                        printLeadIn = FALSE;
                        if(myDebug)
                          puts("no LeadIn.");
                      }
                      break;

                      case MN_REQUEST:
                      if(menuitem->Flags & CHECKED)
                      {
                        myRequest = TRUE;
                        if(myDebug)
                          puts("Request active!");
                      }
                      else
                      {
                        myRequest = FALSE;
                        if(myDebug)
                          puts("no Request.");
                      }
                      break;

                      case MN_DEBUG:
                      if(myDebug)
                        puts("Debugging...");
                      if(menuitem->Flags & CHECKED)
                      {
                        myDebug = TRUE;
                        if(myDebug)
                          puts("Debugging active!");
                      }
                      else
                      {
                        myDebug = FALSE;
                        if(myDebug)
                          puts("no debug.");
                      }
                      break;
                  }
                }
                break;

                case WMHI_GADGETUP:
                switch (result & WMHI_GADGETMASK)
                {
                    /* -- SCHWURBEL --- */
                    case GAD_BTN_SCHWURBEL:
                    if(myDebug)
                      puts("Schwurbel!");

                    schwurbelanswer = geschwurbel();
                    if(myDebug)
                      printf("%s\n", schwurbelanswer);

                    // clear before printing?
                    if (myClear)
                      DoGadgetMethod(win_gadgets[GID_TXT_EDITOR], main_window, NULL, GM_TEXTEDITOR_ClearText, NULL);
                    /* add linefeed! */
                    else
                      DoGadgetMethod(win_gadgets[GID_TXT_EDITOR], main_window, NULL, GM_TEXTEDITOR_InsertText, NULL,"\n\n", GV_TEXTEDITOR_InsertText_Bottom);

                    /* add generated sentence: */
                    DoGadgetMethod(win_gadgets[GID_TXT_EDITOR], main_window, NULL, GM_TEXTEDITOR_InsertText, NULL,
                                   schwurbelanswer, GV_TEXTEDITOR_InsertText_Bottom);

                    if(myDebug)
                      if(printLeadIn)
                        printf("\nleadin - Anzahl Elemente: %ld\n", ((int)sizeof(leadin) / NUM_STRING_LENGTH) / sizeof(char));
                    if(myDebug)
                      printf("openers - Anzahl Elemente: %ld\n", ((int)sizeof(openers) / NUM_STRING_LENGTH) / sizeof(char));
                    if(myDebug)
                      printf("dowords - Anzahl Elemente: %ld\n", ((int)sizeof(dowords) / NUM_STRING_LENGTH) / sizeof(char));
                    if(myDebug)
                      printf("descriptions - Anzahl Elemente: %ld\n", ((int)sizeof(descriptions) / NUM_STRING_LENGTH) / sizeof(char));
                    if(myDebug)
                      printf("closers - Anzahl Elemente: %ld\n\n", ((int)sizeof(closers) / NUM_STRING_LENGTH) / sizeof(char));
                    break;

                    /* -- AUTO-SCHWURBEL --- */
                    case GAD_BTN_AUTOSCHWURBEL:                    
                      doAutoSchwurbel(main_window, win_gadgets[GID_TXT_EDITOR], window_object);
                      done = FALSE;	// Workaround for ReAction stack corruption bug                      
                      break;
                }
                break;

                case WMHI_ICONIFY:
                if ( RA_Iconify( window_object ) )
                  main_window = NULL;
                break;

                case WMHI_UNICONIFY:
                main_window = RA_OpenWindow( window_object );
                if ( menu_strip)
                  SetMenuStrip( main_window, menu_strip );
                break;

            }
          }
      }
    }
  }
}

void window_3( void )
{
  struct NewMenu menuData[] =
    {
      { NM_TITLE, "Projekt",0,0,0, NULL },
      { NM_ITEM, "Schwurbel!","S",0,0, (APTR) MN_SCHWURBEL  },
      { NM_ITEM, "Auto-Schwurbel","O",0,0, (APTR) MN_AUTOSCHWURBEL  },
      { NM_ITEM, NM_BARLABEL,0,0,0,NULL },
      { NM_ITEM, "Text löschen","N",0,0, (APTR) MN_TEXTCLEAR  },
      { NM_ITEM, NM_BARLABEL,0,0,0,NULL },
      { NM_ITEM, "Über...","?",0,0, (APTR) MN_ABOUT },
      { NM_ITEM, NM_BARLABEL,0,0,0,NULL },
      { NM_ITEM, "Beenden","E",0,0, (APTR) MN_QUIT  },

      { NM_TITLE, "Voreinstellungen",0,0,0, NULL },
      { NM_ITEM, "Verzögerung",0,0,0, NULL  },
      { NM_SUB, " 1 Sek.","1", CHECKIT | CHECKED, 31 -1, (APTR)MN_V1 },  // Mutex
      { NM_SUB, " 3 Sek.","3", CHECKIT, 31 -2, (APTR)MN_V3 },            // Mutex
      { NM_SUB, " 5 Sek.","5", CHECKIT, 31 - 4, (APTR)MN_V5 },           // Mutex
      { NM_SUB, " 8 Sek.","8", CHECKIT, 31 - 8, (APTR)MN_V8 },           // Mutex
      { NM_SUB, "10 Sek.", 0, CHECKIT, 31 - 16, (APTR)MN_V10 },          // Mutex
      { NM_ITEM, NM_BARLABEL,0,0,0,NULL },
      { NM_ITEM, "Wiederholungen",0,0,0, NULL  },
      { NM_SUB, "  5", "H", CHECKIT, 63 - 1, (APTR)MN_W5 },             // Mutex
      { NM_SUB, " 10", "I", CHECKIT | CHECKED, 63 - 2, (APTR)MN_W10 },  // Mutex
      { NM_SUB, " 20", "J", CHECKIT, 63 - 4, (APTR)MN_W20 },            // Mutex
      { NM_SUB, " 30", "K", CHECKIT, 63 - 8, (APTR)MN_W30 },            // Mutex
      { NM_SUB, " 50", "L", CHECKIT, 63 - 16, (APTR)MN_W50 },           // Mutex
      { NM_SUB, "100", "M", CHECKIT, 63 - 32, (APTR)MN_W100 },          // Mutex
      { NM_ITEM, NM_BARLABEL,0,0,0,NULL },
      { NM_ITEM, "Feld löschen?","D", CHECKIT|MENUTOGGLE|CHECKED, 0, (APTR)MN_DELETE }, // Checkmark
      { NM_ITEM, "Einleitung zeigen?",0, CHECKIT|MENUTOGGLE|CHECKED, 0, (APTR)MN_LEADIN }, // Checkmark
      { NM_ITEM, "Benachrichtigung zeigen?","B", CHECKIT|MENUTOGGLE|CHECKED, 0, (APTR)MN_REQUEST }, // Checkmark
      { NM_ITEM, NM_BARLABEL,0,0,0,NULL },
      { NM_ITEM, "Debug Ausgabe?","G", CHECKIT|MENUTOGGLE, 0, (APTR)MN_DEBUG }, // Checkmark
      { NM_END, NULL, 0, 0, 0, (APTR)0 }
    };

  struct TagItem scrollermap[] =
    {
      {SCROLLER_Top, GA_TEXTEDITOR_Prop_First},
      {TAG_END, 0}
    };

  struct TagItem texteditormap[] =
    {
      {GA_TEXTEDITOR_Prop_Entries, SCROLLER_Total},
      {GA_TEXTEDITOR_Prop_Visible, SCROLLER_Visible},
      {GA_TEXTEDITOR_Prop_First, SCROLLER_Top},
      {TAG_END, 0}
    };

  struct Menu	*menu_strip = NULL;
  struct Gadget	*main_gadgets[ 11 ];
  Object *window_object = NULL;
  menu_strip = CreateMenusA( menuData, TAG_END );
  LayoutMenus( menu_strip, gVisinfo,
               GTMN_NewLookMenus, TRUE,
               TAG_DONE );


  window_object = WindowObject,
                  WA_Title, "Babbel dummes Zeuch!",
                  WA_ScreenTitle, "Schwurblomat v1.0",
                  WA_Left, 5,
                  WA_Top, 20,
                  WA_Width, 400,
                  WA_Height, 160,
                  WA_MinWidth, 150,
                  WA_MinHeight, 20,
                  WA_MaxWidth, 400,
                  WA_MaxHeight, 20,
                  WINDOW_LockWidth, TRUE,
                  WINDOW_IconifyGadget, TRUE,
                  WINDOW_AppPort, gAppPort,
                  WINDOW_IconifyGadget, TRUE,
                  WA_CloseGadget, TRUE,
                  WA_DepthGadget, TRUE,
                  WA_SizeGadget, TRUE,
                  WA_DragBar, TRUE,
                  WA_Activate, TRUE,
                  WA_SizeBBottom, TRUE,
                  WINDOW_Position, WPOS_TOPLEFT,
                  WINDOW_IconTitle, "Schwurbel",
                  WINDOW_Icon,  GetDiskObject("Schwurblomat"),
                  WINDOW_HintInfo, &hintinfo, // GadgetHelp (ToolTips)
                  WINDOW_GadgetHelp, TRUE,    // adgetHelp (ToolTips)
                  //WA_SmartRefresh, TRUE,
                  WA_SimpleRefresh, TRUE,
                  WA_IDCMP, IDCMP_GADGETDOWN | IDCMP_GADGETUP | IDCMP_CLOSEWINDOW | IDCMP_MENUPICK,
                  WINDOW_ParentGroup, VLayoutObject,
                  LAYOUT_SpaceOuter, TRUE,
                  LAYOUT_DeferLayout, TRUE,
                  LAYOUT_AddChild, main_gadgets[layout_5] = LayoutObject,
                      GA_ID, layout_5_id,
                      LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
                      LAYOUT_AddChild, main_gadgets[layout_6] = LayoutObject,
                          GA_ID, layout_6_id,
                          LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
                          LAYOUT_BevelStyle, BVS_THIN,
                          LAYOUT_FixedHoriz, FALSE,
                          LAYOUT_FixedVert, FALSE,
                          LAYOUT_DeferLayout, TRUE,
                          LAYOUT_AddImage, main_gadgets[bitmap_7] = BitMapObject,
                              GA_ID, bitmap_7_id,
                              IA_Left, 0,
                              IA_Top, 0,
                              IA_Width, 400,
                              IA_Height, 50,
                              BITMAP_Screen, gScreen,
                              // get bitmap image
                              //BITMAP_SourceFile, "DevelDisk:C/under_construction/Schwurblomat/schwurblomat.ilbm",
                              BITMAP_SourceFile, "PROGDIR:schwurblomat.ilbm",
                              //BITMAP_SourceFile, myPic,
                              BitMapEnd,
                              LayoutEnd,
                              LAYOUT_AddChild, main_gadgets[layout_8] = LayoutObject,
                                  GA_ID, layout_8_id,
                                  LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
                                  LAYOUT_SpaceOuter, TRUE,
                                  LAYOUT_EvenSize, TRUE,
                                  LAYOUT_DeferLayout, TRUE,
                                  LAYOUT_AddChild, main_gadgets[layout_9] = LayoutObject,
                                      GA_ID, layout_9_id,
                                      LAYOUT_Orientation, LAYOUT_ORIENT_HORIZ,
                                      LAYOUT_SpaceOuter, TRUE,
                                      LAYOUT_DeferLayout, TRUE,
                                      /* --- TextEditor --- */
                                      LAYOUT_AddChild, main_gadgets[texteditor_10] = NewObject( TextEditor_GetClass(), NULL,
                                          GA_ID, texteditor_10_id,
                                          GA_ReadOnly, 0,
                                          GA_TEXTEDITOR_ExportWrap, 0,
                                          GA_TEXTEDITOR_ImportWrap, 99,
                                          GA_TEXTEDITOR_FixedFont, FALSE,
                                          GA_TEXTEDITOR_Flow, GV_TEXTEDITOR_Flow_Left,
                                          GA_TEXTEDITOR_IndentWidth, 0,
                                          GA_TEXTEDITOR_LineEndingExport, LINEENDING_LF,
                                          GA_TEXTEDITOR_ShowLineNumbers, FALSE,
                                          GA_TEXTEDITOR_SpacesPerTAB, 2,
                                          GA_TEXTEDITOR_TabKeyPolicy, GV_TEXTEDITOR_TabKey_IndentsAfter,
                                          GA_TEXTEDITOR_Contents, "...schwurbels you die Ohren up!",
                                          ICA_MAP, texteditormap,
                                          End,
                                          /* --- Scroller --- */
                                          LAYOUT_AddChild, main_gadgets[scroller_29] = ScrollerObject,
                                          GA_ID, scroller_29_id,
                                          GA_RelVerify, TRUE,
                                          GA_TabCycle, TRUE,
                                          SCROLLER_Top, 0,
                                          SCROLLER_Visible, 1,
                                          SCROLLER_Total, 1,
                                          SCROLLER_Arrows, TRUE,
                                          SCROLLER_Orientation, SORIENT_VERT,
                                          ICA_TARGET,  main_gadgets[texteditor_10],
                                          ICA_MAP, scrollermap,
                                          ScrollerEnd,
                                          LayoutEnd,
                                          LayoutEnd,
                                          LAYOUT_AddChild, main_gadgets[layout_11] = LayoutObject,
                                          GA_ID, layout_11_id,
                                          LAYOUT_Orientation, LAYOUT_ORIENT_HORIZ,
                                          LAYOUT_SpaceOuter, TRUE,
                                          LAYOUT_FixedVert, FALSE,
                                          LAYOUT_EvenSize, TRUE,
                                          LAYOUT_DeferLayout, TRUE,
                                          /* --- Button Auto-Geschwurbel --- */
                                          LAYOUT_AddChild, main_gadgets[button_12] = ButtonObject,
                                          GA_ID, button_12_id,
                                          GA_Text, "Aut_o-Geschwurbel",
                                          GA_RelVerify, TRUE,
                                          GA_TabCycle, TRUE,
                                          BUTTON_TextPen, 1,
                                          BUTTON_BackgroundPen, 0,
                                          BUTTON_FillTextPen, 1,
                                          BUTTON_FillPen, 3,
                                          ButtonEnd,
                                          /* --- Button Schwurbel los! --- */
                                          LAYOUT_AddChild, main_gadgets[button_13] = ButtonObject,
                                          GA_ID, button_13_id,
                                          GA_Text, "_Schwurbel los!",
                                          GA_RelVerify, TRUE,
                                          GA_TabCycle, TRUE,
                                          BUTTON_TextPen, 1,
                                          BUTTON_BackgroundPen, 0,
                                          BUTTON_FillTextPen, 1,
                                          BUTTON_FillPen, 3,
                                          ButtonEnd,
                                          LayoutEnd,
                                          LayoutEnd,
                                          LayoutEnd,
                                          WindowEnd;
                                          main_gadgets[10] = 0;

                                          /* connect TextEditor and Scroller: */
                                          SetAttrs(main_gadgets[texteditor_10],ICA_TARGET,  main_gadgets[scroller_29]);

                                          runWindow( window_object, 3, menu_strip, main_gadgets );

                                          if ( window_object ) DisposeObject( window_object );
                                          if ( menu_strip ) FreeMenus( menu_strip );
                                          }

/* the random function */
int RandomNumber(const int nMin, const int nMax, const int  nNumOfNumsToGenerate)
{
  int i, nRandonNumber = 0;
  for (i = 0; i <= nNumOfNumsToGenerate; i++)
  {
    srand(time(NULL));
    nRandonNumber = rand()%(nMax-nMin) + nMin;
  }
  //if(myDebug) printf("\n");

  return nRandonNumber;
}

/* build buzzword-bingo sentence */
STRPTR geschwurbel(void)
{
  int index0, index1, index2, index3, index4;
  //char antwort[512] = "Senf";

  /* set seed for random number generator */
  srand(time(NULL));

  /* auto-adjust string-array index counters, generate random numbers between 0 and index counter - 1  */
  //srand(time(NULL));
  /* check if user wants  leading sentence */
  if(printLeadIn)
  {
    index0 = RandomNumber(0, ((sizeof(leadin) / NUM_STRING_LENGTH) / sizeof(char)) , 1);
    if (myDebug)
      printf("index0 = %ld\n", index0);
  }
  //srand(time(NULL));
  index1 = RandomNumber(0, ((sizeof(openers) / NUM_STRING_LENGTH) / sizeof(char)) , 1);
  if (myDebug)
    printf("index1 = %ld\n", index1);
  //srand(time(NULL));
  index2 = RandomNumber(0, ((sizeof(dowords) / NUM_STRING_LENGTH) / sizeof(char)), 1);
  if (myDebug)
    printf("index2 = %ld\n", index2);
  //srand(time(NULL));
  index3 = RandomNumber(0, ((sizeof(descriptions) / NUM_STRING_LENGTH) / sizeof(char)), 1);
  if (myDebug)
    printf("index3 = %ld\n", index3);
  //srand(time(NULL));
  index4 = RandomNumber(0, ((sizeof(closers) / NUM_STRING_LENGTH) / sizeof(char)), 1);
  if (myDebug)
    printf("index4 = %ld\n", index4);

  /* prepare return value */
  if(printLeadIn)
    sprintf(antwort, "%s %s %s %s. %s", leadin[index0], openers[index1], dowords[index2], descriptions[index3], closers[index4]);
  else
    sprintf(antwort, "%s %s %s. %s", openers[index1], dowords[index2], descriptions[index3], closers[index4]);


  return(antwort);
}

int doInfoRequest(struct Screen *screen, struct Window *window, const char *title, const char *body, const char *buttons, ULONG image )
{
  Object *req = 0;		// the requester itself
  int button;			// the button that was clicked by the user

  // fill in the requester structure
  req = NewObject(REQUESTER_GetClass(), NULL,
                  REQ_Type,       REQTYPE_INFO,
                  REQ_TitleText,  (ULONG)title,
                  REQ_BodyText,   (ULONG)body,
                  REQ_GadgetText, (ULONG) buttons ,
                  REQ_Image,      image,
                  TAG_DONE);

  if (req)
  {
    struct orRequest reqmsg;

    reqmsg.MethodID  = RM_OPENREQ;
    reqmsg.or_Attrs  = NULL;
    reqmsg.or_Window = NULL;
    reqmsg.or_Screen = screen;

    button = DoMethodA(req, (Msg) &reqmsg);
    DisposeObject(req);

    return button;

  }
  else if(myDebug)
    printf("[request] Failed to allocate requester\n");

  return 0;
}



int main( int argc, char **argv )
{
  if ( setup() )
  {
    window_3();
  }
  cleanup();
}
