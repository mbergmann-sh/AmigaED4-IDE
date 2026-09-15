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
#include <proto/scroller.h>
#include <proto/string.h>
#include <proto/label.h>
#include <proto/bitmap.h>
#include <proto/texteditor.h>

#include <libraries/gadtools.h>
#include <reaction/reaction.h>
#include <intuition/gadgetclass.h>
#include <reaction/reaction_macros.h>
#include <classes/window.h>
#include <exec/memory.h>

#define TextEditor_GetClass() TEXTEDITOR_GetClass()

void window_3( void );

struct Screen	*gScreen = NULL;
struct DrawInfo	*gDrawInfo = NULL;
APTR gVisinfo = NULL;
struct MsgPort	*gAppPort = NULL;

struct Library *WindowBase = NULL,
               *ButtonBase = NULL,
               *ScrollerBase = NULL,
               *StringBase = NULL,
               *TextFieldBase = NULL,
               *LabelBase = NULL,
               *BitMapBase = NULL,
               *GadToolsBase = NULL,
               *LayoutBase = NULL,
               *IconBase = NULL;
struct IntuitionBase *IntuitionBase = NULL;

//Window_3
enum window_3_idx { layout_5, layout_6, bitmap_7, layout_8, layout_9, texteditor_10, 
  scroller_29, layout_11, string_28, button_13 };
enum window_3_id { layout_5_id = 5, layout_6_id = 19, bitmap_7_id = 21, layout_8_id = 6, 
  layout_9_id = 7, texteditor_10_id = 12, scroller_29_id = 29, 
  layout_11_id = 8, string_28_id = 28, button_13_id = 10 };

