#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system_misc.h> /* cli(), *_flags */
#include <linux/uaccess.h>
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/timer.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
MODULE_LICENSE("Dual BSD/GPL");

// Declaring fixed variables
#define DEVICE_NAME "mytimer"
#define MAX_TIMERS 2
#define MAX_MESSAGE_LENGTH 128
#define BUF_SIZE 800 // this is for sending back the data

// declaring functions that we are using in the kernel module
static int mytimer_open(struct inode *inode, struct file *filp);
static int mytimer_release(struct inode *inode, struct file *filp);
static ssize_t mytimer_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static void mytimer_exit(void);
static int mytimer_init(void);
static int mytimer_fasync(int fd, struct file *filp, int mode);


// global variables
static int num_timers = 1; // default to 1 upon module load
static int active_timers = 0;
static int mytimer_major = 61;
struct fasync_struct *async_queue; /* structure for keeping track of asynchronous readers */
static char *mytimer_buffer; //This buffer is for getting the commands from user-space program
static char *mytimer_buffer_proc, *mytimer_proc_ptr; //This buffer is setting up the proc file
static char *mytimer_buffer_read, *mytimer_read_ptr; // These buffers are for sending messages back to user-space program
static struct proc_dir_entry *proc_entry; // These are for creating the proc file
static unsigned long jiffies_per_millisecond; // for setting up the first time that our module is loaded and saving the value for calculating the time 
static unsigned long module_load_jiffies;
// operations that we have for fasync and timers
static struct file_operations fops = {
    .open = mytimer_open,
    .write = mytimer_write,
    .release = mytimer_release,
    .fasync = mytimer_fasync
};

module_init(mytimer_init);
module_exit(mytimer_exit);

// Function for initializing the module
static int mytimer_init(void) {
    int result;
    int i;
    int ret = 0;
    result = register_chrdev(mytimer_major, "laserkeyboard", &fops);
    if (result < 0) {
        printk(KERN_ALERT "laserkeyboard: cannot obtain major number \n");
        return result;
    }
    printk(KERN_ALERT "Module is enable now!\n");
    /*allocating memory to buffers for reading and writing*/
    mytimer_buffer = kmalloc(128, GFP_KERNEL);
    mytimer_buffer_read = kmalloc(BUF_SIZE, GFP_KERNEL);
    memset(mytimer_buffer, 0, 128);
    memset(mytimer_buffer_read, 0, BUF_SIZE);

    return result;
}

// Function for cleaning up resources when unloading the module
static void mytimer_exit(void) {
    printk(KERN_INFO "Exiting my module\n");
    unregister_chrdev(mytimer_major, DEVICE_NAME);
    if (mytimer_buffer) {
        kfree(mytimer_buffer);
        kfree(mytimer_buffer_read);
    }
    printk(KERN_ALERT "Removing module\n");
}

// Function for opening the device file
static int mytimer_open(struct inode *inode, struct file *filp) {
    return 0;
}

// Function for releasing resources when unloading the module which sends a asynchronous SIG
static int mytimer_release(struct inode *inode, struct file *filp) {
    return 0;
}

// Handling fasync when we are finished with a timer
static int mytimer_fasync(int fd, struct file *filp, int mode) {
printk(KERN_ALERT "sending back the signal!\n");
    return fasync_helper(fd, filp, mode, &async_queue);
}

// Function for writing to the device file
static ssize_t mytimer_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos) {
    int temp;
    char tbuf[256], *tbptr = tbuf;
    memset(mytimer_buffer_read, 0, BUF_SIZE);
    if (count > 128) {
        count = 128;
    }

    if (copy_from_user(mytimer_buffer, buf, count)) { // getting the message from the user-space program
        return -EFAULT;
    }
    
    for (temp = 0; temp < count; temp++)
        tbptr += sprintf(tbptr, "%c", mytimer_buffer[temp]);
	printk(KERN_ALERT "we are writing!!!\n");
    mytimer_fasync(-1, filp, 0);
    return count;
}
