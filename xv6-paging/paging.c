#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
#include "paging.h"
#include "fs.h"

#define PTE_SWP1 128
// #define PTE_SWP2 256


struct inode {
	uint dev;
	// Device number
	uint inum;
	// Inode number
	int ref;
	// Reference count
	int flags;
	// I_BUSY, I_VALID
	short type;
	// copy of disk inode
	short major;
	short minor;
	short nlink;
	uint size;
	uint addrs[NDIRECT+1];
};

/* Allocate eight consecutive disk blocks.
 * Save the content of the physical page in the pte
 * to the disk blocks and save the block-id into the
 * pte.
 */
void
swap_page_from_pte(pte_t *pte)
{
	// cprintf("SWP: got address %d\n",*pte);
	struct proc *curproc = myproc();
	
	// cprintf("Going to call balloc_page\n");
	begin_op();
	if (*pte&PTE_SWP1)
    {
    	return;

    }
	uint bid = balloc_page(curproc->cwd->dev);          
	// cprintf("Returned balloc_page\n");
	*pte = (pte_t)P2V(PGROUNDDOWN(*pte));	
	write_page_to_disk(curproc->cwd->dev,(void*)*pte,bid);	
    end_op();
	
	bid=bid<<9;										//Assuming the size of memory is 4GB at max. Then bid can be represented in 23 bytes, 
	bid=bid|PTE_SWP1;								//Making 7th the LSB 1 representing that the page is swapped
	// bid=bid|PTE_SWP2;								//Making 8th the LSB 1 representing that the page is swapped

	// cprintf("swapping\n");
	kfree((void*)*pte);								
	*pte=bid; //updating the PTE
	// cprintf("swapping DONE0000\n");
	return;
}

/* Select a victim and swap the contents to the disk.
 */
int
swap_page(pde_t *pgdir)
{
	pte_t *to_swap;
	// do{

	// 	to_swap=select_a_victim(pgdir);
	// 	if(to_swap==0){
	// 		cprintf("SWP_PG: Clearing the access bit");
	// 		clearaccessbit(pgdir);
	// 	}
	// }while(to_swap==0);
	clearaccessbit(pgdir);
	to_swap=select_a_victim(pgdir);
	
	swap_page_from_pte(to_swap);
	// cprintf("SP: RETURN\n");
	return 1;
}

/* Map a physical page to the virtual address addr.
 * If the page table entry points to a swapped block
 * restore the content of the page from the swapped
 * block and free the swapped block.
 */
void
map_address(pde_t *pgdir, uint addr)
{
	uint pa;
	short int successful=0;
	pte_t *curEntry;
	curEntry = uva2pte(pgdir,addr);
	
	if ((*curEntry&PTE_SWP1) /*&& (*curEntry&PTE_SWP2)*/)
	{	
		// panic("Swapped page found");		

		uint bid=(*curEntry)>>9;
		// cprintf("Swapped page found: got %d\n",bid);		

		while(!successful){
			pa=(uint)kalloc();
			if(pa==0){
						
				swap_page(pgdir);
				// panic("MTPANIC: No mem available");
			}else{
				successful=1;
			}
		}
		struct proc *curproc = myproc();

		// begin_op();
		read_page_from_disk(curproc->cwd->dev, (void*)pa, bid);
		// end_op();

		begin_op();
		bfree_page(curproc->cwd->dev,bid);
		end_op();

		*curEntry = V2P((PGROUNDDOWN(pa)|PTE_P|PTE_W|PTE_U));

	}
	else{
		while(!successful){
			pa=(uint)kalloc();
			if(pa==0){
						
				swap_page(pgdir);
				// panic("MTPANIC: No mem available");
			}else{
				successful=1;
			}
		}
		
			
		*curEntry = V2P((PGROUNDDOWN(pa)|PTE_P|PTE_W|PTE_U));
	}
}

/* page fault handler */
void
handle_pgfault()
{
	unsigned addr;
	struct proc *curproc = myproc();

	asm volatile ("movl %%cr2, %0 \n\t" : "=r" (addr));
	addr &= ~0xfff;
	// cprintf("::::::::::::::::::::::pgflt at %d\n", addr);
	map_address(curproc->pgdir, addr);
}
