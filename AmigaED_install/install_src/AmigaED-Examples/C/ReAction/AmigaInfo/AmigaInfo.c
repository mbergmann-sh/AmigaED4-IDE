/*
 *	File:		 AmigaInfo.c
 *	Version:		1.0
 *	Revision:		0
 *	Purpose:	demonstrate ReAction: Use of string gadgets
 *						demonstrate how to retrieve system information
 *
 *	Author:		Michael Bergmann (Micha B.)
 *	Email:		mbergmann-sh@gmx.de
 *	Web:		https://mbergmann-sh.de
 */

#include <clib/macros.h>
#include <clib/compiler-specific.h>

#include <exec/types.h>
#include <exec/execbase.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <utility/tagitem.h>
#include <devices/timer.h>
#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>

#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/cghooks.h>
#include <intuition/gadgetclass.h>
#include <intuition/icclass.h>
#include <libraries/gadtools.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/utility.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/icon.h>
#include <proto/timer.h>
#include <proto/window.h>
#include <proto/layout.h>
#include <proto/string.h>
#include <proto/label.h>

#include <reaction/reaction.h>
#include <reaction/reaction_macros.h>

BOOL myDebug = TRUE;	/* printf() debugging TRUE or FALSE */

/* --- Version String --- */
char *ver = "\0$VER: AmigaInfo v1.0 (13.09.2026) \xa9 Michael Bergmann";

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#if defined(__STORM__)
	const char *compiler_string = "compiled with StormC3.\n\n";
#elif defined(__STORMGCC__)
	const char *compiler_string = "compiled with GNU gcc, StormC4 flavour.\n\n";
#elif defined(__MAXON__)
	const char *compiler_string = "compiled with Maxon/HiSoft C++.\n\n";
#elif defined(__GNUC__)
	const char *compiler_string = "compiled with GNU gcc v" STR(__GNUC__) "." STR(__GNUC_MINOR__) " Patchlevel " STR(__GNUC_PATCHLEVEL__) "\n";
#elif defined(__VBCC__)
	const char *compiler_string = "compiled with vbcc.";
#elif defined(__SASC)
	const char *compiler_string = "compiled with SAS/C.";
#elif defined(LATTICE)
	const char *compiler_string = "compiled with Lattice C.";
#elif defined(AZTEC_C)
	const char *compiler_string = "compiled with Manx Aztec C.";
#elif defined(_DCC)
	const char *compiler_string = "compiled with dice.";
#else
	const char *compiler_string = "Compiler was not identified.";
#endif

#ifndef GCIT_MMU
	#define GCIT_MMU 0x80000004
#endif

#ifndef GCIMMU_NONE
	#define GCIMMU_NONE  0
	#define GCIMMU_68851 1
	#define GCIMMU_68030 2
	#define GCIMMU_68040 3
	#define GCIMMU_68060 4
#endif

/* --- Function Prototypes --- */
int main( void );
static double get_amiga_time(void);
const char* benchmark_dhrystone(void);
const char* benchmark_mips(void);
const char* benchmark_mflops(void);
const char* GetCPUTypeString(void);
const char* GetFPUTypeString(void);
const char* GetMMUTypeString(void);
const char* GetChipRAMString(void);
const char* GetFastRAMString(void);
const char* GetTotalRAMString(void);
const char* GetOSVersionString(void);
const char* GetKickstartVersionString(void);
const char* GetWorkbenchVersionString(void);
const char* GetPALNTSCString(void);
const char* GetScreenModeString(void);
const char* GetChipsetString(void);
int setup( void );
void cleanup( void );
void window_main( void );
void runWindow( Object *window_object, int window_id, struct Menu *menu_strip, struct Gadget *win_gadgets[] );

/* GetCPUInfo(): kleine taggelisten-basierte Abfragefunktion (aktuell wird
   nur GCIT_MMU ausgewertet). War bisher nur als "extern" deklariert, aber
   nirgends implementiert/gelinkt (kein Aequivalent in NDK 3.2R4/amiga.lib
   fuer klassisches AmigaOS 3.x - GetCPUInfo() in dieser Form gibt es erst
   ab AmigaOS 4) - daher lokal in dieser Datei implementiert, siehe unten
   bei GetMMUTypeString(). */
void GetCPUInfo(struct TagItem *tags);

volatile long v_int1 = 1, v_int2 = 2;
volatile float v_float1 = 1234.56f, v_float2 = 78.9f;
	
struct Screen	*gScreen = NULL;
struct DrawInfo	*gDrawInfo = NULL;
APTR gVisinfo = NULL;
struct MsgPort	*gAppPort = NULL;

/* --- Library Bases --- */
struct Library *WindowBase = NULL,
               *StringBase = NULL,
               *LabelBase = NULL,
               *GadToolsBase = NULL,
               *LayoutBase = NULL,
               *IconBase = NULL;
struct IntuitionBase *IntuitionBase = NULL;
extern struct ExecBase *SysBase;
struct GfxBase *GfxBase;

/* window ids */
enum win { window_main_id = 4 };

/* Window_Main gadgets */
enum window_main_idx { vert_main, vert_system, strg_cpu, strg_fpu, strg_mmu, strg_os, 
  strg_kick, strg_wb, strg_chipset, strg_palntsc, strg_screenmode, 
  vert_memory, strg_chip, strg_fast, strg_total, vertspeed, strg_dhry, 
  strg_mips, strg_mflops, vert_compiler, strg_compiler };