int setup( void )
{
  if( !(IntuitionBase = (struct IntuitionBase*) OpenLibrary("intuition.library",0L)) ) return 0;
  if( !(GadToolsBase = (struct Library*) OpenLibrary("gadtools.library",0L) ) ) return 0;
  if( !(WindowBase = (struct Library*) OpenLibrary("window.class",0L) ) ) return 0;
  if( !(IconBase = (struct Library*) OpenLibrary("icon.library",0L) ) ) return 0;
  if( !(LayoutBase = (struct Library*) OpenLibrary("gadgets/layout.gadget",0L) ) ) return 0;
  if( !(ButtonBase = (struct Library*) OpenLibrary("gadgets/button.gadget",0L) ) ) return 0;
  if( !(ScrollerBase = (struct Library*) OpenLibrary("gadgets/scroller.gadget",0L) ) ) return 0;
  if( !(StringBase = (struct Library*) OpenLibrary("gadgets/string.gadget",0L) ) ) return 0;
  if( !(LabelBase = (struct Library*) OpenLibrary("images/label.image",0L) ) ) return 0;
  if( !(BitMapBase = (struct Library*) OpenLibrary("images/bitmap.image",0L) ) ) return 0;
  if( !(TextFieldBase = (struct Library*) OpenLibrary("gadgets/texteditor.gadget",0L) ) ) return 0;
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
  if (ScrollerBase) CloseLibrary( (struct Library *)ScrollerBase );
  if (StringBase) CloseLibrary( (struct Library *)StringBase );
  if (LabelBase) CloseLibrary( (struct Library *)LabelBase );
  if (BitMapBase) CloseLibrary( (struct Library *)BitMapBase );
  if (TextFieldBase) CloseLibrary( (struct Library *)TextFieldBase );
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
                done = TRUE;
                break;

              case WMHI_MENUPICK:
                puts("menu pick");
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

void window_3( void )
{
  struct NewMenu menuData[] =
  {
    { NM_TITLE, "Projekt",0,0,0,NULL },
    { NM_ITEM, "Talk to rElizA","T",0,0,NULL },
    { NM_ITEM, "Clear Window","W",0,0,NULL },
    { NM_ITEM, NM_BARLABEL,0,0,0,NULL },
    { NM_ITEM, "About...","?",0,0,NULL },
    { NM_ITEM, NM_BARLABEL,0,0,0,NULL },
    { NM_ITEM, "Quit","Q",0,0,NULL },
    { NM_END, NULL, 0, 0, 0, (APTR)0 }
  };

  struct Menu	*menu_strip = NULL;
  struct Gadget	*main_gadgets[ 11 ];
  Object *window_object = NULL;
  menu_strip = CreateMenusA( menuData, TAG_END );
  LayoutMenus( menu_strip, gVisinfo,
    GTMN_NewLookMenus, TRUE,
    TAG_DONE );


  window_object = WindowObject,
    WA_Title, "Need psychological Help?",
    WA_ScreenTitle, "rElizA v1.0",
    WA_Left, 5,
    WA_Top, 20,
    WA_Width, 400,
    WA_Height, 240,
    WA_MinWidth, 150,
    WA_MinHeight, 100,
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
    WINDOW_Icon,  GetDiskObject("Schwurbel"),
    WA_NoCareRefresh, TRUE,
    WA_IDCMP, IDCMP_GADGETDOWN | IDCMP_GADGETUP | IDCMP_CLOSEWINDOW | IDCMP_MENUPICK | IDCMP_VANILLAKEY | IDCMP_RAWKEY,
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
            BITMAP_SourceFile, "DevelDisk:C/under_construction/rElizA/reliza.ilbm",
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
            LAYOUT_BevelStyle, BVS_GROUP,
            LAYOUT_SpaceOuter, TRUE,
            LAYOUT_LeftSpacing, 2,
            LAYOUT_RightSpacing, 2,
            LAYOUT_TopSpacing, 4,
            LAYOUT_BottomSpacing, 4,
            LAYOUT_DeferLayout, TRUE,
            LAYOUT_AddChild, main_gadgets[texteditor_10] = NewObject( TextEditor_GetClass(), NULL, 
              GA_ID, texteditor_10_id,
              GA_ReadOnly, 0,
              GA_TEXTEDITOR_ExportWrap, 1,
              GA_TEXTEDITOR_ImportWrap, 99,
              GA_TEXTEDITOR_FixedFont, FALSE,
              GA_TEXTEDITOR_Flow, GV_TEXTEDITOR_Flow_Justified,
              GA_TEXTEDITOR_IndentWidth, 0,
              GA_TEXTEDITOR_LineEndingExport, LINEENDING_LF,
              GA_TEXTEDITOR_ShowLineNumbers, FALSE,
              GA_TEXTEDITOR_SpacesPerTAB, 2,
              GA_TEXTEDITOR_TabKeyPolicy, GV_TEXTEDITOR_TabKey_IndentsAfter,
            End,
            LAYOUT_AddChild, main_gadgets[scroller_29] = ScrollerObject,
              GA_ID, scroller_29_id,
              GA_RelVerify, TRUE,
              GA_TabCycle, TRUE,
              SCROLLER_Top, 0,
              SCROLLER_Visible, 1,
              SCROLLER_Total, 1,
              SCROLLER_Arrows, TRUE,
              SCROLLER_Orientation, SORIENT_VERT,
            ScrollerEnd,
          LayoutEnd,
        LayoutEnd,
        LAYOUT_AddChild, main_gadgets[layout_11] = LayoutObject,
          GA_ID, layout_11_id,
          LAYOUT_Orientation, LAYOUT_ORIENT_HORIZ,
          LAYOUT_SpaceOuter, TRUE,
          LAYOUT_FixedVert, FALSE,
          LAYOUT_DeferLayout, TRUE,
          LAYOUT_AddChild, main_gadgets[string_28] = StringObject,
            GA_ID, string_28_id,
            GA_RelVerify, TRUE,
            GA_TabCycle, TRUE,
            STRINGA_MaxChars, 80,
            STRINGA_MinVisible, 44,
          StringEnd,
          LAYOUT_AddChild, main_gadgets[button_13] = ButtonObject,
            GA_ID, button_13_id,
            GA_Text, "_Talk",
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

  runWindow( window_object, 3, menu_strip, main_gadgets );

  if ( window_object ) DisposeObject( window_object );
  if ( menu_strip ) FreeMenus( menu_strip );
}

int main( int argc, char **argv )
{
  if ( setup() )
  {
    window_3();
  }
  cleanup();
}
