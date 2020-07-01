// Buffer cache.
//
// The buffer cache is a linked list of buf structures holding
// cached copies of disk block contents.  Caching disk blocks
// in memory reduces the number of disk reads and also provides
// a synchronization point for disk blocks used by multiple processes.
//
// Interface:
// * To get a buffer for a particular disk block, call bread.
// * After changing buffer data, call bwrite to write it to disk.
// * When done with the buffer, call brelse.
// * Do not use the buffer after calling brelse.
// * Only one process at a time can use a buffer,
//     so do not keep them longer than necessary.
//
// The implementation uses two state flags internally:
// * B_VALID: the buffer data has been read from the disk.
// * B_DIRTY: the buffer data has been modified
//     and needs to be written to disk.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "buf.h"

struct {
  struct spinlock lock;
  struct buf buf[NBUF];

  // Linked list of all buffers, through prev/next.
  // head.next is most recently used.
  struct buf head;
} bcache;

void
binit(void)
{
  struct buf *b;

  initlock(&bcache.lock, "bcache");

//PAGEBREAK!
  // Create linked list of buffers
  bcache.head.prev = &bcache.head;
  bcache.head.next = &bcache.head;
  for(b = bcache.buf; b < bcache.buf+NBUF; b++){
    b->next = bcache.head.next;
    b->prev = &bcache.head;
    initsleeplock(&b->lock, "buffer");
    bcache.head.next->prev = b;
    bcache.head.next = b;
  }
}

// Look through buffer cache for block on device dev.
// If not found, allocate a buffer.
// In either case, return locked buffer.
static struct buf*
bget(uint dev, uint blockno)
{
  struct buf *b;

  acquire(&bcache.lock);

  // Is the block already cached?
  for(b = bcache.head.next; b != &bcache.head; b = b->next){
    if(b->dev == dev && b->blockno == blockno){
      b->refcnt++;
      release(&bcache.lock);
      acquiresleep(&b->lock);
      return b;
    }
  }

  // Not cached; recycle an unused buffer.
  // Even if refcnt==0, B_DIRTY indicates a buffer is in use
  // because log.c has modified it but not yet committed it.
  for(b = bcache.head.prev; b != &bcache.head; b = b->prev){
    if(b->refcnt == 0 && (b->flags & B_DIRTY) == 0) {
      b->dev = dev;
      b->blockno = blockno;
      b->flags = 0;
      b->refcnt = 1;
      release(&bcache.lock);
      acquiresleep(&b->lock);
      return b;
    }
  }
  panic("bget: no buffers");
}

void
memmove2(uchar *dst, const uchar *src, uint n, int diff)
{

  for (int i = 0; i < n; ++i)
  {
    dst[i]=src[i+diff];
  }
 
}

/* Write 4096 bytes pg to the eight consecutive
 * starting at blk.
 */
void
write_page_to_disk(uint dev, void *pg, uint bid)
{
  //pg is a virtual address;
  // cprintf("WTD: bread\n");
  struct buf* b1=bread(dev, bid+0);
  struct buf* b2=bread(dev, bid+1);
  struct buf* b3=bread(dev, bid+2);
  struct buf* b4=bread(dev, bid+3);
  struct buf* b5=bread(dev, bid+4);
  struct buf* b6=bread(dev, bid+5);
  struct buf* b7=bread(dev, bid+6);
  struct buf* b8=bread(dev, bid+7);
  // cprintf("WTD: memmove1\n");
  memmove2(b1->data, (pg), BSIZE, BSIZE*0);
  // cprintf("WTD: memmove2\n");
  memmove2(b2->data, (pg), BSIZE, BSIZE*1);
  // cprintf("WTD: memmove3\n");
  memmove2(b3->data, (pg), BSIZE, BSIZE*2);
  // cprintf("WTD: memmove4\n");
  memmove2(b4->data, (pg), BSIZE, BSIZE*3);
  // cprintf("WTD: memmove5\n");
  memmove2(b5->data, (pg), BSIZE, BSIZE*4);
  // cprintf("WTD: memmove6\n");
  memmove2(b6->data, (pg), BSIZE, BSIZE*5);
  // cprintf("WTD: memmove7\n");
  memmove2(b7->data, (pg), BSIZE, BSIZE*6);
  // cprintf("WTD: memmove8\n");
  memmove2(b8->data, (pg), BSIZE, BSIZE*7);
  // cprintf("WTD: bwrite\n");
  bwrite(b1);
  bwrite(b2);
  bwrite(b3);
  bwrite(b4);
  bwrite(b5);
  bwrite(b6);
  bwrite(b7);
  bwrite(b8);
  // cprintf("WTD: brelease\n");
  brelse(b1);  
  brelse(b2);  
  brelse(b3);  
  brelse(b4);  
  brelse(b5);  
  brelse(b6);  
  brelse(b7);  
  brelse(b8);  
  // cprintf("WTD: DONE\n");
}

