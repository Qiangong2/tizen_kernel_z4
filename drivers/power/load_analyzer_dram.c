
/* drivers/power/load_analyzer_dram.c */

#define DDR_FREQ	640

#define DRAM_TIMING_NAME_LEN	16

#define NA 0


#define tRFCAB  2100  /* 210ns unit is per 10ns */
#define tRFCPB	900   /* 90ns  unit is per 10ns */


enum {
	MIN_TYPE,
	MAX_TYPE,
	MIN_MAX_TYPE,
};
const char minmax_string[][10] = {
	{"min"},
	{"max"},
	{"min/max"},
};

enum {
	TCK,
	NS,
};
const char unit_string[][10] = {
	{"[tCK]"},
	{"[ NS]"},
};


struct dram_param_spec_tag {
	unsigned int spec_type;
	unsigned int unit_type;
	unsigned int min_100ps;
	unsigned int min_tck;
	unsigned int max_time;
};


struct dram_timing_tag {
	char name[DRAM_TIMING_NAME_LEN];
	unsigned int phyaddr;
	unsigned int upper_bit;
	unsigned int lower_bit;
	struct dram_param_spec_tag param_spec;
	unsigned int (*clc_func)(char *, void *);
	unsigned int read_bit_value; /* internal used value */
	unsigned int clc_data;       /* internal used value */
};

unsigned int dram_timing_spec_checker(struct dram_param_spec_tag *pdram_param_spec,
														unsigned int clc_data) {
	int ret = 0;

	int ddr_timing_min_time = 0;

	switch (pdram_param_spec->spec_type) {
		case MIN_TYPE:
			if (pdram_param_spec->unit_type == NS) {
				ddr_timing_min_time = max(pdram_param_spec->min_100ps
									,(pdram_param_spec->min_tck * 1000 * 10 / DDR_FREQ));
				if (clc_data >= ddr_timing_min_time) {
					ret = 1;
				}
			} else if (pdram_param_spec->unit_type == TCK) {
				if (clc_data >= pdram_param_spec->min_tck) {
					ret = 1;
				}
			}
			break;
		case MAX_TYPE:
			if (pdram_param_spec->max_time >= clc_data) {
				ret = 1;
			}

			break;
	}


	return ret;

}

unsigned int dram_timing_default_clc(char *buf, void *param) {

	int buf_size = 128, ret=0, result = 0;
	struct dram_timing_tag *ptdram_timing = param;
	int ddr_timing_min_time = 0;

	char str_min_spec[10]={0,}, str_max_spec[10]={0, };
	char str_clc_min_value[10]={0,}, str_clc_max_value[10]={0, };
	char str_result[64]={0,};

	ptdram_timing->read_bit_value = get_phy_addr_bit_value(ptdram_timing->phyaddr
			,ptdram_timing->upper_bit, ptdram_timing->lower_bit);

	if (strstr(ptdram_timing->name, "tXSR") != NULL) {
		ptdram_timing->clc_data = (ptdram_timing->read_bit_value-1) * 32 * 1000000 / DDR_FREQ;
	} else if (strstr(ptdram_timing->name, "tRAS_MAX") != NULL) {
		ptdram_timing->clc_data = (ptdram_timing->read_bit_value) * 2 *  1024 * 1000000 / DDR_FREQ;
	} else {
		/*unit of clc_data is pico sec */
		ptdram_timing->clc_data = ptdram_timing->read_bit_value * 1000000 / DDR_FREQ;
	}

	result = dram_timing_spec_checker(&(ptdram_timing->param_spec), ptdram_timing->clc_data /100);

	/* SPEC string */
	if (ptdram_timing->param_spec.min_100ps == NA) {
		sprintf(str_min_spec, "N/A");
	} else {
		ddr_timing_min_time = max(ptdram_timing->param_spec.min_100ps
								,(ptdram_timing->param_spec.min_tck * 1000 * 10 / DDR_FREQ));
		sprintf(str_min_spec, "%d.%d",  ddr_timing_min_time/10, ddr_timing_min_time%10);
	}
	if (ptdram_timing->param_spec.max_time == NA) {
		sprintf(str_max_spec, "N/A");
	} else {
		sprintf(str_max_spec, "%d",  ptdram_timing->param_spec.max_time);
	}

	/* CLC string */
	if (ptdram_timing->param_spec.min_100ps == NA) {
		sprintf(str_clc_min_value, "N/A");
	} else {
		sprintf(str_clc_min_value, "%d.%d",  ptdram_timing->clc_data/1000, (ptdram_timing->clc_data%1000));
	}
	if (ptdram_timing->param_spec.max_time == NA) {
		sprintf(str_clc_max_value, "N/A");
	} else {
		sprintf(str_clc_max_value, "%d",  ptdram_timing->clc_data/1000);
	}

	/* result */
	if (result == 1) {
		sprintf(str_result, "   === >   [SPEC  IN]");
	} else {
		sprintf(str_result, "   === >   [SPEC OUT]");
	}

	ret +=  snprintf(buf + ret, buf_size, "%s %8s %8s %8s  %8s     %8s %8s %s \n"
		, ptdram_timing->name
		, minmax_string[ptdram_timing->param_spec.spec_type]
		, unit_string[ptdram_timing->param_spec.unit_type]
		, str_min_spec
		, str_max_spec
		, str_clc_min_value
		, str_clc_max_value
		, str_result);

	return ret;

}