enum window_main_id { vert_main_id = 6, vert_system_id = 7, strg_cpu_id = 8, strg_fpu_id = 10, 
  strg_mmu_id = 11, strg_os_id = 12, strg_kick_id = 13, strg_wb_id = 14, 
  strg_chipset_id = 25, strg_palntsc_id = 26, strg_screenmode_id = 29, 
  vert_memory_id = 22, strg_chip_id = 23, strg_fast_id = 27, strg_total_id = 28, 
  vertspeed_id = 50, strg_dhry_id = 51, strg_mips_id = 52, strg_mflops_id = 53, 
  vert_compiler_id = 54, strg_compiler_id = 55 };

int setup( void )
{
  if( !(IntuitionBase = (struct IntuitionBase*) OpenLibrary((STRPTR)"intuition.library",0L)) ) return 0;
  if( !(GadToolsBase = (struct Library*) OpenLibrary((STRPTR)"gadtools.library",0L) ) ) return 0;
  if( !(WindowBase = (struct Library*) OpenLibrary((STRPTR)"window.class",0L) ) ) return 0;
  if( !(IconBase = (struct Library*) OpenLibrary((STRPTR)"icon.library",0L) ) ) return 0;
  if( !(LayoutBase = (struct Library*) OpenLibrary((STRPTR)"gadgets/layout.gadget",0L) ) ) return 0;
  if( !(StringBase = (struct Library*) OpenLibrary((STRPTR)"gadgets/string.gadget",0L) ) ) return 0;
  if( !(LabelBase = (struct Library*) OpenLibrary((STRPTR)"images/label.image",0L) ) ) return 0;
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
  if (StringBase) CloseLibrary( (struct Library *)StringBase );
  if (LabelBase) CloseLibrary( (struct Library *)LabelBase );
  if (LayoutBase) CloseLibrary( (struct Library *)LayoutBase );
  if (WindowBase) CloseLibrary( (struct Library *)WindowBase );
}

