/*
 *	File:		 Window1.c
 *	Version:		1.0
 *	Revision:		0
 *	Purpose:	demonstrate compiling and linking of an	
 *						imported AmigaOS 3.s ReAction window that was
 *						generated using "ReBuild" by Darren Coles
 *	
 *						CAVEAT: The original output was modified by hand
 *						in order to make identification of buttons and
 *						menu entries more comfortable.
 *
 *	Author:		Michael Bergmann
 *	Email:		mbergmann-sh@gmx.de
 *	Web:		  https://mbergmann-sh.de
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

#include <proto/window.h>
#include <proto/layout.h>
#include <proto/button.h>
#include <proto/label.h>
#include <proto/requester.h>
#include <classes/requester.h>

#include <libraries/gadtools.h>
#include <reaction/reaction.h>
#include <intuition/gadgetclass.h>
#include <intuition/icclass.h>
#include <reaction/reaction_macros.h>
#include <classes/window.h>
#include <exec/memory.h>
#include <exec/types.h>

BOOL myDebug = TRUE;		// NOTE: used for conditional shell debuging messages

/* NOTE: create version information for AmigaOS 'version' command */
const char *ver = "\0$VER: Window1 v1.0.0 (12.09.2026)";


/* Function Prototypes */
int main( void );
int setup( void );
void cleanup( void );
int req_message(Object *reactionWindow);
int req_about(Object *reactionWindow);
int req_quit(Object *reactionWindow);
void window_main( void );
void runWindow( Object *window_object, int window_id, 
								struct Menu *menu_strip, struct Gadget *win_gadgets[] );

/* --- Menu IDs, used in GadTools menu struct's user field for message identification --- */
enum menus
{
	/* Menu "File"  */
	MN_MESSAGE, MN_ABOUT, MN_QUIT
};

/* not really neccessary, but for the fun of it: identify our button */	
#define	BTN_MESSAGE btn_message_id


struct Screen	*gScreen = NULL;
struct DrawInfo	*gDrawInfo = NULL;
APTR gVisinfo = NULL;
struct MsgPort	*gAppPort = NULL;
	
/* --- Requesters ---------------------------------------------------------------- */
struct EasyStruct warnreq =
{
  sizeof(struct EasyStruct),
  0,
  "Error",
  "This demo needs AmigaOS 3.2.x\n\nProgram aborded.",
  "Ok"
};

/* requester class MessageBox Requester, showing a short message */
int req_message(Object *reactionWindow)
{
  Object *reqobj;
  ULONG win;
  int res=0;

  GetAttr(WINDOW_Window, reactionWindow, &win);
  reqobj = NewObject(REQUESTER_GetClass(), NULL, REQ_Type, REQTYPE_INFO, REQ_Image, REQIMAGE_INFO,
       REQ_TitleText, "Here is your Message!",
       REQ_BodyText," I'm just a humble Message, shown in\n an AmigaOS 3.2 ReAction Requester.",
       REQ_GadgetText, "_Understood",
       TAG_DONE);
  if (reqobj)
  {
    res=DoMethod(reqobj, RM_OPENREQ, NULL, win, NULL);
    DisposeObject(reqobj);
  }
  return res;
}

/* requester class MessageBox Requester, showing an "About..." message */
int req_about(Object *reactionWindow)
{
  Object *reqobj;
  ULONG win;
  int res=0;

  GetAttr(WINDOW_Window, reactionWindow, &win);
  reqobj = NewObject(REQUESTER_GetClass(), NULL, REQ_Type, REQTYPE_INFO, REQ_Image, REQIMAGE_INFO,
       REQ_TitleText, "About this example...",
       REQ_BodyText," ReBuild generated AmigaOS 3.2 \n ReAction Window Example with \n a Menu, a button and requesters\n\n written by Micha B.",
       REQ_GadgetText, "_OK",
       TAG_DONE);
  if (reqobj)
  {
    res=DoMethod(reqobj, RM_OPENREQ, NULL, win, NULL);
    DisposeObject(reqobj);
  }
  return res;
}

