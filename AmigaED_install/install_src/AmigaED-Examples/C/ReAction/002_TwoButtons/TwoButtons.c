/*
 *	File:		 TwoButtons.c
 *	Version:		1.0
 *	Revision:		0
 *	Purpose: 	demonstrate how to identify which Button was
 *						pressed, demonstrate how to disable/enable a Gadget.
 *
 *	Author:		Michael Bergmann
 *	Email:		mbergmann-sh@gmx.de
 *	Web:			https://mbergmann-sh.de
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

#include <libraries/gadtools.h>
#include <reaction/reaction.h>
#include <intuition/gadgetclass.h>
#include <intuition/icclass.h>
#include <reaction/reaction_macros.h>
#include <classes/window.h>
#include <exec/memory.h>

#define myDebug TRUE

#define PROGRAMNAME     "TwoButtons"
#define VERSION         "1"
#define SUBVERSION      "0"
#define REVISION        "0"


/* construct version tag */
#if defined(__SASC)
  const UBYTE VersionTag[] = "$VER: " PROGRAMNAME " v" VERSION "." SUBVERSION "." REVISION " " __AMIGADATE__ "\n\0";
#else
  const UBYTE VersionTag[] = "$VER: TwoButtons v1.0 (26.09.2026)";
#endif


/* Function Prototypes */
int main( void );
int setup( void );
void cleanup( void );
void window_main( void );
void runWindow( Object *window_object, int window_id, 
								struct Menu *menu_strip, struct Gadget *win_gadgets[] );
									

struct Screen	*gScreen = NULL;
struct DrawInfo	*gDrawInfo = NULL;
APTR gVisinfo = NULL;
struct MsgPort	*gAppPort = NULL;

struct Library *WindowBase = NULL,
               *ButtonBase = NULL,
               *LabelBase = NULL,
               *GadToolsBase = NULL,
               *LayoutBase = NULL,
               *IconBase = NULL;
struct IntuitionBase *IntuitionBase = NULL;

//window ids
enum win { window_main_id = 4 };

//Window_Main gadgets
enum window_main_idx { vert_top, horiz_label, label_status, horiz_buttons, button_one, 
  button_two };
enum window_main_id { vert_top_id = 6, horiz_label_id = 7, label_status_id = 9, horiz_buttons_id = 8, 
  button_one_id = 10, button_two_id = 11 };

int setup( void )
{
  if( !(IntuitionBase = (struct IntuitionBase*) OpenLibrary("intuition.library",0L)) ) return 0;
  if( !(GadToolsBase = (struct Library*) OpenLibrary("gadtools.library",0L) ) ) return 0;
  if( !(WindowBase = (struct Library*) OpenLibrary("window.class",0L) ) ) return 0;
  if( !(IconBase = (struct Library*) OpenLibrary("icon.library",0L) ) ) return 0;
  if( !(LayoutBase = (struct Library*) OpenLibrary("gadgets/layout.gadget",47L) ) ) return 0;
  if( !(ButtonBase = (struct Library*) OpenLibrary("gadgets/button.gadget",0L) ) ) return 0;
  if( !(LabelBase = (struct Library*) OpenLibrary("images/label.image",0L) ) ) return 0;
  if( !(gScreen = LockPubScreen( 0 ) ) ) return 0;
  if( !(gVisinfo = GetVisualInfo( gScreen, TAG_DONE ) ) ) return 0;
  if( !(gDrawInfo = GetScreenDrawInfo ( gScreen ) ) ) return 0;
  if( !(gAppPort = CreateMsgPort() ) ) return 0;

  return -1;
}

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
}