void runWindow( Object *window_object, int window_id, struct Menu *menu_strip, struct Gadget *win_gadgets[] )
{
  struct Window	*main_window = NULL;

  if ( window_object )
  {
    if ( (main_window = (struct Window *) RA_OpenWindow( window_object )) )
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

void window_main( void )
{
  struct Gadget	*main_gadgets[56];
  Object *window_object = NULL;
  /* GetDiskObject() liefert einen struct DiskObject*, der explizit per
     FreeDiskObject() wieder freigegeben werden muss - das fehlte bisher
     komplett (WINDOW_Icon uebernimmt hier NICHT automatisch den Besitz).
     Vorab in einer eigenen Variable holen, damit wir sie nach dem
     Dispose des Fensters wieder freigeben koennen. */
  struct DiskObject *progIcon = NULL;

  /* Jedes Objekt bekommt jetzt eine EIGENE, VORAB fertiggestellte lokale
     Variable, statt wie zuvor als tief verschachtelter NewObject(...)-
     Aufruf direkt innerhalb der Tag-Liste eines anderen NewObject(...)
     zu stehen. Grund: NewObject ist hier laut Compiler-Fehlermeldung ein
     MAKRO (nicht die echte variadic Intuition-Funktion) - solche Makros
     bauen ihre Tag-Liste ueblicherweise in einem gemeinsamen/wiederver-
     wendeten Zwischenpuffer auf. Ruft man NewObject(...) verschachtelt
     als Tag-WERT eines aeusseren NewObject(...)-Aufrufs auf, wird dieser
     Puffer waehrend der Auswertung der inneren Aufrufe wiederholt
     ueberschrieben, bevor der AEUSSERE Aufruf seine eigene Tag-Liste
     damit aufbauen kann - die Gruppen-Objekte (die andere Objekte als
     Kinder referenzieren) bekommen dadurch teils falsche/vertauschte
     Tag-Werte, waehrend einfache Blatt-Objekte (String/Label ohne
     verschachtelte Kinder) unauffaellig bleiben. Das erklaert genau das
     beobachtete Bild: CPU/FPU/MMU-Werte korrekt, aber ab dort Felder in
     der falschen Gruppe. Durch die Aufteilung in einzelne, sequentielle
     Anweisungen ist zum Zeitpunkt, an dem eine Gruppe ihre eigene
     Tag-Liste aufbaut, jedes Kind bereits fertig und sein Zeiger in einer
     stabilen Variable gesichert - es findet keine verschachtelte
     Makro-Auswertung mehr waehrend des Aufbaus statt. */
  Object *cpuStr, *cpuLbl, *fpuStr, *fpuLbl, *mmuStr, *mmuLbl;
  Object *osStr, *osLbl, *kickStr, *kickLbl, *wbStr, *wbLbl;
  Object *chipsetStr, *chipsetLbl, *palntscStr, *palntscLbl, *screenmodeStr, *screenmodeLbl;
  Object *sysGroup;

  Object *chipStr, *chipLbl, *fastStr, *fastLbl, *totalStr, *totalLbl;
  Object *memGroup;

  Object *dhryStr, *dhryLbl, *mipsStr, *mipsLbl, *mflopsStr, *mflopsLbl;
  Object *speedGroup;

  Object *compilerStr;
  Object *compGroup;

  Object *mainGroup, *outerGroup;

  struct HintInfo hintInfo[] =
  {
    {vert_main_id,-1,(STRPTR)"",0},
    {vert_system_id,-1,(STRPTR)"",0},
    {strg_cpu_id,-1,(STRPTR)"",0},
    {strg_fpu_id,-1,(STRPTR)"",0},
    {strg_mmu_id,-1,(STRPTR)"",0},
    {strg_os_id,-1,(STRPTR)"",0},
    {strg_kick_id,-1,(STRPTR)"",0},
    {strg_wb_id,-1,(STRPTR)"",0},
    {strg_chipset_id,-1,(STRPTR)"",0},
    {strg_palntsc_id,-1,(STRPTR)"",0},
    {strg_screenmode_id,-1,(STRPTR)"",0},
    {vert_memory_id,-1,(STRPTR)"",0},
    {strg_chip_id,-1,(STRPTR)"",0},
    {strg_fast_id,-1,(STRPTR)"",0},
    {strg_total_id,-1,(STRPTR)"",0},
    {vertspeed_id,-1,(STRPTR)"",0},
    {strg_dhry_id,-1,(STRPTR)"",0},
    {strg_mips_id,-1,(STRPTR)"",0},
    {strg_mflops_id,-1,(STRPTR)"",0},
    {vert_compiler_id,-1,(STRPTR)"",0},
    {strg_compiler_id,-1,(STRPTR)"",0},
    {-1,-1,NULL,0}
  };

  progIcon = GetDiskObject((STRPTR)"AmigaInfo");

  /* --- System --- */
  cpuStr = main_gadgets[strg_cpu] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_cpu_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)GetCPUTypeString(),
    STRINGA_MaxChars, 80,
  TAG_END);
  cpuLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"CPU:",
  TAG_END);

  fpuStr = main_gadgets[strg_fpu] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_fpu_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)GetFPUTypeString(),
    STRINGA_MaxChars, 80,
  TAG_END);
  fpuLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"FPU:",
  TAG_END);

  mmuStr = main_gadgets[strg_mmu] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_mmu_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)GetMMUTypeString(),
    STRINGA_MaxChars, 80,
  TAG_END);
  mmuLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"MMU:",
  TAG_END);

  osStr = main_gadgets[strg_os] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_os_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)GetOSVersionString(),
    STRINGA_MaxChars, 80,
  TAG_END);
  osLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"OS:",
  TAG_END);

  kickStr = main_gadgets[strg_kick] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_kick_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)GetKickstartVersionString(),
    STRINGA_MaxChars, 80,
  TAG_END);
  kickLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"Kick:",
  TAG_END);

  wbStr = main_gadgets[strg_wb] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_wb_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)GetWorkbenchVersionString(),
    STRINGA_MaxChars, 80,
  TAG_END);
  wbLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"Workbench:",
  TAG_END);

  chipsetStr = main_gadgets[strg_chipset] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_chipset_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)GetChipsetString(),
    STRINGA_MaxChars, 80,
  TAG_END);
  chipsetLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"Chipset:",
  TAG_END);

  palntscStr = main_gadgets[strg_palntsc] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_palntsc_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)GetPALNTSCString(),
    STRINGA_MaxChars, 80,
  TAG_END);
  palntscLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"PAL/NTSC:",
  TAG_END);

  screenmodeStr = main_gadgets[strg_screenmode] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_screenmode_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)GetScreenModeString(),
    STRINGA_MaxChars, 80,
  TAG_END);
  screenmodeLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"Screenmode:",
  TAG_END);

  sysGroup = main_gadgets[vert_system] = NewObject( LAYOUT_GetClass(), NULL,
    GA_ID, vert_system_id,
    LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
    LAYOUT_BevelStyle, BVS_THIN,
    LAYOUT_SpaceOuter, TRUE,
    LAYOUT_LeftSpacing, 10,
    LAYOUT_RightSpacing, 10,
    LAYOUT_TopSpacing, 10,
    LAYOUT_BottomSpacing, 10,
    LAYOUT_Label, (ULONG)"System",
    LAYOUT_AddChild, (ULONG)cpuStr, CHILD_Label, (ULONG)cpuLbl,
    LAYOUT_AddChild, (ULONG)fpuStr, CHILD_Label, (ULONG)fpuLbl,
    LAYOUT_AddChild, (ULONG)mmuStr, CHILD_Label, (ULONG)mmuLbl,
    LAYOUT_AddChild, (ULONG)osStr, CHILD_Label, (ULONG)osLbl,
    LAYOUT_AddChild, (ULONG)kickStr, CHILD_Label, (ULONG)kickLbl,
    LAYOUT_AddChild, (ULONG)wbStr, CHILD_Label, (ULONG)wbLbl,
    LAYOUT_AddChild, (ULONG)chipsetStr, CHILD_Label, (ULONG)chipsetLbl,
    LAYOUT_AddChild, (ULONG)palntscStr, CHILD_Label, (ULONG)palntscLbl,
    LAYOUT_AddChild, (ULONG)screenmodeStr, CHILD_Label, (ULONG)screenmodeLbl,
  TAG_END);

  /* --- Memory --- */
  chipStr = main_gadgets[strg_chip] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_chip_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)GetChipRAMString(),
    STRINGA_MaxChars, 80,
  TAG_END);
  chipLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"Chip:",
  TAG_END);

  fastStr = main_gadgets[strg_fast] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_fast_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)GetFastRAMString(),
    STRINGA_MaxChars, 80,
  TAG_END);
  fastLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"Fast:",
  TAG_END);

  totalStr = main_gadgets[strg_total] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_total_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)GetTotalRAMString(),
    STRINGA_MaxChars, 80,
  TAG_END);
  totalLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"Total:",
  TAG_END);

  memGroup = main_gadgets[vert_memory] = NewObject( LAYOUT_GetClass(), NULL,
    GA_ID, vert_memory_id,
    LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
    LAYOUT_BevelStyle, BVS_THIN,
    LAYOUT_SpaceOuter, TRUE,
    LAYOUT_LeftSpacing, 10,
    LAYOUT_RightSpacing, 10,
    LAYOUT_TopSpacing, 10,
    LAYOUT_BottomSpacing, 10,
    LAYOUT_Label, (ULONG)"Memory",
    LAYOUT_AddChild, (ULONG)chipStr, CHILD_Label, (ULONG)chipLbl,
    LAYOUT_AddChild, (ULONG)fastStr, CHILD_Label, (ULONG)fastLbl,
    LAYOUT_AddChild, (ULONG)totalStr, CHILD_Label, (ULONG)totalLbl,
  TAG_END);

  /* --- Speedtests --- */
  dhryStr = main_gadgets[strg_dhry] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_dhry_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)benchmark_dhrystone(),
    STRINGA_MaxChars, 80,
  TAG_END);
  dhryLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"Dhrystone:",
  TAG_END);

  mipsStr = main_gadgets[strg_mips] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_mips_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)benchmark_mips(),
    STRINGA_MaxChars, 80,
  TAG_END);
  mipsLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"MIPS:",
  TAG_END);

  mflopsStr = main_gadgets[strg_mflops] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_mflops_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)benchmark_mflops(),
    STRINGA_MaxChars, 80,
  TAG_END);
  mflopsLbl = NewObject( LABEL_GetClass(), NULL,
    LABEL_Text, (ULONG)"MFLOPS:",
  TAG_END);

  speedGroup = main_gadgets[vertspeed] = NewObject( LAYOUT_GetClass(), NULL,
    GA_ID, vertspeed_id,
    LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
    LAYOUT_BevelStyle, BVS_THIN,
    LAYOUT_SpaceOuter, TRUE,
    LAYOUT_LeftSpacing, 10,
    LAYOUT_RightSpacing, 10,
    LAYOUT_TopSpacing, 10,
    LAYOUT_BottomSpacing, 10,
    LAYOUT_Label, (ULONG)"Speedtests",
    LAYOUT_AddChild, (ULONG)dhryStr, CHILD_Label, (ULONG)dhryLbl,
    LAYOUT_AddChild, (ULONG)mipsStr, CHILD_Label, (ULONG)mipsLbl,
    LAYOUT_AddChild, (ULONG)mflopsStr, CHILD_Label, (ULONG)mflopsLbl,
  TAG_END);

  /* --- Compiler --- */
  compilerStr = main_gadgets[strg_compiler] = NewObject( STRING_GetClass(), NULL,
    GA_ID, strg_compiler_id,
    GA_RelVerify, TRUE,
    GA_TabCycle, TRUE,
    GA_ReadOnly, TRUE,
    STRINGA_TextVal, (ULONG)compiler_string,
    STRINGA_MaxChars, 80,
  TAG_END);

  compGroup = main_gadgets[vert_compiler] = NewObject( LAYOUT_GetClass(), NULL,
    GA_ID, vert_compiler_id,
    LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
    LAYOUT_BevelStyle, BVS_THIN,
    LAYOUT_SpaceOuter, TRUE,
    LAYOUT_LeftSpacing, 10,
    LAYOUT_RightSpacing, 10,
    LAYOUT_TopSpacing, 10,
    LAYOUT_BottomSpacing, 10,
    LAYOUT_Label, (ULONG)"Compiler",
    LAYOUT_AddChild, (ULONG)compilerStr,
  TAG_END);

  /* --- vert_main: haelt die vier umrandeten Gruppen --- */
  mainGroup = main_gadgets[vert_main] = NewObject( LAYOUT_GetClass(), NULL,
    GA_ID, vert_main_id,
    LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
    LAYOUT_SpaceOuter, TRUE,
    LAYOUT_DeferLayout, TRUE,
    LAYOUT_AddChild, (ULONG)sysGroup,
    LAYOUT_AddChild, (ULONG)memGroup,
    LAYOUT_AddChild, (ULONG)speedGroup,
    LAYOUT_AddChild, (ULONG)compGroup,
  TAG_END);

  /* --- aeusserer Wrapper, direkt als WINDOW_ParentGroup verwendet
         (unveraendert aus dem Original uebernommen) --- */
  outerGroup = NewObject( LAYOUT_GetClass(), NULL,
    LAYOUT_Orientation, LAYOUT_ORIENT_VERT,
    LAYOUT_SpaceOuter, TRUE,
    LAYOUT_DeferLayout, TRUE,
    LAYOUT_AddChild, (ULONG)mainGroup,
  TAG_END);

  window_object = NewObject( WINDOW_GetClass(), NULL,
    WA_Title, (ULONG)"AmigaInfo",
    WA_ScreenTitle, (ULONG)"AmigaInfo v1.0",
    WA_Left, 5,
    WA_Top, 20,
    WA_Width, 500,
    WA_Height, 20,
    WA_MinWidth, 150,
    WA_MinHeight, 80,
    WA_MaxWidth, 8192,
    WA_MaxHeight, 8192,
    WINDOW_LockHeight, TRUE,
    WINDOW_IconifyGadget, TRUE,
    WINDOW_HintInfo, (ULONG)hintInfo,
    WINDOW_GadgetHelp, TRUE,
    WINDOW_AppPort, (ULONG)gAppPort,
    WA_CloseGadget, TRUE,
    WA_DepthGadget, TRUE,
    WA_SizeGadget, TRUE,
    WA_DragBar, TRUE,
    WA_Activate, TRUE,
    WINDOW_Position, WPOS_TOPLEFT,
    WINDOW_IconTitle, (ULONG)"AmigaInfo",
    WINDOW_Icon,  (ULONG)progIcon,
    WA_NoCareRefresh, TRUE,
    WA_IDCMP, IDCMP_GADGETDOWN | IDCMP_GADGETUP | IDCMP_CLOSEWINDOW | IDCMP_MENUPICK | IDCMP_NEWSIZE | IDCMP_IDCMPUPDATE,
    WINDOW_ParentGroup, (ULONG)outerGroup,
  TAG_END);

  runWindow( window_object, window_main_id, 0, main_gadgets );

  if ( window_object ) DisposeObject( window_object );
  if ( progIcon ) FreeDiskObject( progIcon );
}

