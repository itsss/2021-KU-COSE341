#include <linux/unistd.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/sched.h>

asmlinkage void sys_print_student_id(void)
{
	// TODO
	printk("My student id is 2020000000\n");
}
EXPORT_SYMBOL_GPL(sys_print_student_id);

asmlinkage void sys_print_student_info(char *name, char *major)
{
	// TODO
	printk("My name is %s\nI major in %s", name, major);
}
EXPORT_SYMBOL_GPL(sys_print_student_info);
