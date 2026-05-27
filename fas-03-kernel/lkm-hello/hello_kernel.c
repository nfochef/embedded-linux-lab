#include <linux/init.h>  /* __init och __exit macros */
#include <linux/module.h> /* modul init, module exit, module * macros */
#include <linux/kernel.h> /* pr_info och andra kernel print funktioner */


MODULE_LICENSE("MIT");   /* obligatorisk man behöver en module licens annars fel: "tainted kernel"*/
MODULE_AUTHOR("jens");
MODULE_DESCRIPTION("My first LKM");
MODULE_VERSION("0.1");

/* init macrot behövs aldrig igen, kernel samlar all __init kod i en speciell sektion och frigör den senare */

static int __init hello_init(void) /* __init macrot,"denna funktion behövs bara vid load, när den är körd firgör minne*/
{
    pr_info("hello_kernel: module loaded, greetings kernel kraken\n"); /* printk = systemets log samma som läs med dmesg */
    pr_info("hello_kernel: code with kernel priv's\n");
    return 0; /* vanliga retur värden -EINVAL, -ENOMEM*/
}



static void __exit hello_exit(void)
{
        pr_info("hello_kernel:  module unloaded, Bye!\n");
}

/* de macros nedan säger till kernel vilka funktioner som är init och exit man behöver deklarera båda annars ladds ej modulen */

module_init(hello_init);
module_exit(hello_exit);