static double get_amiga_time(void) 
{
    struct MsgPort *timer_port = NULL;
    struct TimeRequest *timer_req = NULL;
    struct timeval tv;

    timer_port = CreateMsgPort();
    if (!timer_port) return 0.0;

    timer_req = (struct TimeRequest *)CreateIORequest(timer_port, sizeof(struct TimeRequest));
    if (!timer_req) 
	{
        DeleteMsgPort(timer_port);
        return 0.0;
    }

    if (OpenDevice((STRPTR)TIMERNAME, UNIT_MICROHZ, (struct IORequest *)timer_req, 0) == 0) 
	{
        timer_req->tr_node.io_Command = TR_GETSYSTIME;
        DoIO((struct IORequest *)timer_req);
        tv.tv_secs  = timer_req->tr_time.tv_secs;
		tv.tv_micro = timer_req->tr_time.tv_micro;
        CloseDevice((struct IORequest *)timer_req);
    } 
	else 
	{
        tv.tv_secs = 0;
        tv.tv_micro = 0;
    }

    DeleteIORequest((struct IORequest *)timer_req);
    DeleteMsgPort(timer_port);

    return (double)tv.tv_secs + ((double)tv.tv_micro / 1000000.0);
}

const char* benchmark_dhrystone(void) 
{
    static char buffer[80];
    long iterations = 50000;
    double start, end, seconds, dhrystones_per_sec, dmips;
    long i;
    
    start = get_amiga_time();
    for (i = 0; i < iterations; i++) {
        v_int1 = v_int2 + i;
        v_int2 = v_int1 - i;
        if (v_int1 == v_int2) {
            v_int1++;
        }
    }
    end = get_amiga_time();
    
    seconds = end - start;
    if (seconds < 0.0001) seconds = 0.0001;
    
    dhrystones_per_sec = (double)(iterations * 10) / seconds;
    dmips = dhrystones_per_sec / 1757.0;
    
    snprintf(buffer, sizeof(buffer), "%.1f P/S (~%.2f DMIPS)", dhrystones_per_sec, dmips);
    return buffer;
}

