/*
 * Copyright (C) 2007 Casey Schaufler <casey@schaufler-ca.com>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, version 2.
 *
 * Author:
 *      Casey Schaufler <casey@schaufler-ca.com>
 *
 */

#ifndef _SECURITY_SMACK_H
#define _SECURITY_SMACK_H

#include <linux/capability.h>
#include <linux/spinlock.h>
#include <linux/security.h>
#include <linux/in.h>
#include <net/netlabel.h>
#include <linux/list.h>
#include <linux/rculist.h>
#include <linux/lsm_audit.h>

/*
 * Smack labels were limited to 23 characters for a long time.
 */
#define SMK_LABELLEN	24
#define SMK_LONGLABEL	256

/*
 * This is the repository for labels seen so that it is
 * not necessary to keep allocating tiny chuncks of memory
 * and so that they can be shared.
 *
 * Labels are never modified in place. Anytime a label
 * is imported (e.g. xattrset on a file) the list is checked
 * for it and it is added if it doesn't exist. The address
 * is passed out in either case. Entries are added, but
 * never deleted.
 *
 * Since labels are hanging around anyway it doesn't
 * hurt to maintain a secid for those awkward situations
 * where kernel components that ought to use LSM independent
 * interfaces don't. The secid should go away when all of
 * these components have been repaired.
 *
 * The cipso value associated with the label gets stored here, too.
 *
 * Keep the access rules for this subject label here so that
 * the entire set of rules does not need to be examined every
 * time.
 */
struct smack_known {
	struct list_head		list;
	struct hlist_node		smk_hashed;
	char				*smk_known;
	u32				smk_secid;
	struct netlbl_lsm_secattr	smk_netlabel;	/* on wire labels */
	struct list_head		smk_rules;	/* access rules */
	struct mutex			smk_rules_lock;	/* lock for rules */
};

/*
 * Maximum number of bytes for the levels in a CIPSO IP option.
 * Why 23? CIPSO is constrained to 30, so a 32 byte buffer is
 * bigger than can be used, and 24 is the next lower multiple
 * of 8, and there are too many issues if there isn't space set
 * aside for the terminating null byte.
 */
#define SMK_CIPSOLEN	24

struct superblock_smack {
	char		*smk_root;
	char		*smk_floor;
	char		*smk_hat;
	char		*smk_default;
	int		smk_initialized;
};

struct socket_smack {
	struct smack_known	*smk_out;	/* outbound label */
	struct smack_known	*smk_in;	/* inbound label */
	struct smack_known	*smk_packet;	/* TCP peer label */
};

/*
 * Inode smack data
 */
struct inode_smack {
	char			*smk_inode;	/* label of the fso */
	struct smack_known	*smk_task;	/* label of the task */
	struct smack_known	*smk_mmap;	/* label of the mmap domain */
	struct mutex		smk_lock;	/* initialization lock */
	int			smk_flags;	/* smack inode flags */
};

struct task_smack {
	struct smack_known	*smk_task;	/* label for access control */
	struct smack_known	*smk_forked;	/* label when forked */
	struct list_head	smk_rules;	/* per task access rules */
	struct mutex		smk_rules_lock;	/* lock for the rules */
	struct list_head	smk_relabel;	/* transit allowed labels */
};

#define	SMK_INODE_INSTANT	0x01	/* inode is instantiated */
#define	SMK_INODE_TRANSMUTE	0x02	/* directory is transmuting */
#define	SMK_INODE_CHANGED	0x04	/* smack was transmuted */
#define	SMK_INODE_IMPURE	0x08	/* involved in an impure transaction */

/*
 * A label access rule.
 */
struct smack_rule {
	struct list_head	list;
	struct smack_known	*smk_subject;
	char			*smk_object;
	int			smk_access;
};

/*
 * An entry in the table identifying hosts.
 */
struct smk_netlbladdr {
	struct list_head	list;
	struct sockaddr_in	smk_host;	/* network address */
	struct in_addr		smk_mask;	/* network mask */
	char			*smk_label;	/* label */
};

/*
 * An entry in the table identifying ports.
 */
struct smk_port_label {
	struct list_head	list;
	struct sock		*smk_sock;	/* socket initialized on */
	unsigned short		smk_port;	/* the port number */
	struct smack_known	*smk_in;	/* inbound label */
	struct smack_known	*smk_out;	/* outgoing label */
};

struct smack_known_list_elem {
	struct list_head	list;
	struct smack_known	*smk_label;
};

/*
 * Mount options
 */