int req_quit(Object *reactionWindow)
{
  Object *reqobj;
  ULONG win;
  int res=0;

  GetAttr(WINDOW_Window, reactionWindow, &win);
  reqobj = NewObject(REQUESTER_GetClass(), NULL, REQ_Type, REQTYPE_INFO, REQ_Image, REQIMAGE_QUESTION,
       REQ_TitleText, "Window Example - Quit",
       REQ_BodyText," Do you really want to quit? ",
       REQ_GadgetText, "_Quit|_Cancel",
       TAG_DONE);
  if (reqobj)
  {
    res=DoMethod(reqobj, RM_OPENREQ, NULL, win, NULL);
    DisposeObject(reqobj);
  }
  return res;
}

/* --- Library Bases --------------------------------- */
struct Library *WindowBase = NULL,
               *ButtonBase = NULL,
							 *LabelBase = NULL,
               *RequesterBase = NULL,
               *GadToolsBase = NULL,
               *LayoutBase = NULL,
               *IconBase = NULL;
struct IntuitionBase *IntuitionBase = NULL;

/* --- Window IDs ----------------------------------- */
enum win { window_main_id = 4 };

//Window_Main gadgets
enum window_main_idx { vert_main, vert_button, lbl_hint, btn_message };
enum window_main_id { vert_main_id = 6, vert_button_id = 7, lbl_hint_id = 11, btn_message_id = 8 };

/* open libraries and classes, create a message port, create screen informations */
int setup( void )
{
  if( !(IntuitionBase = (struct IntuitionBase*) OpenLibrary("intuition.library",0L)) ) return 0;
  if( !(GadToolsBase = (struct Library*) OpenLibrary("gadtools.library",0L) ) ) return 0;
  if( !(WindowBase = (struct Library*) OpenLibrary("window.class",0L) ) ) return 0;
  if( !(IconBase = (struct Library*) OpenLibrary("icon.library",0L) ) ) return 0;
  if( !(LayoutBase = (struct Library*) OpenLibrary("gadgets/layout.gadget",47) ) )
  {
    EasyRequest(NULL, &warnreq, NULL);
    return 0;
  }
	if( !(LabelBase = (struct Library*) OpenLibrary("images/label.image",0L) ) ) return 0;
  if( !(ButtonBase = (struct Library*) OpenLibrary("gadgets/button.gadget",0L) ) ) return 0;
  if( !(RequesterBase = (struct Library*) OpenLibrary("requester.class",0L) ) ) return 0;
  if( !(gScreen = LockPubScreen( 0 ) ) ) return 0;
  if( !(gVisinfo = GetVisualInfo( gScreen, TAG_DONE ) ) ) return 0;
  if( !(gDrawInfo = GetScreenDrawInfo ( gScreen ) ) ) return 0;
  if( !(gAppPort = CreateMsgPort() ) ) return 0;

  return -1;
}

	/* Close libraries, free message port and screen informations */
void cleanup( void )
{
  if ( gDrawInfo ) FreeScreenDrawInfo( gScreen, gDrawInfo);
  if ( gVisinfo ) FreeVisualInfo( gVisinfo );
  if ( gAppPort ) DeleteMsgPort( gAppPort );
  if ( gScreen ) UnlockPubScreen( 0, gScreen );

  if (GadToolsBase) CloseLibrary( (struct Library *)GadToolsBase );
  if (IconBase) CloseLibrary( (struct Library *)IconBase );
  if (IntuitionBase) CloseLibrary( (struct Library *)IntuitionBase );
  if (ButtonBase) CloseLibrary( (struct Library *)ButtonBase );
	if (LabelBase) CloseLibrary( (struct Library *)LabelBase );
  if (LayoutBase) CloseLibrary( (struct Library *)LayoutBase );
  if (WindowBase) CloseLibrary( (struct Library *)WindowBase );
  if (RequesterBase) CloseLibrary( (struct Library *)RequesterBase );
}