const char* benchmark_mips(void) {
    static char buffer[80];
    long iterations = 50000;
    double start, end, seconds, total_instructions, mips;
    long i;
    
    start = get_amiga_time();
    for (i = 0; i < iterations; i++) {
        v_int1 += 2;
        v_int1 ^= v_int2;
        v_int2 *= 3;
        v_int1 -= v_int2;
    }
    end = get_amiga_time();
    
    seconds = end - start;
    if (seconds < 0.0001) seconds = 0.0001;
    
    total_instructions = (double)iterations * 10.0;
    mips = (total_instructions / seconds) / 1000000.0;
    
    snprintf(buffer, sizeof(buffer), "%.3f Million Instr/Sec", mips);
    return buffer;
}

const char* benchmark_mflops(void) {
    static char buffer[80];
    long iterations = 5000;
    double start, end, seconds, total_flops, mflops;
    long i;
    
    start = get_amiga_time();
    for (i = 0; i < iterations; i++) {
        v_float1 = (v_float1 + v_float2) * 0.5f;
        v_float2 = (v_float1 - v_float2) * 1.1f;
    }
    end = get_amiga_time();
    
    seconds = end - start;
    if (seconds < 0.0001) seconds = 0.0001;
    
    total_flops = (double)iterations * 4.0;
    mflops = (total_flops / seconds) / 1000000.0;
    
    snprintf(buffer, sizeof(buffer), "%.4f Million Flops/Sec", mflops);
    return buffer;
}

const char* GetCPUTypeString(void)
{
    UWORD flags = SysBase->AttnFlags;

    if (flags & AFF_68060) return "MC68060";
    if (flags & AFF_68040) return "MC68040";
    if (flags & AFF_68030) return "MC68030";
    if (flags & AFF_68020) return "MC68020";
    if (flags & AFF_68010) return "MC68010";
    
    return "MC68000";
}

const char* GetFPUTypeString(void)
{
    UWORD flags = SysBase->AttnFlags;

    if (flags & (AFF_68040 | AFF_68060))
    {
        if (flags & AFF_68881) return "Interne FPU";
    }
    if (flags & AFF_68882) return "MC68882";
    if (flags & AFF_68881) return "MC68881";

    return "Keine";
}

/* Lokale Implementierung von GetCPUInfo() fuer klassisches AmigaOS 3.x:
   eine echte "GetCPUInfo(struct TagItem*)"-Bibliotheksfunktion gibt es
   erst ab AmigaOS 4 (exec.library); unter NDK 3.2R4 existiert sie nicht,
   daher hier per Hand nachgebaut - taggelisten-kompatibel zum bisherigen
   Aufruf in GetMMUTypeString(), aber ohne Abhaengigkeit von
   utility.library (die Datei oeffnet UtilityBase bislang nicht), indem
   das kurze, kontrollierte Tag-Array manuell durchlaufen wird statt
   NextTagItem()/GetTagData() zu benutzen. */
