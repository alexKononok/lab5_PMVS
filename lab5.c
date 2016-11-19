#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#define BUF_LEN 80

int init_module(void);

void cleanup_module(void);

static ssize_t dev_first_write(struct file *filp, const char *buffer, size_t length, loff_t *offset);

static ssize_t dev_second_write(struct file *filp, const char *buffer, size_t length, loff_t *offset);

static ssize_t dev_operator_write(struct file *filp, const char *buffer, size_t length, loff_t *offset);

static ssize_t proc_read(struct file *filp, char *buffer, size_t length, loff_t *offset);

static int operand_1;
static int operand_2;
static int operator;

static char result[BUF_LEN];
static char first[BUF_LEN];
static char second[BUF_LEN];
static char operand[BUF_LEN];

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = proc_read
};

static struct file_operations dev_first_fops = {
	.owner = THIS_MODULE,
	.write = dev_first_write,
};

static struct file_operations dev_second_fops = {
	.owner = THIS_MODULE,
	.write = dev_second_write
};

static struct file_operations dev_operator_fops = {
	.owner = THIS_MODULE,
	.write = dev_operator_write
};

static struct proc_dir_entry *proc_result;

int init_module(void) {
	memset(result, 0, 3);
	operand_1 = register_chrdev(0, "first", &dev_first_fops);
	if (operand_1 < 0) {
		printk(KERN_ALERT
		"Registering char device failed with %d\n", operand_1);
		return operand_1;
	}
	operand_2 = register_chrdev(0, "second", &dev_second_fops);
    
	if (operand_2 < 0) {
		printk(KERN_ALERT
		"Registering char device failed with %d\n", operand_2);
		return operand_2;
	}
	operator = register_chrdev(0, "operator", &dev_operator_fops);
    
	if (operator < 0) {
		printk(KERN_ALERT
		"Registering char device failed with %d\n", operator);
		return operator;
	}
	proc_result = proc_create("result", 0, NULL, &fops);
    
	if (proc_result == NULL) {
		printk(KERN_ALERT
		"Creating proc failed\n");
		return -1;
	}

	return 0;
}

void cleanup_module(void) {
	unregister_chrdev(operand_1, "first");
	unregister_chrdev(operand_2, "second");
	unregister_chrdev(operator, "operator");
	remove_proc_entry("result", NULL);
}

static ssize_t proc_read(struct file *filp, char *buffer, size_t length, loff_t *offset) {
	ssize_t cnt;
	ssize_t ret;
	int nfirst;
	int nsecond;
	int nresult;
	sscanf(first, "%d", &nfirst);
	sscanf(second, "%d", &nsecond);
	if (operand[0] == 'p') {
		nresult = nfirst * nsecond;
		sprintf(result, "%d", nresult);
	}
	else if (operand[0] == '-') {
		nresult = nfirst - nsecond;
		sprintf(result, "%d", nresult);
	}
	else if (operand[0] == '+') {
		nresult = nfirst + nsecond;
		sprintf(result, "%d", nresult);
	}
	else if (operand[0] == '/' && nsecond != 0) {
		nresult = nfirst / nsecond;
		sprintf(result, "%d", nresult);
	}
	else {
		sprintf(result, "NaN");
	}
	cnt = strlen(result);
	ret = copy_to_user(buffer, result, cnt);
	*offset += cnt - ret;
	if (*offset > cnt)
		return 0;
	else
		return cnt;
}

static ssize_t dev_first_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
	if (copy_from_user(first, buff, len)) {
		return -EFAULT;
	}
	return len;
}

static ssize_t dev_second_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
	if (copy_from_user(second, buff, len)) {
		return -EFAULT;
	}
	return len;
}

static ssize_t dev_operator_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
	if (copy_from_user(operand, buff, len)) {
		return -EFAULT;
	}
	return len;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexander Kononok");
