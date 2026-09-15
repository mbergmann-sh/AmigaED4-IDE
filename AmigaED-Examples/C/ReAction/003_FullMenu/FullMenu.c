/*
   Project:    ReBuild examples
   File:       FullMenu.c
   Version:    1.0
   
   Purpose:    demonstrates one way to create checkable/toggleable menu
               entries and how to check their status
   
   Author:     Michael Bergmann
   Date:       16-12-2023
	 
	 CAVEAT: This Project compiles under VBCC and SAS/C ONLY!
   
   VBCC compile_me:		vc +aos68k -v -lamiga -lauto FullMenu.c -o FullMenu		
	 SAS/C compile_me: 	sc icon FullMenu.c link to FullMenu
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
#include <proto/label.h>

#include <libraries/gadtools.h>
#include <reaction/reaction.h>
#include <intuition/gadgetclass.h>
#include <reaction/reaction_macros.h>
#include <classes/window.h>
#include <exec/memory.h>

/* Function Prototypes */
int main(void);
int setup(void);
void cleanup(void);
void window_menudemo(void);
void runWindow( Object *window_object, int window_id, 
								struct Menu *menu_strip, struct Gadget *win_gadgets[], 	
								int gadget_ids[] );

struct Screen	*gScreen = NULL;
struct DrawInfo	*gDrawInfo = NULL;
APTR gVisinfo = NULL;
struct MsgPort	*gAppPort = NULL;

struct Library *WindowBase = NULL,
               *LabelBase = NULL,
               *GadToolsBase = NULL,
               *LayoutBase = NULL,
               *IconBase = NULL;
struct IntuitionBase *IntuitionBase = NULL;

/* --- Menu identifiers --- */
enum menus {              
             MN_NLQ, MN_DRAFT,           // Mutex'ed (point)
             MN_ONE, MN_TWO, MN_THREE,   // selectable/toggleable (checkmark)
             MN_QUIT,                    // simple            
           };

int setup( void )
{
  if( !(IntuitionBase = (struct IntuitionBase*) OpenLibrary("intuition.library",0L)) ) return 0;
  if( !(GadToolsBase = (struct Library*) OpenLibrary("gadtools.library",0L) ) ) return 0;
  if( !(WindowBase = (struct Library*) OpenLibrary("window.class",0L) ) ) return 0;
  if( !(IconBase = (struct Library*) OpenLibrary("icon.library",0L) ) ) return 0;
  if( !(LayoutBase = (struct Library*) OpenLibrary("gadgets/layout.gadget",0L) ) ) return 0;
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
  if (LabelBase) CloseLibrary( (struct Library *)LabelBase );
  if (LayoutBase) CloseLibrary( (struct Library *)LayoutBase );
  if (WindowBase) CloseLibrary( (struct Library *)WindowBase );
}