/* --- constructs a Window and adds an event loop --------------------------------- */
void runWindow( Object *window_object, int window_id, struct Menu *menu_strip, struct Gadget *win_gadgets[] )
{
  struct Window	*main_window = NULL;
	// we need a MenuItem struct to keep
  // track on status informations:
  struct MenuItem *menuitem = NULL;
  int req_result = 0;

  if ( window_object )
  {
    if ( main_window = (struct Window *) RA_OpenWindow( window_object ))
    {
      WORD Code;
      ULONG wait = 0, signal = 0, result = 0, done = FALSE;
      GetAttr( WINDOW_SigMask, window_object, &signal );
      if ( menu_strip)  SetMenuStrip( main_window, menu_strip );
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
                SetAttrs(window_object, WA_BusyPointer, TRUE, TAG_DONE); // switch to modal
                req_result = req_quit(window_object);
                if (req_result == 1)
                  done = TRUE;
                SetAttrs(window_object, WA_BusyPointer, FALSE, TAG_DONE); // switch to non-modal  
								break;

              /* --- Menus -------------------------------------------------------- */
              case WMHI_MENUPICK:      
                menuitem = ItemAddress(menu_strip, result & WMHI_MENUMASK);
                switch ((long)GTMENUITEM_USERDATA(menuitem))
                {
									case MN_MESSAGE:
										SetAttrs(window_object, WA_BusyPointer, TRUE, TAG_DONE); // switch to modal
										req_result = req_message(window_object);										
										SetAttrs(window_object, WA_BusyPointer, FALSE, TAG_DONE); // switch to non-modal
										break;
									case MN_ABOUT:
										SetAttrs(window_object, WA_BusyPointer, TRUE, TAG_DONE); // switch to modal
										req_result = req_about(window_object);										
										SetAttrs(window_object, WA_BusyPointer, FALSE, TAG_DONE); // switch to non-modal
										break;
									case MN_QUIT:
										SetAttrs(window_object, WA_BusyPointer, TRUE, TAG_DONE); // switch to modal
										req_result = req_quit(window_object);
										if (req_result == 1)
											done = TRUE;
										SetAttrs(window_object, WA_BusyPointer, FALSE, TAG_DONE); // switch to non-modal
										break;
									
								}		
                break;

              /* --- Gadgets ----------------------------------------------------- */
              case WMHI_GADGETUP:
                if(myDebug)
                  puts("gadget press");
                switch (result & WMHI_GADGETMASK)
                { 
									case BTN_MESSAGE:
										if(myDebug)
											puts("Button was pressed...");
										
										SetAttrs(window_object, WA_BusyPointer, TRUE, TAG_DONE); // switch to modal
										req_result = req_message(window_object);										
										SetAttrs(window_object, WA_BusyPointer, FALSE, TAG_DONE); // switch to non-modal
										break;
									
								}
							
                break;

              case WMHI_ICONIFY:
                if ( RA_Iconify( window_object ) )
                  main_window = NULL;
                break;

              case WMHI_UNICONIFY:
                main_window = RA_OpenWindow( window_object );
                if ( menu_strip)  SetMenuStrip( main_window, menu_strip );
              break;

            }
          }
      }
    }
  }
}