void GetCPUInfo(struct TagItem *tags)
{
    while (tags && tags->ti_Tag != TAG_DONE)
    {
        switch (tags->ti_Tag)
        {
            case GCIT_MMU:
            {
                UWORD flags = SysBase->AttnFlags;
                ULONG mmuType = GCIMMU_NONE;

                /* 68040/68060 haben eine interne MMU, 68030 ebenfalls.
                   Eine externe 68851-MMU an einer 68000/68010/68020-CPU
                   laesst sich ueber SysBase->AttnFlags allein nicht
                   zuverlaessig erkennen (kein eigenes AFF_*-Bit dafuer in
                   den Exec-Headern) - in diesem (seltenen) Fall wird
                   "keine MMU" gemeldet, wie es auch andere klassische
                   System-Info-Tools an dieser Stelle handhaben. */
                if (flags & AFF_68060)      mmuType = GCIMMU_68060;
                else if (flags & AFF_68040) mmuType = GCIMMU_68040;
                else if (flags & AFF_68030) mmuType = GCIMMU_68030;

                *(ULONG *)tags->ti_Data = mmuType;
                break;
            }

            default:
                break;
        }
        tags++;
    }
}

const char* GetMMUTypeString(void)
{
    ULONG mmuType = GCIMMU_NONE;
    struct TagItem cpuTags[2];

    cpuTags[0].ti_Tag  = GCIT_MMU;
    cpuTags[0].ti_Data = (ULONG)&mmuType;
    cpuTags[1].ti_Tag  = TAG_DONE;
    cpuTags[1].ti_Data = 0;

    GetCPUInfo(cpuTags);

    switch (mmuType)
    {
        case GCIMMU_68851: return "MC68851 (Extern)";
        case GCIMMU_68030: return "MC68030 (Intern)";
        case GCIMMU_68040: return "MC68040 (Intern)";
        case GCIMMU_68060: return "MC68060 (Intern)";
        case GCIMMU_NONE:
        default:           return "Keine";
    }
}

/* Formatiert eine ULONG-Bytegroesse als "X.X MB" (ab 1 MB) oder "X KB"
   (darunter) in den uebergebenen Puffer - gemeinsame Hilfsfunktion fuer
   die drei RAM-Abfragen unten, um die Formatierung konsistent zu halten. */
static void FormatByteSize(ULONG size, char *buffer, size_t bufferSize)
{
    if (size >= 1024UL*1024UL)
        snprintf(buffer, bufferSize, "%.1f MB", (double)size / (1024.0*1024.0));
    else
        snprintf(buffer, bufferSize, "%lu KB", (unsigned long)(size / 1024UL));
}

/* AvailMem() mit dem MEMF_TOTAL-Flag liefert (anders als ohne dieses Flag)
   nicht den aktuell FREIEN, sondern den INSTALLIERTEN Speicher dieses Typs
   - das passt hier besser, da die uebrigen Felder in der Gruppe "System"
   (OS/Kick/Workbench/Chipset/...) ebenfalls statische Systeminformation
   und keine Laufzeit-Auslastung zeigen. */
const char* GetChipRAMString(void)
{
    static char buffer[80];
    FormatByteSize(AvailMem(MEMF_CHIP | MEMF_TOTAL), buffer, sizeof(buffer));
    return buffer;
}

const char* GetFastRAMString(void)
{
    static char buffer[80];
    FormatByteSize(AvailMem(MEMF_FAST | MEMF_TOTAL), buffer, sizeof(buffer));
    return buffer;
}

/* MEMF_TOTAL ohne CHIP/FAST-Einschraenkung liefert den Gesamtspeicher
   ueber ALLE Speichertypen (nicht nur Chip+Fast addiert) - das ist die
   korrektere Definition von "insgesamt verfuegbar", da sie z.B. auch
   Speicher von Beschleunigerkarten mit eigenen MEMF_*-Flags mit erfasst. */
const char* GetTotalRAMString(void)
{
    static char buffer[80];
    FormatByteSize(AvailMem(MEMF_TOTAL), buffer, sizeof(buffer));
    return buffer;
}

/* Liest eine AmigaOS-Environment-Variable direkt aus "ENVARC:<name>"
   (persistente Kopie auf Platte), NICHT ueber die zur Laufzeit gemountete
   "ENV:"-RAM-Disk (GetVar()) - so funktioniert es unabhaengig davon, ob
   ENV: zum Zeitpunkt des Aufrufs bereits vollstaendig aus ENVARC: befuellt
   wurde. Schneidet ein abschliessendes CR/LF ab. Liefert FALSE, wenn die
   Datei fehlt oder leer ist (z.B. auf Systemen ohne diese Variable, etwa
   vor AmigaOS 3.1). */
static BOOL ReadEnvArcVar(const char *name, char *buffer, size_t bufferSize)
{
    char path[128];
    BPTR fh;
    LONG len;

    snprintf(path, sizeof(path), "ENVARC:%s", name);

    fh = Open((STRPTR)path, MODE_OLDFILE);
    if (!fh) return FALSE;

    len = Read(fh, buffer, (LONG)(bufferSize - 1));
    Close(fh);

    if (len <= 0) return FALSE;

    buffer[len] = '\0';
    while (len > 0 && (buffer[len-1] == '\n' || buffer[len-1] == '\r'))
        buffer[--len] = '\0';

    return (len > 0);
}

/* Liefert die "offizielle" Kickstart-Version, wie sie im Amiga-Menu
   "Version, Copyright" angezeigt wird (z.B. "47.111"). WICHTIG: Das ist
   NICHT dasselbe wie SysBase->LibNode.lib_Version/.lib_Revision - Test auf
   echter Hardware ergab 47.10 (rohe exec.library-Wartungsversion) statt
   der offiziell gefuehrten Zahl 47.111. Vermutung (ungetestet, siehe Chat):
   analog zu ENVARC:Versions/Release (siehe GetOSVersionString()) koennte
   ENVARC:Versions/Kickstart die offizielle Zahl enthalten, da genau dieses
   Versions-Verzeichnis erkennbar zu diesem Zweck existiert. Fallback auf
   die rohe exec.library-Versionsnummer, falls die Datei fehlt/anders
   heisst - bitte Dateiname in ENVARC:Versions/ pruefen und melden, falls
   dieser Versuch nicht das Erwartete liefert. */
