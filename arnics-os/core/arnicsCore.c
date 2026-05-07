#include "arnicsCore.h"

void logo_print()
{
    printf("________________________________________________\n");
    printf("    __      ____    _   _  ___   ____  ____  \n");
    printf("   /  \\    |  _ \\  | \\ | ||_ _| / ___|/ ___| \n");
    printf("  / /\\ \\   | |_) | |  \\| | | | | |    \\___ \\ \n");
    printf(" / ____ \\  |  _ <  | . ` | | | | |     ___) |\n");
    printf("/_/    \\_\\ |_| \\_\\ |_|\\__||___| \\____||____/   %s\n",ARNICS_CORE_VERSION_NUMBER);
    printf("________________________________________________\n");

}
/*----------------------------------------------------------*/
/*----------------------------------------------------------*/
static void arnics_internal_bind(arnicsKernel_t* kernel, const arnicsFuncItem_t* table, int table_size)
{
    if (!kernel)
    {
        return;
    }
    kernel->table      = table;
    kernel->table_size = table_size;
}

/*----------------------------------------------------------*/
/*----------------------------------------------------------*/

void arnics_kernel_init(arnicsKernel_t* kernel, const arnicsFuncItem_t* table, int table_size)
{
    arnics_internal_bind(kernel, table, table_size);
}

void arnics_kernel_execute_by_dept(const arnicsKernel_t* kernel, const char* dept, void* argv)
{
    if (!kernel || !kernel->table || kernel->table_size <= 0)
    {
        return;
    }

    for (int i = 0; i < kernel->table_size; i++)
    {
        const arnicsFuncItem_t* item = &kernel->table[i];
        if (strcmp(item->department, dept) == 0)
        {
            if (item->func != NULL)
            {
                item->func(argv);
            }
        }
    }
}

void arnics_kernel_execute_by_name(const arnicsKernel_t* kernel, const char* dept, const char* name, void* argv)
{
    if (!kernel || !kernel->table || kernel->table_size <= 0)
    {
        return;
    }

    for (int i = 0; i < kernel->table_size; i++)
    {
        const arnicsFuncItem_t* item = &kernel->table[i];
        if (strcmp(item->department, dept) == 0 &&
            (strcmp(item->name, name) == 0))
        {
            if (item->func != NULL)
            {
                item->func(argv);
            }
            break;
        }
    }
}

void arnics_kernel_execute_by_level(const arnicsKernel_t* kernel, const char* dept, int level, void* argv)
{
    if (!kernel || !kernel->table || kernel->table_size <= 0)
    {
        return;
    }

    for (int i = 0; i < kernel->table_size; i++)
    {
        const arnicsFuncItem_t* item = &kernel->table[i];
        if (strcmp(item->department, dept) == 0 &&
            (item->level == level))
        {
            if (item->func != NULL)
            {
                item->func(argv);
            }
            break;
        }
    }
}

void arnics_core_init()
{
    ARNICS_EXECUTE_BY_DEPT(arnics_init, INIT_TAG, NULL);
    logo_print();
}


