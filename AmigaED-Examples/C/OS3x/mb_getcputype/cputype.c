/*
 * CPUtype.c
 * Welche Hardware ist im Rechner?
 * @ MB-Softworks 5/96
 *
 */

#include <exec/types.h>
#include <exec/execbase.h>
#include <exec/memory.h>
#include <proto/exec.h>
#include <graphics/gfxbase.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Function Prototypes */
void ShowMem(void);
char * ShowChips(void);


char *ver = "\0$VER: CheckProcessorBoard 1.2 (22.05.96) © Michael Bergmann";

/*struct ExecBase *AbsSysBase = *((struct ExecBase **)4);
struct GfxBase *GfxBase = NULL;*/

int main(void)
{
   
    struct ExecBase *AbsSysBase = *((struct ExecBase **)4);
 //   struct Library *GfxBase = NULL;

    UWORD flags;
    char cpuname[26], fpuname[25], chipname[5];

    strcpy(cpuname,"CPU: MOTOROLLA MC68000");
    strcpy(fpuname,"FPU: nicht vorhanden");

    flags = AbsSysBase->AttnFlags;


    if (flags & AFF_68060)
	{         strcpy(cpuname,"68060");
	     strcpy(fpuname,"68060");
	  // Keine Ahnunng, wie das hier mit EC und LC aussieht 
   } 
    else if (flags & AFF_68040)
    {
      if (flags & AFF_FPU40)
      {
	strcpy(cpuname,"68(EC)040");
	strcpy(fpuname,"68040");
      }
      else strcpy(cpuname,"68LC040");
    }
    else
	{
	  if (flags & AFF_68881) strcpy(fpuname,"68881");
	  if (flags & AFF_68882) strcpy(fpuname,"68882");

	  if (flags & AFF_68030) strcpy(cpuname,"68(EC)030");
	  else if (flags & AFF_68020) strcpy(cpuname,"68(EC)020");
	  else if (flags & AFF_68010) strcpy(cpuname,"68010");
	  else strcpy(cpuname,"68000");
	}

    strcpy(chipname, ShowChips() );

    printf("\n\033[1m\033[1m\033[11mCheckProcessorBoard v1.1  © 5/'96 by Michael Bergmann\033[0m\n>CPU:\t\t%s\n>CoPro:\t\t%s\n>Chipset:\t%s\n", cpuname, fpuname, chipname);
	
    ShowMem();

}

void ShowMem(void)
{
    printf(">Total CHIP: \t%6.3f MB, available: %9ld Bytes\n", ((float)(AvailMem(MEMF_CHIP|MEMF_TOTAL)) / 1000000), AvailMem(MEMF_CHIP|MEMF_LARGEST));
    printf(">Total FAST: \t%6.3f MB, available: %9ld Bytes\n", ((float)(AvailMem(MEMF_FAST|MEMF_TOTAL)) / 1000000), AvailMem(MEMF_FAST|MEMF_LARGEST));
    printf(">Total RAM: \t%6.3f MB (%ld Bytes) \n\n", ((float)(AvailMem(MEMF_TOTAL))/1000000), AvailMem(MEMF_TOTAL) );
}

char * ShowChips(void)
{
    struct GfxBase *GfxBase = NULL; 
    UWORD flags;
    char chipname[5];


    if ( NULL != (GfxBase = (struct GfxBase * )OpenLibrary((UBYTE *)"graphics.library" , 37)))
	{

	    flags = GfxBase->ChipRevBits0;
	    /* AGA */
	    if (flags & GFXF_AA_MLISA)
				strcpy(chipname, "AGA");
	    /* ECS */
	    else if (flags & GFXF_HR_AGNUS)
		{
		    if (flags & GFXF_HR_DENISE)
		       strcpy(chipname, "ECS");
		}
	    /* OLD */
	    else
		strcpy(chipname, "Alt");


	    CloseLibrary( ( struct Library * )GfxBase );

	    return(chipname);

	}

}