/* --- Main Window definition ------------------------------------- */
void window_main( void )
{
  struct NewMenu menuData[] =
  {
    { NM_TITLE, "File", 0, 0, 0, NULL },
    { NM_ITEM, "Show Message...", "S", 0, 30, (APTR)MN_MESSAGE },
    { NM_ITEM, NM_BARLABEL, 0, 0, 29, NULL },
    { NM_ITEM, "About...", 0, 0, 27, (APTR)MN_ABOUT },
    { NM_ITEM, NM_BARLABEL, 0, 0, 23, NULL },
    { NM_ITEM, "Quit", "Q", 0, 15, (APTR)MN_QUIT },
    { NM_END, NULL, 0, 0, 0, (APTR)0 }
  };

  struct Menu	*menu_strip = NULL;
  struct Gadget	*main_gadgets[ 5 ];
  Object *window_object = NULL;
  struct HintInfo hintInfo[] =
  {
    {vert_main_id,-1,"",0},
    {vert_button_id,-1,"",0},
    {lbl_hint_id,-1,"Please see my menu!",0},
    {btn_message_id,-1,"I will show you a message...",0},
    {-1,-1,NULL,0}
  };
  menu_strip = CreateMenusA( menuData, TAG_END );
  LayoutMenus( menu_strip, gVisinfo,
    GTMN_NewLookMenus, TRUE,
    TAG_DONE );


  window_object = NewObject( WINDOW_GetClass(), NULL, 
    WA_Title, "Window 01",
    WA_ScreenTitle, "ReBuild-generated ReAction Example 001",
    WA_Left, 5,
    WA_Top, 20,
    WA_Width, 250,
    WA_Height, 80,
    WA_MinWidth, 150,
    WA_MinHeight, 80,
    WA_MaxWidth, 8192,
    WA_MaxHeight, 8192,
    WINDOW_IconifyGadget, TRUE,
    WINDOW_HintInfo, hintInfo,
    WINDOW_GadgetHelp, TRUE,
    WINDOW_AppPort, gAppPort,
    WINDOW_IconifyGadget, TRUE,
    WA_CloseGadget, TRUE,
    WA_DepthGadget, TRUE,
    WA_SizeGadget, TRUE,
    WA_DragBar, TRUE,
    WA_Activate, TRUE,
    WA_SizeBBottom, TRUE,
    WINDOW_Position, WPOS_CENTERSCREEN,
    WINDOW_IconTitle, "Window1",
    WINDOW_Icon,  GetDiskObject("Window1"),
    WA_NoCareRefresh, TRUE,
    WA_IDCMP, IDCMP_GADGETDOWN | IDCMP_GADGETUP | IDCMP_CLOSEWINDOW | IDCMP_MENUPICK | IDCMP_MENUHELP | IDCMP_NEWSIZE | IDCMP_IDCMPUPDATE,
    WINDOW_ParentGroup, NewObject( LAYOUT_GetClass(), NULL,
    LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
    LAYOUT_SpaceOuter, TRUE,
    LAYOUT_DeferLayout, TRUE,
      LAYOUT_AddChild, main_gadgets[vert_main] = NewObject( LAYOUT_GetClass(), NULL, 
        GA_ID, vert_main_id,
        LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
        LAYOUT_SpaceOuter, TRUE,
        LAYOUT_DeferLayout, TRUE,
        LAYOUT_AddChild, main_gadgets[vert_button] = NewObject( LAYOUT_GetClass(), NULL, 
          GA_ID, vert_button_id,
          LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
          LAYOUT_SpaceOuter, TRUE,
          LAYOUT_LeftSpacing, 10,
          LAYOUT_RightSpacing, 10,
          LAYOUT_TopSpacing, 10,
          LAYOUT_BottomSpacing, 10,
          LAYOUT_DeferLayout, TRUE,
          LAYOUT_AddImage, main_gadgets[lbl_hint] = NewObject( LABEL_GetClass(), NULL, 
            GA_ID, lbl_hint_id,
            LABEL_DrawInfo, gDrawInfo,
            LABEL_Text, "Please also look at my File Menu!",
            IA_FGPen, 32,
            LABEL_Justification, LJ_CENTER,
            LABEL_VerticalSpacing , 2,
          TAG_END),
          LAYOUT_AddChild, main_gadgets[btn_message] = NewObject( BUTTON_GetClass(), NULL, 
            GA_ID, btn_message_id,
            GA_Text, "Klick me for Message!",
            GA_RelVerify, TRUE,
            GA_TabCycle, TRUE,
            BUTTON_TextPen, 1,
            BUTTON_BackgroundPen, 0,
            BUTTON_FillTextPen, 1,
            BUTTON_FillPen, 3,
          TAG_END),
        TAG_END),
      TAG_END),
    TAG_END),
  TAG_END);  
  main_gadgets[4] = 0;

  runWindow( window_object, window_main_id, menu_strip, main_gadgets );

  if ( window_object ) DisposeObject( window_object );
  if ( menu_strip ) FreeMenus( menu_strip );
}

/* --- Program's main entry point --------------------------------- */
int main( void )
{
  if ( setup() )
  {
    window_main();
  }
  cleanup();
	
	return(RETURN_OK);
}
