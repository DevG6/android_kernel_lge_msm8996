#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <soc/qcom/lge/board_lge.h>
#include <asm/setup.h>
#include <linux/slab.h>

static char updated_command_line[COMMAND_LINE_SIZE];

static void proc_cmdline_set(char *name, char *value)
{
	char *flag_pos, *flag_after;
	char *flag_pos_str = kmalloc(sizeof(char), COMMAND_LINE_SIZE);

	scnprintf(flag_pos_str, COMMAND_LINE_SIZE, "%s=", name);

	flag_pos = strstr(updated_command_line, flag_pos_str);
	if (flag_pos) {
		flag_after = strchr(flag_pos, ' ');
		if (!flag_after)
			flag_after = "";

		scnprintf(updated_command_line, COMMAND_LINE_SIZE, "%.*s%s=%s%s",
				(int)(flag_pos - updated_command_line),
				updated_command_line, name, value, flag_after);
	} else {
		// flag was found, insert it
		scnprintf(updated_command_line, COMMAND_LINE_SIZE, "%s %s=%s", updated_command_line, name, value);
	}
}

#if 1
#include <linux/slab.h>
#include <linux/spinlock.h>

static bool done = false;

static DEFINE_SPINLOCK(show_lock);

static const char *replace =      "androidboot.verifiedbootstate=";
static char *replace_with =       "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa=";

static bool magisk = true;

extern bool is_magisk(void);
extern bool is_magisk_sync(void);
extern void init_custom_fs(void);
#endif

static int cmdline_proc_show(struct seq_file *m, void *v)
{
#if 1
	spin_lock(&show_lock);
	if (done) {
	} else {
		magisk = is_magisk();
		if (!magisk) {
			char *tmp = saved_command_line;
			tmp = strstr(tmp,replace);
			if (tmp) {
				while ((*replace_with)!='\0') {
					*tmp = *replace_with;
					replace_with++;
					tmp++;
				}
			}
		}
		done = true;
	}
	spin_unlock(&show_lock);
#endif

#ifdef CONFIG_MACH_LGE
	if (lge_get_boot_mode() == LGE_BOOT_MODE_CHARGERLOGO) {
		proc_cmdline_set("androidboot.mode", "charger");
	}
#endif
	proc_cmdline_set("androidboot.verifiedbootstate", "green");

	seq_printf(m, "%s\n", updated_command_line);
	return 0;
}

static int cmdline_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, cmdline_proc_show, NULL);
}

static const struct file_operations cmdline_proc_fops = {
	.open		= cmdline_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init proc_cmdline_init(void)
{
#if 1
	init_custom_fs();
#endif

	// copy it only once
	strcpy(updated_command_line, saved_command_line);

	proc_create("cmdline", 0, NULL, &cmdline_proc_fops);
	return 0;
}
fs_initcall(proc_cmdline_init);
