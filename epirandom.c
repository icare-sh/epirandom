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
static char * alphabet = NULL;
static int nb_major;

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("epirandom module");
MODULE_AUTHOR("sabir.mohamed-ben-ali");

module_param(alphabet, charp, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);  
MODULE_PARM_DESC(alphabet, "alphabet");
 
 /*
* @brief: function to handler alphabet with comma separted values
*/
 int handler_comma(char *str)
 {  
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (i%2 != 0 && str[i] != ',')
            return 1;   //return 1 if charater in odd position is not a comma
    }
    return 0;  //return 0 if all charaters in odd position are commas
 }

/*
* @brief: function to generate random pair number  between 0 and size of alphabet
*/
int generate_random_number(int n)
{
    int random_number;
    get_random_bytes(&random_number, sizeof(int));
    random_number = random_number % n;
    if (!(random_number % 2 == 0))
        random_number++;
    if (random_number < 0)
        random_number = -random_number;
    return random_number;   
}

static int epirandom_proc_open(struct inode *inode, struct file *file)
{
    pr_info("epirandom: open\n");
    return 0;
}

static ssize_t epirandom_proc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    int i;
    char *kbuf;
    ssize_t ret;
    kbuf = kmalloc(count, GFP_KERNEL);
    if (!kbuf)
        return -ENOMEM;
    if (alphabet == NULL)
        get_random_bytes(kbuf, count);   //if alphabet is not set, generate random bytes
    else
        for (i = 0; i < count; i++)
        {
            if (handler_comma(alphabet))
                kbuf[i] = alphabet[get_random_int()%strlen(alphabet)]; //if alphabet is set and no comma separate, generate random charater from alphabet
            else
                kbuf[i] = alphabet[generate_random_number(strlen(alphabet))]; //if alphabet is set and comma separate, generate random number between 0 and size of alphabet
        }
    ret = copy_to_user(buf, kbuf, count);
    kfree(kbuf);
    if (ret)
        return -EFAULT;
    return count;
}

static ssize_t epirandom_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    pr_info("epirandom: write\n");
    return count;
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