void
memmove3(uchar *dst, const uchar *src, uint n, int diff)
{

  for (int i = 0; i < n; ++i)
  {
    dst[i+diff]=src[i];
  }
 
}

/* Read 4096 bytes from the eight consecutive
 * starting at blk into pg.
 */
void
read_page_from_disk(uint dev, void *pg, uint bid)
{
  //pg is a virtual address;
  
  
  // cprintf("RTD: bread\n");
  // cprintf("RTD: bid = %d\n",bid);
  struct buf* b1=bread(dev, bid+0);
  struct buf* b2=bread(dev, bid+1);
  struct buf* b3=bread(dev, bid+2);
  struct buf* b4=bread(dev, bid+3);
  struct buf* b5=bread(dev, bid+4);
  struct buf* b6=bread(dev, bid+5);
  struct buf* b7=bread(dev, bid+6);
  struct buf* b8=bread(dev, bid+7);
  // cprintf("RTD: memmove1\n");
  memmove3((void*)(pg), b1->data,BSIZE, BSIZE*0);
  // cprintf("RTD: memmove2\n");
  memmove3((void*)(pg), b2->data,BSIZE, BSIZE*1);
  // cprintf("RTD: memmove3\n");
  memmove3((void*)(pg), b3->data,BSIZE, BSIZE*2);
  // cprintf("RTD: memmove4\n");
  memmove3((void*)(pg), b4->data,BSIZE, BSIZE*3);
  // cprintf("RTD: memmove5\n");
  memmove3((void*)(pg), b5->data,BSIZE, BSIZE*4);
  // cprintf("RTD: memmove6\n");
  memmove3((void*)(pg), b6->data,BSIZE, BSIZE*5);
  // cprintf("RTD: memmove7\n");
  memmove3((void*)(pg), b7->data,BSIZE, BSIZE*6);
  // cprintf("RTD: memmove8\n");
  memmove3((void*)(pg), b8->data,BSIZE, BSIZE*7);

  // cprintf("RTD: brelease\n");
  brelse(b1);  
  brelse(b2);  
  brelse(b3);  
  brelse(b4);  
  brelse(b5);  
  brelse(b6);  
  brelse(b7);  
  brelse(b8);    
  // cprintf("RTD: DONE\n");
}

// Return a locked buf with the contents of the indicated block.
struct buf*
bread(uint dev, uint blockno)
{
  struct buf *b;

  b = bget(dev, blockno);
  if((b->flags & B_VALID) == 0) {
    iderw(b);
  }
  return b;
}

// Write b's contents to disk.  Must be locked.
void
bwrite(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("bwrite");
  b->flags |= B_DIRTY;
  iderw(b);
}

// Release a locked buffer.
// Move to the head of the MRU list.
void
brelse(struct buf *b)
{
  if(!holdingsleep(&b->lock))
    panic("brelse");
	
  releasesleep(&b->lock);

  acquire(&bcache.lock);
  b->refcnt--;
  if (b->refcnt == 0) {
    // no one is waiting for it.
    b->next->prev = b->prev;
    b->prev->next = b->next;
    b->next = bcache.head.next;
    b->prev = &bcache.head;
    bcache.head.next->prev = b;
    bcache.head.next = b;
  }
  
  release(&bcache.lock);
}
//PAGEBREAK!
// Blank page.