unsigned int dram_timing_tccd_clc(char *buf, void *param) {

	int buf_size = 128, ret=0, result = 0;
	struct dram_timing_tag *ptdram_timing = param;

	char str_min_spec[10]={0,}, str_max_spec[10]={"N/A"};
	char str_clc_min_value[10]={0,}, str_clc_max_value[10]={"N/A"};
	char str_result[64]={0,};

	ptdram_timing->read_bit_value = get_phy_addr_bit_value(ptdram_timing->phyaddr
			,ptdram_timing->upper_bit, ptdram_timing->lower_bit);

	ptdram_timing->clc_data = ptdram_timing->read_bit_value;

	result = dram_timing_spec_checker(&(ptdram_timing->param_spec), ptdram_timing->clc_data);

	/* SPEC string */
	if (ptdram_timing->param_spec.min_tck == NA) {
		sprintf(str_min_spec, "N/A");
	} else {
		sprintf(str_min_spec, "%d",  ptdram_timing->param_spec.min_tck);
	}

	/* CLC string */
	if (ptdram_timing->param_spec.min_tck == NA) {
		sprintf(str_clc_min_value, "N/A");
	} else {
		sprintf(str_clc_min_value, "%d",  ptdram_timing->clc_data);
	}

	/* result */
	if (result == 1) {
		sprintf(str_result, "   === >   [SPEC  IN]");
	} else {
		sprintf(str_result, "   === >   [SPEC OUT]");
	}

	ret +=  snprintf(buf + ret, buf_size, "%s %8s %8s %8s  %8s     %8s %8s %s \n"
		, ptdram_timing->name
		, minmax_string[ptdram_timing->param_spec.spec_type]
		, unit_string[ptdram_timing->param_spec.unit_type]
		, str_min_spec
		, str_max_spec
		, str_clc_min_value
		, str_clc_max_value
		, str_result);

	return ret;

}

unsigned int dram_timing_twr_clc(char *buf, void *param) {

	int buf_size = 128, ret=0, result = 0;
	struct dram_timing_tag *ptdram_timing = param;
	int ddr_timing_min_time = 0;

	int mr2_nwre=0, twr_clk=0;
	char str_min_spec[10]={0,}, str_max_spec[10]={"N/A"};
	char str_clc_min_value[10]={0,}, str_clc_max_value[10]={"N/A"};
	char str_result[64]={0,};

	ptdram_timing->read_bit_value = get_phy_addr_bit_value(ptdram_timing->phyaddr
			,ptdram_timing->upper_bit, ptdram_timing->lower_bit);


	mr2_nwre = get_phy_addr_bit_value(0x30010000 +(0x29*4), 4, 4);  /* MR2 : nWRE */
	pr_info("mr2_nwre=%d\n", mr2_nwre);
	if (mr2_nwre == 0) {
		twr_clk = ptdram_timing->read_bit_value + 4; /* refer to MR1 */
	} else if (mr2_nwre == 1) {
		switch (ptdram_timing->read_bit_value) {
			case 0:
				twr_clk = 10;
				break;
			case 1:
				twr_clk = 11;
				break;
			case 2:
				twr_clk = 12;
				break;
			case 4:
				twr_clk = 14;
				break;
			case 6:
				twr_clk = 16;
				break;
		}
	}

	pr_info("mr2_nwre=%d ptdram_timing->read_bit_value=%d twr_clk=%d\n"
		, mr2_nwre, ptdram_timing->read_bit_value, twr_clk);
	ptdram_timing->clc_data = twr_clk * 1000000 / DDR_FREQ;

	result = dram_timing_spec_checker(&(ptdram_timing->param_spec), (ptdram_timing->clc_data/100));

	/* SPEC string */
	if (ptdram_timing->param_spec.min_100ps == NA) {
		sprintf(str_min_spec, "N/A");
	} else {
		ddr_timing_min_time = max(ptdram_timing->param_spec.min_100ps
								,(ptdram_timing->param_spec.min_tck * 1000 * 10 / DDR_FREQ));
		sprintf(str_min_spec, "%d.%d",  ddr_timing_min_time/10, ddr_timing_min_time%10);
	}
	if (ptdram_timing->param_spec.max_time == NA) {
		sprintf(str_max_spec, "N/A");
	} else {
		sprintf(str_max_spec, "%d",  ptdram_timing->param_spec.max_time);
	}

	/* CLC string */
	if (ptdram_timing->param_spec.min_100ps == NA) {
		sprintf(str_clc_min_value, "N/A");
	} else {
		sprintf(str_clc_min_value, "%d.%d",  ptdram_timing->clc_data/1000, (ptdram_timing->clc_data%1000));
	}
	if (ptdram_timing->param_spec.max_time == NA) {
		sprintf(str_clc_max_value, "N/A");
	} else {
		sprintf(str_clc_max_value, "%d",  ptdram_timing->clc_data/1000);
	}


	/* result */
	if (result == 1) {
		sprintf(str_result, "   === >   [SPEC  IN]");
	} else {
		sprintf(str_result, "   === >   [SPEC OUT]");
	}

	ret +=  snprintf(buf + ret, buf_size, "%s %8s %8s %8s  %8s     %8s %8s %s \n"
		, ptdram_timing->name
		, minmax_string[ptdram_timing->param_spec.spec_type]
		, unit_string[ptdram_timing->param_spec.unit_type]
		, str_min_spec
		, str_max_spec
		, str_clc_min_value
		, str_clc_max_value
		, str_result);

	return ret;

}