const char* GetKickstartVersionString(void)
{
    static char buffer[40];

    if (ReadEnvArcVar("Versions/Kickstart", buffer, sizeof(buffer)))
        return buffer;

    snprintf(buffer, sizeof(buffer), "%u.%u",
        (unsigned)SysBase->LibNode.lib_Version,
        (unsigned)SysBase->LibNode.lib_Revision);
    return buffer;
}

/* Bildet die exec.library-Versionsnummer (Kickstart-ROM) auf die uebliche
   "Marketing"-AmigaOS-Versionsbezeichnung ab (wie auf Verpackung/Handbuch).
   HINWEIS/VORSICHT: Diese Tabelle ist nach bestem Wissen aus oeffentlich
   dokumentierten Exec-Versionen zusammengestellt, aber gerade fuer 3.1.4
   und 3.2.x nicht mit letzter Sicherheit verifiziert. Da hier ein
   tatsaechlicher Mitentwickler von AmigaOS 3.2 pruefen kann: bitte bei
   Gelegenheit kontrollieren/korrigieren. Unbekannte Werte fallen auf die
   rohe Exec-Versionsnummer zurueck, statt etwas Falsches zu behaupten.
   Zusaetzlich wird, durch Komma getrennt, der Inhalt von
   ENVARC:Versions/Release angehaengt (sofern vorhanden) - der liefert die
   genaue Release-Nummer (z.B. "3.2.3"), waehrend die Tabelle oben nur grob
   nach Exec-Version raet. */
const char* GetOSVersionString(void)
{
    UWORD execVer = SysBase->LibNode.lib_Version;
    static char buffer[80];
    const char *osVer;
    char releaseBuf[40];
    size_t curLen;

    switch (execVer)
    {
        case 34: osVer = "AmigaOS 1.2/1.3";   break;
        case 36: osVer = "AmigaOS 2.0";       break;
        case 37: osVer = "AmigaOS 2.04/2.05"; break;
        case 39: osVer = "AmigaOS 3.0";       break;
        case 40: osVer = "AmigaOS 3.1";       break;
        case 45: osVer = "AmigaOS 3.9";       break;
        case 46: osVer = "AmigaOS 3.1.4";     break;
        case 47: osVer = "AmigaOS 3.2.x";     break;
        default: osVer = NULL;                break;
    }

    if (osVer)
    {
        strncpy(buffer, osVer, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "Unbekannt (Exec %u.%u)",
            (unsigned)execVer, (unsigned)SysBase->LibNode.lib_Revision);
    }

    if (ReadEnvArcVar("Versions/Release", releaseBuf, sizeof(releaseBuf)))
    {
        curLen = strlen(buffer);
        if (curLen + 2 < sizeof(buffer))
            snprintf(buffer + curLen, sizeof(buffer) - curLen, ", %s", releaseBuf);
    }

    return buffer;
}

/* Liefert die "offizielle" Workbench-Version wie im Amiga-Menu "Version,
   Copyright" (z.B. "47.5"). Gleiches Problem wie bei GetKickstartVersionString()
   oben: workbench.library selbst meldete auf echter Hardware 47.42 statt
   der offiziell gefuehrten 47.5 - vermutlich ebenfalls zwei unterschiedliche
   Zaehler (Bibliotheks-Wartungsversion vs. offizielle Release-Zahl).
   Gleicher Loesungsversuch: zuerst ENVARC:Versions/Workbench probieren,
   sonst Fallback auf workbench.library selbst - bitte auch hier den
   tatsaechlichen Dateinamen in ENVARC:Versions/ pruefen/melden. */
const char* GetWorkbenchVersionString(void)
{
    static char buffer[40];
    struct Library *wbBase;

    if (ReadEnvArcVar("Versions/Workbench", buffer, sizeof(buffer)))
        return buffer;

    wbBase = OpenLibrary((STRPTR)"workbench.library", 0L);
    if (wbBase)
    {
        snprintf(buffer, sizeof(buffer), "%u.%u",
            (unsigned)wbBase->lib_Version, (unsigned)wbBase->lib_Revision);
        CloseLibrary(wbBase);
    }
    else
    {
        strcpy(buffer, "Nicht ermittelbar");
    }
    return buffer;
}

/* PAL/NTSC ist eine Eigenschaft des gesamten Grafiksystems (GfxBase->
   DisplayFlags), nicht eines einzelnen Screens - daher unabhaengig von
   gScreen/setup() abfragbar. Die globale (nie geoeffnete) "GfxBase"-Variable
   oben im Quelltext wird bewusst nicht verwendet; stattdessen oeffnet diese
   Funktion graphics.library lokal (schattiert die globale GfxBase-Variable
   durch eine lokale mit gleichem Namen, wie in proto/graphics.h vorausgesetzt)
   und schliesst sie danach wieder. */
const char* GetPALNTSCString(void)
{
    struct GfxBase *GfxBase;
    const char *result;

    GfxBase = (struct GfxBase *) OpenLibrary((STRPTR)"graphics.library", 0L);
    if (GfxBase)
    {
        result = (GfxBase->DisplayFlags & PAL) ? "PAL" : "NTSC";
        CloseLibrary((struct Library *)GfxBase);
    }
    else
    {
        result = "Unbekannt";
    }
    return result;
}

