#include <linux/init.h>
#include<linux/acpi.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/random.h>
#include <linux/uaccess.h>
#include <linux/moduleparam.h>

#define DRIVER_NAME "epirandom"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("epirandom module");
MODULE_AUTHOR("sabir.mohamed-ben-ali");

static int nb_major; 
static ssize_t epirandom_proc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    int i;
    char *kbuf;
    kbuf = kmalloc(count, GFP_KERNEL);
    if (!kbuf)
        return -ENOMEM;
    for (i = 0; i < count; i++)
        kbuf[i] = get_random_int() % 256;
    if (copy_to_user(buf, kbuf, count))
        return -EFAULT;
    kfree(kbuf);
    return count;
}

static ssize_t epirandom_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    pr_info("epirandom: write\n");
    return count;
}

static int epirandom_proc_open(struct inode *inode, struct file *file)
{
    pr_info("epirandom: open\n");
    return 0;
}

static int epirandom_proc_release(struct inode *inode, struct file *file)
{
    pr_info("epirandom: release\n");
    return 0;
}

static struct file_operations epirandom_proc_ops = {
    .read = epirandom_proc_read,
    .write = epirandom_proc_write,
    .open = epirandom_proc_open,
    .release = epirandom_proc_release,
};


static __init int epirandom_init(void)
{
    pr_info("epirandom: init epirandom module\n");
    nb_major = register_chrdev(0, "epirandom", &epirandom_proc_ops);
    pr_info ("epirandom: major number \"%d \\\"\n", nb_major);
    return 0;
}

static __exit void epirandom_exit(void)
{
    unregister_chrdev(nb_major, "epirandom");
    pr_info("epirandom: Exit.\n");
}


module_init(epirandom_init);
module_exit(epirandom_exit);