#define SMK_FSDEFAULT	"smackfsdef="
#define SMK_FSFLOOR	"smackfsfloor="
#define SMK_FSHAT	"smackfshat="
#define SMK_FSROOT	"smackfsroot="
#define SMK_FSTRANS	"smackfstransmute="

#define SMACK_CIPSO_OPTION 	"-CIPSO"

/*
 * How communications on this socket are treated.
 * Usually it's determined by the underlying netlabel code
 * but there are certain cases, including single label hosts
 * and potentially single label interfaces for which the
 * treatment can not be known in advance.
 *
 * The possibility of additional labeling schemes being
 * introduced in the future exists as well.
 */
#define SMACK_UNLABELED_SOCKET	0
#define SMACK_CIPSO_SOCKET	1

/*
 * CIPSO defaults.
 */
#define SMACK_CIPSO_DOI_DEFAULT		3	/* Historical */
#define SMACK_CIPSO_DOI_INVALID		-1	/* Not a DOI */
#define SMACK_CIPSO_DIRECT_DEFAULT	250	/* Arbitrary */
#define SMACK_CIPSO_MAPPED_DEFAULT	251	/* Also arbitrary */
#define SMACK_CIPSO_MAXLEVEL            255     /* CIPSO 2.2 standard */
/*
 * CIPSO 2.2 standard is 239, but Smack wants to use the
 * categories in a structured way that limits the value to
 * the bits in 23 bytes, hence the unusual number.
 */
#define SMACK_CIPSO_MAXCATNUM           184     /* 23 * 8 */

/*
 * Ptrace rules
 */
#define SMACK_PTRACE_DEFAULT	0
#define SMACK_PTRACE_EXACT	1
#define SMACK_PTRACE_DRACONIAN	2
#define SMACK_PTRACE_MAX	SMACK_PTRACE_DRACONIAN

/*
 * Flags for untraditional access modes.
 * It shouldn't be necessary to avoid conflicts with definitions
 * in fs.h, but do so anyway.
 */
#define MAY_TRANSMUTE	0x00001000	/* Controls directory labeling */
#define MAY_LOCK	0x00002000	/* Locks should be writes, but ... */
#define MAY_BRINGUP	0x00004000	/* Report use of this rule */

#define SMACK_BRINGUP_ALLOW		1	/* Allow bringup mode */
#define SMACK_UNCONFINED_SUBJECT	2	/* Allow unconfined label */
#define SMACK_UNCONFINED_OBJECT		3	/* Allow unconfined label */

/*
 * Just to make the common cases easier to deal with
 */
#define MAY_ANYREAD	(MAY_READ | MAY_EXEC)
#define MAY_READWRITE	(MAY_READ | MAY_WRITE)
#define MAY_NOT		0

/*
 * Number of access types used by Smack (rwxatlb)
 */
#define SMK_NUM_ACCESS_TYPE 7

/* SMACK data */
struct smack_audit_data {
	const char *function;
	char *subject;
	char *object;
	char *request;
	int result;
};

/*
 * Smack audit data; is empty if CONFIG_AUDIT not set
 * to save some stack
 */
struct smk_audit_info {
#ifdef CONFIG_AUDIT
	struct common_audit_data a;
	struct smack_audit_data sad;
#endif
};

struct smack_master_list {
	struct list_head	list;
	struct smack_rule	*smk_rule;
};

/*
 * These functions are in smack_lsm.c
 */
struct inode_smack *new_inode_smack(char *);

/*
 * These functions are in smack_access.c
 */
int smk_access_entry(char *, char *, struct list_head *);
int smk_access(struct smack_known *, char *, int, struct smk_audit_info *);
int smk_tskacc(struct task_smack *, char *, u32, struct smk_audit_info *);
int smk_curacc(char *, u32, struct smk_audit_info *);
struct smack_known *smack_from_secid(const u32);
char *smk_parse_smack(const char *string, int len);
int smk_netlbl_mls(int, char *, struct netlbl_lsm_secattr *, int);
char *smk_import(const char *, int);
struct smack_known *smk_import_entry(const char *, int);
void smk_insert_entry(struct smack_known *skp);
struct smack_known *smk_find_entry(const char *);
int smack_privileged(int cap);
u32 smack_to_secid(const char *);
void smk_destroy_label_list(struct list_head *list);

/*
 * Shared data.
 */
extern int smack_enabled;
extern int smack_cipso_direct;
extern int smack_cipso_mapped;
extern struct smack_known *smack_net_ambient;
extern struct smack_known *smack_syslog_label;
#ifdef CONFIG_SECURITY_SMACK_BRINGUP
extern struct smack_known *smack_unconfined;
#endif
extern const char *smack_cipso_option;
extern int smack_ptrace_rule;