struct dram_timing_tag dram_timing[] = {
	{"    tCKE", 0x30000114, 3, 	0,	{MIN_TYPE, NS, 75, 3, NA}, dram_timing_default_clc},
	{"   tRCpb", 0x30000104, 5, 	0, 	{MIN_TYPE, NS, 600, 6, NA},	dram_timing_default_clc},
	{"  tCHESR", 0x30000114, 13, 	8, 	{MIN_TYPE, NS, 150, 3, NA}, dram_timing_default_clc},
	{"    tXSR", 0x30000120, 6, 	0, 	{MIN_TYPE, NS, tRFCAB+100, 2, NA}, dram_timing_default_clc},
	{"     tXP", 0x30000104, 20,	15, {MIN_TYPE, NS, 75, 3, NA}, dram_timing_default_clc},
	{"    tCCD", 0x30000110, 18,	16, {MIN_TYPE, TCK,	NA, 4, NA},	dram_timing_tccd_clc},
	{"    tRTP", 0x30000104, 12, 	8, 	{MIN_TYPE, NS, 75, 4, NA},	dram_timing_default_clc},
	{"    tRCD", 0x30000110, 28, 	24, {MIN_TYPE, NS,  180, 3, NA},	dram_timing_default_clc},
	{"   tRPpb", 0x30000110, 4, 	0, 	{MIN_TYPE, NS,	180, 3, NA},	dram_timing_default_clc},
	{"tRAS_MIN", 0x30000100, 5, 	0, 	{MIN_TYPE, NS, 	420, 3, NA}, dram_timing_default_clc},
	{"tRAS_MAX", 0x30000100, 14, 	8, 	{MAX_TYPE, NS, NA, NA, 70000}, dram_timing_default_clc},
	{"     tWR", 0x30010000+(0x28*4), 7, 	5,  {MIN_TYPE, NS, 	150, 5, NA},	dram_timing_twr_clc},
	{"    tWTR", 0x30010000+(0x23*4), 7, 	4, 	{MIN_TYPE, NS, 75, 4, NA}, dram_timing_default_clc},
	{"    tRRD", 0x30000110, 11, 	8, 	{MIN_TYPE, NS, 100, 2, NA}, 	dram_timing_default_clc},
	{"    tFAW", 0x30000100, 21, 	16, {MIN_TYPE, NS, 500, 8, NA},	dram_timing_default_clc},
	{"  tRFCab", 0x30000064, 8, 	0, 	{MIN_TYPE, NS, tRFCAB, 2, NA},	dram_timing_default_clc},
};


int dram_timing_read_sub(char *buf, int buf_size)
{
	int i=0, ret = 0;
	int dram_timing_array_size = 0;

	dram_timing_array_size = sizeof(dram_timing)/sizeof(struct dram_timing_tag);

	pr_info("dram_timing_array_size=%d\n", dram_timing_array_size);

	ret +=  snprintf(buf + ret, buf_size - ret
		, "==============================================" \
		"===============================================\n");

	ret +=  snprintf(buf + ret, buf_size - ret
		, "SYMBOL  PARAM    MIN/MAX   UNIT   MIN_SPEC   MAX_SPEC   MIN_CLC   MAX_CLC             RESULT\n");

	for (i=0; i<dram_timing_array_size; i++) {
		ret += snprintf(buf + ret, buf_size, "[%2d] ", i+1);
		ret += dram_timing[i].clc_func(buf+ret, &(dram_timing[i]));
	}

	return ret;
}


static ssize_t check_dram_timing_read(struct file *file,
	char __user *buffer, size_t count, loff_t *ppos)
{
	unsigned int size_for_copy;

	size_for_copy = wrapper_for_debug_fs(buffer, count, ppos, dram_timing_read_sub);

	return size_for_copy;
}

static const struct file_operations check_dram_timing_fops = {
	.owner = THIS_MODULE,
	.read = check_dram_timing_read,
};


void debugfs_dram(struct dentry *d)
{
	if (!debugfs_create_file("check_dram_timing", 0644
		, d, NULL,&check_dram_timing_fops))   \
			pr_err("%s : debugfs_create_file, error\n", "check_phy_addr");

}