void runWindow( Object *window_object, int window_id, struct Menu *menu_strip, struct Gadget *win_gadgets[], int gadget_ids[] )
{
  struct Window	*main_window = NULL;
  // we need a MenuItem struct to keep
  // track on status informations:
  struct MenuItem *menuitem = NULL;

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
                done = TRUE;
                break;

              case WMHI_MENUPICK:
                puts("menu pick");
                /* get menu entries */
                menuitem = ItemAddress(menu_strip, result & WMHI_MENUMASK);
                switch ((long)GTMENUITEM_USERDATA(menuitem))
                {
                    case MN_QUIT:
												puts("Quit selected");
                        done = TRUE;
                        break;                        
                    /* mutex'ed entries */
                    case MN_NLQ:
                        puts("NLQ selected");
                        puts("Extra bonus for wasting ink!");
                        break;
                    case MN_DRAFT:
                        puts("Draft selected");          
                        break; 
                    /* checkable entries */ 
                    case MN_ONE:
                        puts("ONE selected");
                         printf("menuitem = %ld\n", menuitem);
                         /* evaluate, if item has checkmark set... */
                         if(menuitem->Flags & CHECKED)
                         {                           
                            puts("Extra bonus for just taking ONE!");
                         }
                         else
                             puts("ONE deselected.");
                        break; 
                    case MN_TWO:
                        puts("TWO selected");
                        break; 
                    case MN_THREE:
                        puts("THREE selected");
                        break;    
                }                
                break;

              case WMHI_GADGETUP:
                puts("gadget press");
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

void window_menudemo( void )
{
  struct NewMenu menuData[] =
  {
    { NM_TITLE, "Demo",0,0,0,NULL},
    { NM_ITEM, "Print",0,0,0,NULL },
    { NM_SUB, "NLQ","L", CHECKIT | CHECKED, 3 - 1, (APTR)MN_NLQ },   // Mutex
    { NM_SUB, "Draft","D", CHECKIT, 3 - 2, (APTR)MN_DRAFT },         // Mutex
    { NM_ITEM, NM_BARLABEL,0,0,0,NULL },
    { NM_ITEM, "Count",0,0,0,NULL },
    { NM_SUB, "One","1", CHECKIT|MENUTOGGLE, 0, (APTR)MN_ONE },     // Checkmark
    { NM_SUB, "Two","2", CHECKIT|MENUTOGGLE,0, (APTR)MN_TWO },      // Checkmark
    { NM_SUB, "Three","3", CHECKIT|MENUTOGGLE,0, (APTR)MN_THREE  }, // Checkmark
    { NM_ITEM, NM_BARLABEL,0,0,0,NULL },
    { NM_ITEM, "Quit","Q",0,0, (APTR)MN_QUIT},
    { NM_END, NULL, 0, 0, 0, (APTR)0 }
  };

  struct Menu	*menu_strip = NULL;
  struct Gadget	*main_gadgets[ 4 ];
  int gadget_ids[ 4 ];
  Object *window_object = NULL;
  menu_strip = CreateMenusA( menuData, TAG_END );
  LayoutMenus( menu_strip, gVisinfo,
    GTMN_NewLookMenus, TRUE,
    TAG_DONE );


  window_object = WindowObject,
    WA_Title, "Check my menu!",
    WA_ScreenTitle, "Full Menu Demo v1.0",
    WA_Left, 5,
    WA_Top, 20,
    WA_Width, 150,
    WA_Height, 20,
    WA_MinWidth, 150,
    WA_MinHeight, 80,
    WA_MaxWidth, 8192,
    WA_MaxHeight, 20,
    WINDOW_LockHeight, TRUE,
    WINDOW_AppPort, gAppPort,
    WA_CloseGadget, TRUE,
    WA_DepthGadget, TRUE,
    WA_SizeGadget, TRUE,
    WA_DragBar, TRUE,
    WA_Activate, TRUE,
    WA_SizeBBottom, TRUE,
    WINDOW_Position, WPOS_TOPLEFT,
    WINDOW_IconTitle, "mDemo",
    WA_NoCareRefresh, TRUE,
    WA_IDCMP, IDCMP_GADGETDOWN | IDCMP_GADGETUP | IDCMP_CLOSEWINDOW | IDCMP_MENUPICK,
    WINDOW_ParentGroup, VLayoutObject,
    LAYOUT_SpaceOuter, TRUE,
    LAYOUT_DeferLayout, TRUE,
      LAYOUT_AddChild, main_gadgets[0] = LayoutObject,
        GA_ID, gadget_ids[0] = 5,
        LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
        LAYOUT_LeftSpacing, 10,
        LAYOUT_RightSpacing, 10,
        LAYOUT_TopSpacing, 10,
        LAYOUT_BottomSpacing, 10,
        LAYOUT_AddImage, main_gadgets[1] = LabelObject,
          GA_ID, gadget_ids[1] = 6,
          LABEL_DrawInfo, gDrawInfo,
          LABEL_Text, "I'm just a humble window, waiting",
        LabelEnd,
        LAYOUT_AddImage, main_gadgets[2] = LabelObject,
          GA_ID, gadget_ids[2] = 7,
          LABEL_DrawInfo, gDrawInfo,
          LABEL_Text, "for YOU to check my menu!",
        LabelEnd,
      LayoutEnd,
    LayoutEnd,
  WindowEnd;  
  main_gadgets[3] = 0;
  gadget_ids[3] = 0;

  runWindow( window_object, 3, menu_strip, main_gadgets, gadget_ids );

  if ( window_object ) DisposeObject( window_object );
  if ( menu_strip ) FreeMenus( menu_strip );
}

int main( void )
{
  if ( setup() )
  {
    window_menudemo();
  }
  cleanup();
}
