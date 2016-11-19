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