void runWindow( Object *window_object, int window_id, struct Menu *menu_strip, struct Gadget *win_gadgets[] )
{
  struct Window	*main_window = NULL;

  if ( window_object )
  {
    if ( main_window = (struct Window *) RA_OpenWindow( window_object ))
    {
      ULONG button_two_disabled = TRUE;  // We need this for mutex'ing our button gadgets
      WORD Code;
      ULONG wait = 0, signal = 0, result = 0, done = FALSE;
      GetAttr( WINDOW_SigMask, window_object, &signal );
      if ( menu_strip)  SetMenuStrip( main_window, menu_strip );
      while ( !done)
      {
        wait = Wait( signal | SIGBREAKF_CTRL_C );

        if ( wait & SIGBREAKF_CTRL_C )
          done = TRUE;  // Close the application
        else
          while (( result = RA_HandleInput( window_object, &Code )) != WMHI_LASTMSG)
          {
            switch ( result & WMHI_CLASSMASK )
            {
              case WMHI_CLOSEWINDOW:
                done = TRUE;  // Close the application
                break;

              case WMHI_GADGETUP:
                if(myDebug)
                       puts("WMHI_GADGETUP occured!");
                       
                /* - Get status of button_two - */ 
                GetAttr(GA_Disabled, (APTR)win_gadgets[button_two], (ULONG *)&button_two_disabled);
                if(myDebug)
                  printf("Button Two status: %lu\n", button_two_disabled); 
                     
                switch (result & WMHI_GADGETMASK)
                {
                   case button_one_id:
                     if(myDebug)
                       puts("One pressed");
                       if(button_two_disabled)
                       { 
                         // enable button_two
                         SetGadgetAttrs((APTR)win_gadgets[button_two], main_window, NULL, GA_Disabled,  FALSE, TAG_END); 
                         // disable button_one
                         SetGadgetAttrs((APTR)win_gadgets[button_one], main_window, NULL, GA_Disabled,  TRUE, TAG_END);
                         // set status text in label_status
                         SetGadgetAttrs((APTR)win_gadgets[label_status], main_window, NULL, LABEL_Text, "One - disabled, Two - enabled", TAG_END);
                         // ReThink the window
                         //RethinkLayout( (struct Gadget *)WINDOW_ParentGroup, main_window, NULL, TRUE);
                         //DoMethod(window_object, WM_RETHINK);
                         RethinkLayout( win_gadgets[horiz_label], main_window, NULL, TRUE);
                          
                       }
                     break;
                     
                   case button_two_id:
                     if(myDebug)
                       puts("Two pressed");
                       if(!button_two_disabled)
                       { 
                         // disable button_two
                         SetGadgetAttrs((APTR)win_gadgets[button_two], main_window, NULL, GA_Disabled, TRUE, TAG_END); 
                         // enable button_one
                         SetGadgetAttrs((APTR)win_gadgets[button_one], main_window, NULL, GA_Disabled, FALSE, TAG_END);
                         // set status text in label_status
                         SetGadgetAttrs((APTR)win_gadgets[label_status], main_window, NULL, LABEL_Text, "One - enabled, Two - disabled", TAG_END);
                         // ReThink the window
                         RethinkLayout( win_gadgets[horiz_label], main_window, NULL, TRUE);
                          
                       }
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

void window_main( void )
{
  struct Gadget	*main_gadgets[ 7 ];
  Object *window_object = NULL;
  struct HintInfo hintInfo[] =
  {
    {vert_top_id,-1,"",0},
    {horiz_label_id,-1,"",0},
    {label_status_id,-1,"",0},
    {horiz_buttons_id,-1,"",0},
    {button_one_id,-1,"First Button",0},
    {button_two_id,-1,"Second Button",0},
    {-1,-1,NULL,0}
  };

  window_object = WindowObject,
    WA_Title, "TwoButtons",
    WA_ScreenTitle, "Rebuild Tutorial - Example: TwoButtons",
    WA_Left, 5,
    WA_Top, 20,
    WA_Width, 150,
    WA_Height, 50,
    WA_MinWidth, 150,
    WA_MinHeight, 10,
    WA_MaxWidth, 8192,
    WA_MaxHeight, 10,
    WINDOW_LockHeight, TRUE,
    WINDOW_HintInfo, hintInfo,
    WINDOW_GadgetHelp, TRUE,
    WINDOW_AppPort, gAppPort,
    WA_CloseGadget, TRUE,
    WA_DepthGadget, TRUE,
    WA_SizeGadget, TRUE,
    WA_DragBar, TRUE,
    WA_Activate, TRUE,
    WINDOW_IconTitle, "TwoButtons",
    WA_NoCareRefresh, TRUE,
    WA_IDCMP, IDCMP_GADGETDOWN | IDCMP_GADGETUP | IDCMP_CLOSEWINDOW | IDCMP_NEWSIZE,
    WINDOW_ParentGroup, VLayoutObject,
    LAYOUT_SpaceOuter, TRUE,
    LAYOUT_DeferLayout, TRUE,
      LAYOUT_AddChild, main_gadgets[vert_top] = LayoutObject,
        GA_ID, vert_top_id,
        LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
        LAYOUT_AddChild, main_gadgets[horiz_label] = LayoutObject,
          GA_ID, horiz_label_id,
          LAYOUT_Orientation, LAYOUT_ORIENT_HORIZ,
          LAYOUT_BevelStyle, BVS_GROUP,
          LAYOUT_SpaceOuter, TRUE,
          LAYOUT_Label, "Status",
          LAYOUT_DeferLayout, TRUE,
          LAYOUT_AddImage, main_gadgets[label_status] = LabelObject,
            GA_ID, label_status_id,
            LABEL_DrawInfo, gDrawInfo,
            LABEL_Text, "One - enabled, Two - Disabled",
            LABEL_Justification, LJ_CENTER,
          LabelEnd,
          CHILD_Label, LabelObject,
            LABEL_Text, "Buttons:",
          LabelEnd,
        LayoutEnd,
        LAYOUT_AddChild, main_gadgets[horiz_buttons] = LayoutObject,
          GA_ID, horiz_buttons_id,
          LAYOUT_Orientation, LAYOUT_ORIENT_HORIZ,
          LAYOUT_EvenSize, TRUE,
          LAYOUT_AddChild, main_gadgets[button_one] = ButtonObject,
            GA_ID, button_one_id,
            GA_Text, "One",
            GA_RelVerify, TRUE,
            GA_TabCycle, TRUE,
            BUTTON_TextPen, 1,
            BUTTON_BackgroundPen, 0,
            BUTTON_FillTextPen, 1,
            BUTTON_FillPen, 3,
          ButtonEnd,
          LAYOUT_AddChild, main_gadgets[button_two] = ButtonObject,
            GA_ID, button_two_id,
            GA_Text, "Two",
            GA_RelVerify, TRUE,
            GA_TabCycle, TRUE,
            GA_Disabled, TRUE,
            BUTTON_TextPen, 1,
            BUTTON_BackgroundPen, 0,
            BUTTON_FillTextPen, 1,
            BUTTON_FillPen, 3,
          ButtonEnd,
        LayoutEnd,
      LayoutEnd,
    LayoutEnd,
  WindowEnd;  
  main_gadgets[6] = 0;

  runWindow( window_object, window_main_id, 0, main_gadgets );

  if ( window_object ) DisposeObject( window_object );
}

int main( void )
{
  if ( setup() )
  {
    window_main();
  }
  cleanup();
}