/* Liest den Namen des aktuell verwendeten Screenmodes (z.B. "PAL High Res
   Laced") des offenen Public Screens aus. gScreen wird erst von setup()
   gesetzt, das im main()-Ablauf ERST NACH dem myDebug-Block laeuft - diese
   Funktion muss daher NULL-sicher sein und faellt vorher auf "Unbekannt"
   zurueck (dort wird sie ohnehin nur ausgegeben, wenn myDebug bereits vor
   setup() ausgewertet wird). */
const char* GetScreenModeString(void)
{
    static char buffer[DISPLAYNAMELEN];
    struct GfxBase *GfxBase;
    struct NameInfo nameInfo;
    ULONG modeID;
    DisplayInfoHandle handle;

    strcpy(buffer, "Unbekannt");

    if (!gScreen) return buffer;

    GfxBase = (struct GfxBase *) OpenLibrary((STRPTR)"graphics.library", 0L);
    if (!GfxBase) return buffer;

    modeID = GetVPModeID(&gScreen->ViewPort);
    if (modeID != INVALID_ID)
    {
        handle = FindDisplayInfo(modeID);
        if (handle)
        {
            if (GetDisplayInfoData(handle, (UBYTE *)&nameInfo, sizeof(nameInfo), DTAG_NAME, 0))
            {
                /* nameInfo.Name ist in diesem NDK als UBYTE[] deklariert, nicht
                   char[] - Cast vermeidet die reine Vorzeichen-Warnung von
                   strncpy() (kein Bugfix, nur Signedness-Kosmetik). */
                strncpy(buffer, (const char *)nameInfo.Name, sizeof(buffer) - 1);
                buffer[sizeof(buffer) - 1] = '\0';
            }
        }
    }

    CloseLibrary((struct Library *)GfxBase);
    return buffer;
}

/* Das in dieser Toolchain mitgelieferte <graphics/gfxbase.h> kennt fuer
   ChipRevBits0 offenbar nur die (aelteren, bis ECS reichenden) Bits
   GFXB/GFXF_HR_AGNUS und _DENISE, nicht aber die spaeter fuer AGA (Alice/
   Lisa) hinzugekommenen GFXB/GFXF_HR_ALICE/_LISA (Build brach mit
   "undeclared identifier" ab) - deshalb hier als Fallback selbst
   nachgetragen, nur falls das Header sie nicht bereits definiert. Bit 2/3
   fuer Alice/Lisa (nach Bit 0=Agnus, 1=Denise) sind die allgemein
   dokumentierte, stabile Konvention fuer dieses Feld. */
#ifndef GFXB_HR_ALICE
#define GFXB_HR_ALICE 2
#define GFXF_HR_ALICE (1<<GFXB_HR_ALICE)
#endif
#ifndef GFXB_HR_LISA
#define GFXB_HR_LISA 3
#define GFXF_HR_LISA (1<<GFXB_HR_LISA)
#endif

/* Chipset-Erkennung (OCS/ECS/AGA) ueber GfxBase->ChipRevBits0 - dieselbe
   Technik wie bei vielen etablierten Amiga-Systeminfo-Tools: Die Bits
   GFXB_HR_ALICE/GFXB_HR_LISA zeigen AGA-Agnus/AGA-Denise an, GFXB_HR_AGNUS/
   GFXB_HR_DENISE die jeweiligen ECS-Chips. Liegt weder AGA noch ECS vor,
   handelt es sich um den originalen OCS-Chipsatz. HINWEIS: Wie bei
   GetOSVersionString() gilt auch hier - falls auf echter Hardware
   Grenzfaelle (z.B. gemischte ECS/AGA-Bestueckung) anders klassifiziert
   werden sollten als hier angenommen, bitte korrigieren. */
const char* GetChipsetString(void)
{
    struct GfxBase *GfxBase;
    const char *result;

    GfxBase = (struct GfxBase *) OpenLibrary((STRPTR)"graphics.library", 0L);
    if (GfxBase)
    {
        UBYTE rev = GfxBase->ChipRevBits0;

        if ((rev & GFXF_HR_ALICE) || (rev & GFXF_HR_LISA))
            result = "AGA";
        else if ((rev & GFXF_HR_AGNUS) || (rev & GFXF_HR_DENISE))
            result = "ECS";
        else
            result = "OCS";

        CloseLibrary((struct Library *)GfxBase);
    }
    else
    {
        result = "Unbekannt";
    }
    return result;
}

int main( void )
{
	if(myDebug)
	{
		printf("CPU: %s\n", GetCPUTypeString());
		printf("FPU: %s\n", GetFPUTypeString());
		printf("MMU: %s\n", GetMMUTypeString());
		printf("Chip RAM: %s\n", GetChipRAMString());
		printf("Fast RAM: %s\n", GetFastRAMString());
		printf("Total RAM: %s\n", GetTotalRAMString());
		printf("OS-Version: %s\n", GetOSVersionString());
		printf("Kickstart-Version: %s\n", GetKickstartVersionString());
		printf("Workbench-Version: %s\n", GetWorkbenchVersionString());
		printf("PAL/NTSC: %s\n", GetPALNTSCString());
		printf("Screenmode: %s\n", GetScreenModeString());
		printf("Chipset: %s\n", GetChipsetString());
		printf("Dhrystone: %s\n", benchmark_dhrystone());
		printf("MIPS: %s\n", benchmark_mips());
		printf("MFLOPS: %s\n", benchmark_mflops());
		printf("%s\n", compiler_string);
	}
  if ( setup() )
  {
    window_main();
  }
  cleanup();
  return 0;
}