extern struct smack_known smack_known_floor;
extern struct smack_known smack_known_hat;
extern struct smack_known smack_known_huh;
extern struct smack_known smack_known_invalid;
extern struct smack_known smack_known_star;
extern struct smack_known smack_known_web;

extern struct mutex	smack_known_lock;
extern struct list_head smack_known_list;
extern struct list_head smk_netlbladdr_list;

extern struct mutex     smack_onlycap_lock;
extern struct list_head smack_onlycap_list;

/* Cache for fast and thrifty allocations */
extern struct kmem_cache *smack_rule_cache;
extern struct kmem_cache *smack_master_list_cache;

extern struct security_operations smack_ops;

#define SMACK_HASH_SLOTS 16
extern struct hlist_head smack_known_hash[SMACK_HASH_SLOTS];

/*
 * Is the directory transmuting?
 */
static inline int smk_inode_transmutable(const struct inode *isp)
{
	struct inode_smack *sip = isp->i_security;
	return (sip->smk_flags & SMK_INODE_TRANSMUTE) != 0;
}

/*
 * Present a pointer to the smack label in an inode blob.
 */
static inline char *smk_of_inode(const struct inode *isp)
{
	struct inode_smack *sip = isp->i_security;
	return sip->smk_inode;
}

/*
 * Present a pointer to the smack label entry in an task blob.
 */
static inline struct smack_known *smk_of_task(const struct task_smack *tsp)
{
	return tsp->smk_task;
}

/*
 * Present a pointer to the forked smack label entry in an task blob.
 */
static inline struct smack_known *smk_of_forked(const struct task_smack *tsp)
{
	return tsp->smk_forked;
}

/*
 * Present a pointer to the smack label in the current task blob.
 */
static inline struct smack_known *smk_of_current(void)
{
	return smk_of_task(current_security());
}

/*
 * logging functions
 */
#define SMACK_AUDIT_DENIED 0x1
#define SMACK_AUDIT_ACCEPT 0x2
extern int log_policy;

void smack_log(char *subject_label, char *object_label,
		int request,
		int result, struct smk_audit_info *auditdata);

#ifdef CONFIG_AUDIT

/*
 * some inline functions to set up audit data
 * they do nothing if CONFIG_AUDIT is not set
 *
 */
static inline void smk_ad_init(struct smk_audit_info *a, const char *func,
			       char type)
{
	memset(&a->sad, 0, sizeof(a->sad));
	a->a.type = type;
	a->a.smack_audit_data = &a->sad;
	a->a.smack_audit_data->function = func;
}

static inline void smk_ad_init_net(struct smk_audit_info *a, const char *func,
				   char type, struct lsm_network_audit *net)
{
	smk_ad_init(a, func, type);
	memset(net, 0, sizeof(*net));
	a->a.u.net = net;
}

static inline void smk_ad_setfield_u_tsk(struct smk_audit_info *a,
					 struct task_struct *t)
{
	a->a.u.tsk = t;
}
static inline void smk_ad_setfield_u_fs_path_dentry(struct smk_audit_info *a,
						    struct dentry *d)
{
	a->a.u.dentry = d;
}
static inline void smk_ad_setfield_u_fs_inode(struct smk_audit_info *a,
					      struct inode *i)
{
	a->a.u.inode = i;
}
static inline void smk_ad_setfield_u_fs_path(struct smk_audit_info *a,
					     struct path p)
{
	a->a.u.path = p;
}
static inline void smk_ad_setfield_u_net_sk(struct smk_audit_info *a,
					    struct sock *sk)
{
	a->a.u.net->sk = sk;
}

#else /* no AUDIT */

static inline void smk_ad_init(struct smk_audit_info *a, const char *func,
			       char type)
{
}
static inline void smk_ad_setfield_u_tsk(struct smk_audit_info *a,
					 struct task_struct *t)
{
}
static inline void smk_ad_setfield_u_fs_path_dentry(struct smk_audit_info *a,
						    struct dentry *d)
{
}
static inline void smk_ad_setfield_u_fs_path_mnt(struct smk_audit_info *a,
						 struct vfsmount *m)
{
}
static inline void smk_ad_setfield_u_fs_inode(struct smk_audit_info *a,
					      struct inode *i)
{
}
static inline void smk_ad_setfield_u_fs_path(struct smk_audit_info *a,
					     struct path p)
{
}
static inline void smk_ad_setfield_u_net_sk(struct smk_audit_info *a,
					    struct sock *sk)
{
}
#endif

#endif  /* _SECURITY_SMACK_H */
