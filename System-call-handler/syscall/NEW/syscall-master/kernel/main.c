#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/uaccess.h>
#include <linux/gfp.h>
#include "main.h"
#include "klib.h"
#include <linux/slab.h>
MODULE_LICENSE("GPL");
static struct idt_desc *old_table;
static struct idt_desc *new_table;

extern void syscall_handler(int id, void* arg);

/* Allocates a new IDT via imp_copy_idt.
 * Index 15 of IDT is used for
 * custom system call handler. Copies
 * the entry corresponding to the current
 * system call handler (index 128) to 
 * index 15 in the new IDT. Sets the lower16 
 * and higher16 fields at index 15 to the 
 * lower 16 bits and higher 16 bits
 * of syscall_handler respectively. Loads
 * the new IDT using imp_load_idt. Saves
 * the original IDT in a global variable.
 */
static void register_syscall(void)
{
	struct idt_entry *my_system_call;
	struct idt_entry *no128_syscall;
	unsigned long bits = (unsigned long)(syscall_handler) & 4294901760LL;  //higher 16 bits
	old_table=(struct idt_desc*)kmalloc(sizeof(struct idt_desc),GFP_KERNEL);
	new_table=(struct idt_desc*)kmalloc(sizeof(struct idt_desc),GFP_KERNEL);
	
	if(imp_copy_idt(new_table) == 0 ){
		printk("CSE231: Cannot Register\n");
		return;
	}
	
	//---- Modifying the new table---//
	
	bits=bits>>16;
	no128_syscall = new_table->base + 128;
	my_system_call = new_table->base + 15;
	my_system_call->flags = no128_syscall->flags;
	my_system_call->dpl = no128_syscall->dpl;
	my_system_call->present = no128_syscall->present;
	my_system_call->sel = no128_syscall->sel;
	my_system_call->higher16=(short)(bits);
	my_system_call->lower16=(short)((unsigned long)(syscall_handler) & ~4294901760LL);

	//--- Loading the new table ---//
	printk("loading table\n");
	imp_load_idt(new_table, old_table);
	printk("table loaded\n");

}

/* If the current IDT is not the original one
 * (can be checked using the global variable),
 * loads the original IDT using imp_load_idt.
 * Frees the previous IDT using imp_free_desc.
 */
static void unregister_syscall(void)
{
	if(new_table==NULL || old_table==NULL)
		return;

	if(old_table->base!=new_table->base){
		imp_load_idt(old_table, new_table);
		imp_free_desc(new_table);
		kfree(new_table);
		kfree(old_table);
		new_table=NULL;
		old_table=NULL;
	}
}

static long device_ioctl(struct file *file, unsigned int ioctl, unsigned long arg)
{
	switch (ioctl)
	{
		case REGISTER_SYSCALL:
			register_syscall();
			break;
		case UNREGISTER_SYSCALL:
			unregister_syscall();
			break;
		default:
			printk("NOT a valid ioctl\n");
			return 1;
	}
	return 0;
}

static struct file_operations fops = 
{
	.unlocked_ioctl = device_ioctl,
};

int start_module(void)
{
	if (register_chrdev(MAJOR_NUM, MODULE_NAME, &fops) < 0)
	{
		printk("PANIC: module loading failed\n");
		return 1;
	}
	printk("module loaded successfully\n");
	return 0;
}

void exit_module(void)
{
	unregister_chrdev(MAJOR_NUM, MODULE_NAME);
	printk("module unloaded successfully\n");
}

module_init(start_module);
module_exit(exit_module);
