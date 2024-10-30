#include "arnicsCore.h"


void logo_print()
{
    printf("________________________________________________\n");
    printf("    __      ____    _   _  ___   ____  ____  \n");
    printf("   /  \\    |  _ \\  | \\ | ||_ _| / ___|/ ___| \n");
    printf("  / /\\ \\   | |_) | |  \\| | | | | |    \\___ \\ \n");
    printf(" / ____ \\  |  _ <  | . ` | | | | |     ___) |\n");
    printf("/_/    \\_\\ |_| \\_\\ |_|\\__||___| \\____||____/ \n");
    printf("________________________________________________\n");

}
/*----------------------------------------------------------*/
/*----------------------------------------------------------*/

/*----------------------------------------------------------*/
/*----------------------------------------------------------*/

DEFINE_ARNICS_FUNC_ITEM_RANGE(arnics_init_item, INIT_TAG, 0, 5);

void arnics_core_init()
{
    EXECUTE_FUNC_ALLSECTION(arnics_init_item);
    logo_print();
}


