/****************************************************************************\
 *  slurm_protocol_defs.h - definitions used for RPCs
 *****************************************************************************
 *  Copyright (C) 2002 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Kevin Tew <tew1@llnl.gov>.
 *  UCRL-CODE-2002-040.
 *  
 *  This file is part of SLURM, a resource management program.
 *  For details, see <http://www.llnl.gov/linux/slurm/>.
 *  
 *  SLURM is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *  
 *  SLURM is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with SLURM; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
\*****************************************************************************/

#ifndef _SLURM_PROTOCOL_DEFS_H
#define _SLURM_PROTOCOL_DEFS_H

#if HAVE_CONFIG_H
#  include <config.h>
#  if HAVE_INTTYPES_H
#    include <inttypes.h>
#  else
#    if HAVE_STDINT_H
#      include <stdint.h>
#    endif
#  endif			/* HAVE_INTTYPES_H */
#  ifdef HAVE_LIBELAN3
#    include <src/common/qsw.h>
#  endif
#else				/* !HAVE_CONFIG_H */
#  include <inttypes.h>
#endif				/*  HAVE_CONFIG_H */

#include <src/common/macros.h>
#include <src/common/xassert.h>
#include <src/common/slurm_protocol_common.h>


/* used to define the size of the credential.signature size
 * used to define the key size of the io_stream_header_t
 */
#define SLURM_SSL_SIGNATURE_LENGTH 128

/* used to define the type of the io_stream_header_t.type
 */
#define SLURM_IO_STREAM_INOUT 0
#define SLURM_IO_STREAM_SIGERR 1


/* INFINITE is used to identify unlimited configurations,  */
/* eg. the maximum count of nodes any job may use in some partition */
#define	INFINITE (0xffffffff)
#define NO_VAL	 (0xfffffffe)

/* last entry must be STATE_END, keep in sync with node_state_string    		*/
/* if a node ceases to respond, its last state is ORed with NODE_STATE_NO_RESPOND	*/
enum node_states {
	NODE_STATE_DOWN,	/* node is not responding */
	NODE_STATE_UNKNOWN,	/* node's initial state, unknown */
	NODE_STATE_IDLE,	/* node idle and available for use */
	NODE_STATE_ALLOCATED,	/* node has been allocated, job not currently running */
	NODE_STATE_DRAINED,	/* node idle and not to be allocated future work */
	NODE_STATE_DRAINING,	/* node in use, but not to be allocated future work */
	NODE_STATE_END		/* last entry in table */
};
#define NODE_STATE_NO_RESPOND (0x8000)

enum task_dist_states {
	SLURM_DIST_CYCLIC,	/* distribute tasks one per node, round robin */
	SLURM_DIST_BLOCK	/* distribute tasks filling node by node */
};

/* last entry must be JOB_END
 * NOTE: keep in sync with job_state_string and job_state_string_compact */
enum job_states {
	JOB_PENDING,		/* queued waiting for initiation */
	JOB_RUNNING,		/* allocated resources and executing */
	JOB_COMPLETE,		/* completed execution successfully, nodes released */
	JOB_FAILED,		/* completed execution unsuccessfully, nodes released */
	JOB_TIMEOUT,		/* terminated on reaching time limit, nodes released */
	JOB_NODE_FAIL,		/* terminated on node failure, nodes released */
	JOB_END			/* last entry in table */
};

enum part_shared {
	SHARED_NO,		/* Nodes never shared in partition */
	SHARED_YES,		/* Nodes possible to share in partition */
	SHARED_FORCE		/* Nodes always shares in partition */
};
/* SLURM Message types */
typedef enum {
	REQUEST_NODE_REGISTRATION_STATUS = 1001,
	MESSAGE_NODE_REGISTRATION_STATUS,
	REQUEST_RECONFIGURE,
	RESPONSE_RECONFIGURE,
	REQUEST_SHUTDOWN,
	REQUEST_SHUTDOWN_IMMEDIATE,
	RESPONSE_SHUTDOWN,
	REQUEST_PING,

	REQUEST_BUILD_INFO = 2001,
	RESPONSE_BUILD_INFO,
	REQUEST_JOB_INFO,
	RESPONSE_JOB_INFO,
	REQUEST_JOB_STEP_INFO,
	RESPONSE_JOB_STEP_INFO,
	REQUEST_NODE_INFO,
	RESPONSE_NODE_INFO,
	REQUEST_PARTITION_INFO,
	RESPONSE_PARTITION_INFO,
	REQUEST_ACCTING_INFO,
	RESPONSE_ACCOUNTING_INFO,

	REQUEST_UPDATE_JOB = 3001,
	REQUEST_UPDATE_NODE,
	REQUEST_UPDATE_PARTITION,

	REQUEST_RESOURCE_ALLOCATION = 4001,
	RESPONSE_RESOURCE_ALLOCATION,
	REQUEST_SUBMIT_BATCH_JOB,
	RESPONSE_SUBMIT_BATCH_JOB,
	REQUEST_BATCH_JOB_LAUNCH,
	RESPONSE_BATCH_JOB_LAUNCH,
	REQUEST_SIGNAL_JOB,
	RESPONSE_SIGNAL_JOB,
	REQUEST_CANCEL_JOB,
	RESPONSE_CANCEL_JOB,
	REQUEST_JOB_RESOURCE,
	RESPONSE_JOB_RESOURCE,
	REQUEST_JOB_ATTACH,
	RESPONSE_JOB_ATTACH,
	REQUEST_IMMEDIATE_RESOURCE_ALLOCATION,
	RESPONSE_IMMEDIATE_RESOURCE_ALLOCATION,
	REQUEST_JOB_WILL_RUN,
	RESPONSE_JOB_WILL_RUN,
	REQUEST_REVOKE_JOB_CREDENTIAL,
	REQUEST_ALLOCATION_AND_RUN_JOB_STEP,
	RESPONSE_ALLOCATION_AND_RUN_JOB_STEP,
	REQUEST_OLD_JOB_RESOURCE_ALLOCATION,

	REQUEST_JOB_STEP_CREATE = 5001,
	RESPONSE_JOB_STEP_CREATE,
	REQUEST_RUN_JOB_STEP,
	RESPONSE_RUN_JOB_STEP,
	REQUEST_SIGNAL_JOB_STEP,
	RESPONSE_SIGNAL_JOB_STEP,
	REQUEST_CANCEL_JOB_STEP,
	RESPONSE_CANCEL_JOB_STEP,
	REQUEST_COMPLETE_JOB_STEP,
	RESPONSE_COMPLETE_JOB_STEP,

	REQUEST_LAUNCH_TASKS = 6001,
	RESPONSE_LAUNCH_TASKS,
	MESSAGE_TASK_EXIT,
	REQUEST_KILL_TASKS,
	REQUEST_REATTACH_TASKS_STREAMS,
	RESPONSE_REATTACH_TASKS_STREAMS,

	RESPONSE_SLURM_RC = 8001,
	MESSAGE_UPLOAD_ACCOUNTING_INFO,
} slurm_msg_type_t;

typedef enum {
	CREDENTIAL1
} slurm_credential_type_t;

/******************************************************************************
 * core api configuration struct 
 ******************************************************************************/
typedef struct slurm_protocol_config {
	slurm_addr primary_controller;
	slurm_addr secondary_controller;
} slurm_protocol_config_t;

/*core api protocol message structures */
typedef struct slurm_protocol_header {
	uint16_t version;
	uint16_t flags;
	slurm_credential_type_t cred_type;
	uint32_t cred_length;
	slurm_msg_type_t msg_type;
	uint32_t body_length;
} header_t;

typedef struct slurm_io_stream_header {
	uint16_t version;	/*version/magic number */
	char key[SLURM_SSL_SIGNATURE_LENGTH];
	uint32_t task_id;
	uint16_t type;
} slurm_io_stream_header_t;

typedef struct slurm_msg {
	slurm_msg_type_t msg_type;
	slurm_addr address;
	slurm_fd conn_fd;
	slurm_credential_type_t cred_type;
	void *cred;
	uint32_t cred_size;
	void *data;
	uint32_t data_size;
} slurm_msg_t;


/*****************************************************************************
 * Slurm Protocol Data Structures
 *****************************************************************************/
typedef struct {
	uint32_t job_id;
	uid_t user_id;
	char *node_list;
	time_t expiration_time;
	char signature[SLURM_SSL_SIGNATURE_LENGTH];	/* What are we going to do here? */
} slurm_job_credential_t;

typedef struct {
	uint32_t job_id;	/* job ID */
	uint16_t step_id;	/* step ID */
	uint32_t user_id;	/* user the job runs as */
	time_t start_time;	/* step start time */
	char *partition;	/* name of assigned partition */
	char *nodes;		/* comma delimited list of nodes allocated to job_step */
} job_step_info_t;

typedef struct job_step_specs {
	uint32_t job_id;
	uint32_t user_id;
	uint32_t node_count;
	uint32_t cpu_count;
	uint16_t relative;
	uint16_t task_dist;	/* see task_dist_states for values */
	char *node_list;
} job_step_specs_t;

typedef struct job_descriptor {	/* Job descriptor for submit, allocate, and update requests */
	uint16_t contiguous;	/* 1 if job requires contiguous nodes, 0 otherwise,
				 * default=0 */
	uint16_t kill_on_node_fail; /* 1 if node failure to kill job, 0 otherwise,
				 * default=1 */
	char **environment;	/* environment variables to set for job, 
				   *   name=value pairs, one per line */
	uint16_t env_size;	/* element count in environment */
	char *features;		/* comma separated list of required features, default NONE */
	uint32_t job_id;	/* job ID, default set by SLURM */
	char *name;		/* name of the job, default "" */
	uint32_t min_procs;	/* minimum processors required per node, default=0 */
	uint32_t min_memory;	/* minimum real memory required per node, default=0 */
	uint32_t min_tmp_disk;	/* minimum temporary disk required per node, default=0 */
	char *partition;	/* name of requested partition, default in SLURM config */
	uint32_t priority;	/* relative priority of the job, default set by SLURM,
				 * can only be explicitly set if user is root, maximum
				 *                                  * value is #fffffffe */
	char *req_nodes;	/* comma separated list of required nodes, default NONE */
	uint16_t shared;	/* 1 if job can share nodes with other jobs, 0 otherwise,
				 * default in SLURM configuration */
	uint32_t time_limit;	/* maximum run time in minutes, default is partition
				 * limit as defined in SLURM configuration, maximum
				 *                                  * value is #fffffffe */
	uint32_t num_procs;	/* number of processors required by job, default=0 */
	uint32_t num_nodes;	/* number of nodes required by job, default=0 */
	char *script;		/* the actual job script, default NONE */
	char *err;		/* pathname of stderr */
	char *in;		/* pathname of stdin */
	char *out;		/* pathname of stdout */
	uint32_t user_id;	/* set only if different from current UID, default set
				 * to UID by API, can only be set if user is root */
	char *work_dir;		/* fully qualified pathname of working directory */
} job_descriptor_t;

typedef struct job_step_id {
	uint32_t last_update;
	uint32_t job_id;
	uint32_t job_step_id;
} job_step_id_t;

typedef struct job_info {
	uint32_t job_id;	/* job ID */
	char *name;		/* name of the job */
	uint32_t user_id;	/* user the job runs as */
	uint16_t job_state;	/* state of the job, see enum job_states */
	uint32_t time_limit;	/* maximum run time in minutes or INFINITE */
	time_t start_time;	/* time execution begins, actual or expected */
	time_t end_time;	/* time of termination, actual or expected */
	uint32_t priority;	/* relative priority of the job */
	char *nodes;		/* comma delimited list of nodes allocated to job */
	int *node_inx;		/* list index pairs into node_table for *nodes:
				   start_range_1, end_range_1, start_range_2, .., -1  */
	char *partition;	/* name of assigned partition */
	uint32_t num_procs;	/* number of processors required by job */
	uint32_t num_nodes;	/* number of nodes required by job */
	uint16_t shared;	/* 1 if job can share nodes with other jobs */
	uint16_t contiguous;	/* 1 if job requires contiguous nodes */
	uint32_t min_procs;	/* minimum processors required per node */
	uint32_t min_memory;	/* minimum real memory required per node */
	uint32_t min_tmp_disk;	/* minimum temporary disk required per node */
	char *req_nodes;	/* comma separated list of required nodes */
	int *req_node_inx;	/* list index pairs into node_table for *req_nodes:
				   start_range_1, end_range_1, start_range_2, .., -1  */
	char *features;		/* comma separated list of required features */
} job_info_t;

typedef struct node_info {
	char *name;		/* node name */
	uint16_t node_state;	/* see node_state_string below for translation */
	uint32_t cpus;		/* configured count of cpus running on the node */
	uint32_t real_memory;	/* configured megabytes of real memory on the node */
	uint32_t tmp_disk;	/* configured megabytes of total disk in TMP_FS */
	uint32_t weight;	/* arbitrary priority of node for scheduling work on */
	char *features;		/* arbitrary list of features associated with a node */
	char *partition;	/* name of partition node configured to */
} node_info_t;

typedef struct partition_info {
	char *name;		/* name of the partition */
	uint32_t max_time;	/* minutes or INFINITE */
	uint32_t max_nodes;	/* per job or INFINITE */
	uint32_t total_nodes;	/* total number of nodes in the partition */
	uint32_t total_cpus;	/* total number of cpus in the partition */
	uint16_t default_part;	/* 1 if this is default partition */
	uint16_t root_only;	/* 1 if allocate/submit RPC must come for user root */
	uint16_t shared;	/* 1 if job can share nodes, 2 if job must share nodes */
	uint16_t state_up;	/* 1 if state is up, 0 if down */
	char *nodes;		/* comma delimited list names of nodes in partition */
	int *node_inx;		/* list index pairs into node_table:
				   start_range_1, end_range_1, start_range_2, .., -1  */
	char *allow_groups;	/* comma delimited list of groups, null indicates all */
} partition_info_t;

/*****************************************************************************
 * Slurm API Protocol Data Structures
 *****************************************************************************/

struct slurm_ctl_conf {
	uint32_t last_update;	/* last update time of the build parameters */
	char *backup_addr;	/* comm path of slurmctld secondary server */
	char *backup_controller;	/* name of slurmctld secondary server */
	char *control_addr;	/* comm path of slurmctld primary server */
	char *control_machine;	/* name of slurmctld primary server */
	char *epilog;		/* pathname of job epilog */
	uint32_t first_job_id;	/* first slurm generated job_id to assign */
	uint16_t fast_schedule;	/* 1 to *not* check configurations by node 
				 * (only check configuration file, faster) */
	uint16_t hash_base;	/* base used for hashing node table */
	uint16_t heartbeat_interval;	/* interval between node heartbeats, seconds */
	uint16_t inactive_limit;	/* seconds of inactivity before an idle job is killed */
	uint16_t kill_wait;	/* seconds from SIGXCPU to SIGKILL on job termination */
	char *prioritize;	/* pathname of program to set initial job priority */
	char *prolog;		/* pathname of job prolog */
	uint16_t ret2service;	/* 1 return node to service at registration */ 
	uint32_t slurmctld_port;	/* default communications port to slurmctld */
	uint16_t slurmctld_timeout;	/* how long backup waits for primarly slurmctld */
	uint32_t slurmd_port;	/* default communications port to slurmd */
	uint16_t slurmd_timeout;	/* how long slurmctld waits for slurmd before setting down */
	char *slurm_conf;	/* pathname of slurm config file */
	char *state_save_location;	/* pathname of state save directory */
	char *tmp_fs;		/* pathname of temporary file system */
	char *job_credential_private_key;	/* path to private key */
	char *job_credential_public_certificate;	/* path to public certificate */
};

typedef struct slurm_ctl_conf slurm_ctl_conf_t;

/****************************************************************************
 * Slurm Protocol Message Types
 ****************************************************************************/
typedef struct job_descriptor job_desc_msg_t;

typedef struct job_id_msg {
	uint32_t job_id;
} job_id_msg_t;

typedef struct job_step_create_response_msg {
	uint32_t job_step_id;
	char *node_list;
	slurm_job_credential_t *credentials;
#ifdef HAVE_LIBELAN3
	qsw_jobinfo_t qsw_job;	/* Elan3 switch context, opaque data structure */
#endif

} job_step_create_response_msg_t;

typedef struct job_step_specs job_step_create_request_msg_t;

typedef struct job_step_info_response_msg {
	uint32_t last_update;
	uint32_t job_step_count;
	job_step_info_t *job_steps;
} job_step_info_response_msg_t;

typedef struct job_step_id job_step_id_msg_t;
typedef struct job_step_id job_info_request_msg_t;

typedef struct job_step_info_request_msg {
	uint32_t last_update;
	uint32_t job_id;
	uint32_t step_id;
} job_step_info_request_msg_t;

typedef struct job_info_msg {
	uint32_t last_update;
	uint32_t record_count;
	job_info_t *job_array;
} job_info_msg_t;


typedef struct kill_tasks_msg {
	uint32_t job_id;
	uint32_t job_step_id;
	uint32_t signal;
} kill_tasks_msg_t;

typedef struct shutdown_msg {
	uint16_t core;
} shutdown_msg_t;

typedef struct last_update_msg {
	uint32_t last_update;
} last_update_msg_t;

typedef struct launch_tasks_request_msg {
	uint32_t job_id;
	uint32_t job_step_id;
	uint32_t nnodes;	/* number of nodes in this job step       */
	uint32_t nprocs;	/* number of processes in this job step   */
	uint32_t uid;
	uint32_t srun_node_id;	/* node id of this node (relative to job) */

	slurm_job_credential_t *credential;	/* job credential            */
	uint32_t tasks_to_launch;
	uint16_t envc;
	char **env;
	char *cwd;
	uint16_t argc;
	char **argv;
	slurm_addr response_addr;
	slurm_addr streams;
	uint32_t *global_task_ids;
#ifdef HAVE_LIBELAN3
	qsw_jobinfo_t qsw_job;	/* Elan3 switch context, opaque data structure */
#endif
} launch_tasks_request_msg_t;

typedef struct launch_tasks_response_msg {
	uint32_t return_code;
	char *node_name;
	uint32_t srun_node_id;
} launch_tasks_response_msg_t;

typedef struct task_ext_msg {
	uint32_t task_id;
	uint32_t return_code;
} task_exit_msg_t;

typedef struct node_info_msg {
	uint32_t last_update;
	uint32_t record_count;
	node_info_t *node_array;
} node_info_msg_t;

typedef struct partition_info_msg {
	uint32_t last_update;
	uint32_t record_count;
	partition_info_t *partition_array;
} partition_info_msg_t;


typedef struct partition_info partition_desc_msg_t;
typedef struct partition_info update_part_msg_t;

typedef struct return_code_msg {
	int32_t return_code;
} return_code_msg_t;

typedef struct revoke_credential_msg {
	uint32_t job_id;
	time_t expiration_time;
	char signature[SLURM_SSL_SIGNATURE_LENGTH];
} revoke_credential_msg_t;

typedef struct reattach_tasks_streams_msg {
	uint32_t job_id;
	uint32_t job_step_id;
	uint32_t uid;
	slurm_job_credential_t *credential;
	uint32_t tasks_to_reattach;
	slurm_addr streams;
	uint32_t *global_task_ids;
} reattach_tasks_streams_msg_t;

typedef struct old_job_alloc_msg {
	uint32_t job_id;
	uint32_t uid;
} old_job_alloc_msg_t;

typedef struct resource_allocation_response_msg {
	uint32_t job_id;
	char *node_list;
	int16_t num_cpu_groups;
	int32_t *cpus_per_node;
	int32_t *cpu_count_reps;
	uint16_t node_cnt;
	slurm_addr *node_addr;	/* network addresses */
} resource_allocation_response_msg_t;

typedef struct resource_allocation_and_run_response_msg {
	uint32_t job_id;
	char *node_list;
	int16_t num_cpu_groups;
	int32_t *cpus_per_node;
	int32_t *cpu_count_reps;
	uint32_t job_step_id;
	uint16_t node_cnt;
	slurm_addr *node_addr;	/* network addresses */
	slurm_job_credential_t *credentials;
#ifdef HAVE_LIBELAN3
	qsw_jobinfo_t qsw_job;	/* Elan3 switch context, opaque data structure */
#endif
} resource_allocation_and_run_response_msg_t;

typedef struct submit_response_msg {
	uint32_t job_id;
} submit_response_msg_t;

typedef struct batch_job_launch_msg {
	uint32_t job_id;
	uint32_t user_id;
	char *nodes;		/* comma delimited list of nodes allocated to job_step */
	char *script;		/* the actual job script, default NONE */
	char *err;		/* pathname of stderr */
	char *in;		/* pathname of stdin */
	char *out;		/* pathname of stdout */
	char *work_dir;		/* fully qualified pathname of working directory */
	uint16_t argc;
	char **argv;
	uint16_t env_size;	/* element count in environment */
	char **environment;	/* environment variables to set for job, 
				   *   name=value pairs, one per line */
} batch_job_launch_msg_t;

/****************************************************************************
 * Slurm API Message Types
 ****************************************************************************/
typedef struct slurm_update_node_msg {
	char *node_names;
	uint16_t node_state;
} update_node_msg_t;

typedef struct slurm_node_registration_status_msg {
	uint32_t timestamp;
	char *node_name;
	uint32_t cpus;
	uint32_t real_memory_size;
	uint32_t temporary_disk_space;
	uint32_t job_count;	/* number of associate job_id's */
	uint32_t *job_id;	/* IDs of running job (if any) */
	uint16_t *step_id;	/* IDs of running job steps (if any) */
} slurm_node_registration_status_msg_t;

typedef struct slurm_ctl_conf slurm_ctl_conf_info_msg_t;


/* the following typedefs follow kevin's communication message naming comvention */

/* free message functions */
void inline slurm_free_last_update_msg(last_update_msg_t * msg);
void inline slurm_free_return_code_msg(return_code_msg_t * msg);
void inline slurm_free_job_step_id(job_step_id_t * msg);

#define slurm_free_job_step_id_msg(msg) slurm_free_job_step_id((job_step_id_t*)(msg))
#define slurm_free_job_step_info_request_msg(msg) slurm_free_job_step_id(msg)
#define slurm_free_job_info_request_msg(msg) slurm_free_job_step_id(msg)

void inline slurm_free_ctl_conf(slurm_ctl_conf_info_msg_t * build_ptr);
void inline slurm_free_shutdown_msg (shutdown_msg_t * msg);

void inline slurm_free_job_desc_msg(job_desc_msg_t * msg);
void inline
slurm_free_resource_allocation_response_msg
(resource_allocation_response_msg_t * msg);
void inline
slurm_free_resource_allocation_and_run_response_msg
(resource_allocation_and_run_response_msg_t * msg);
void inline slurm_free_submit_response_response_msg(submit_response_msg_t *
						    msg);

void inline
slurm_free_node_registration_status_msg (slurm_node_registration_status_msg_t * msg);

void inline slurm_free_job_info_msg(job_info_msg_t * msg);
void inline slurm_free_job_info(job_info_t * job);
void inline slurm_free_job_info_members(job_info_t * job);

void inline slurm_free_job_step_info(job_step_info_t * msg);
void inline slurm_free_job_step_info_memebers(job_step_info_t * msg);
void inline
slurm_free_job_step_info_response_msg(job_step_info_response_msg_t * msg);

void inline slurm_free_job_launch_msg(batch_job_launch_msg_t * msg);

void inline slurm_free_partition_info_msg(partition_info_msg_t * msg);
void inline slurm_free_partition_info(partition_info_t * part);
void inline slurm_free_partition_info_members(partition_info_t * part);

void inline slurm_free_node_info_msg(node_info_msg_t * msg);
void inline slurm_free_node_info(node_info_t * node);
void inline slurm_free_node_info_members(node_info_t * node);

void inline slurm_free_update_node_msg(update_node_msg_t * msg);
void inline slurm_free_update_part_msg(update_part_msg_t * msg);
void inline
slurm_free_job_step_create_request_msg(job_step_create_request_msg_t *
				       msg);
void inline
slurm_free_job_step_create_response_msg(job_step_create_response_msg_t *
					msg);
void inline slurm_free_launch_tasks_request_msg(launch_tasks_request_msg_t
						* msg);
void inline
slurm_free_launch_tasks_response_msg(launch_tasks_response_msg_t * msg);
void inline slurm_free_task_exit_msg(task_exit_msg_t * msg);
void inline slurm_free_kill_tasks_msg(kill_tasks_msg_t * msg);
void inline
slurm_free_reattach_tasks_streams_msg(reattach_tasks_streams_msg_t * msg);
void inline slurm_free_revoke_credential_msg(revoke_credential_msg_t *
					     msg);

extern char *job_dist_string(uint16_t inx);
extern char *job_state_string(enum job_states inx);
extern char *job_state_string_compact(enum job_states inx);
extern char *node_state_string(enum node_states inx);
extern char *node_state_string_compact(enum node_states inx);

#define SLURM_JOB_DESC_DEFAULT_CONTIGUOUS	NO_VAL
#define SLURM_JOB_DESC_DEFAULT_KILL_NODE_FAIL	NO_VAL
#define SLURM_JOB_DESC_DEFAULT_ENVIRONMENT	((char **) NULL)
#define SLURM_JOB_DESC_DEFAULT_ENV_SIZE 	0
#define SLURM_JOB_DESC_DEFAULT_FEATURES		NULL
#define SLURM_JOB_DESC_DEFAULT_JOB_ID		NO_VAL
#define SLURM_JOB_DESC_DEFAULT_JOB_NAME 	NULL
#define SLURM_JOB_DESC_DEFAULT_MIN_PROCS	NO_VAL
#define SLURM_JOB_DESC_DEFAULT_MIN_MEMORY	NO_VAL
#define SLURM_JOB_DESC_DEFAULT_MIN_TMP_DISK	NO_VAL
#define SLURM_JOB_DESC_DEFAULT_PARTITION	NULL
#define SLURM_JOB_DESC_DEFAULT_PRIORITY		NO_VAL
#define SLURM_JOB_DESC_DEFAULT_REQ_NODES	NULL
#define SLURM_JOB_DESC_DEFAULT_JOB_SCRIPT	NULL
#define SLURM_JOB_DESC_DEFAULT_SHARED	 	NO_VAL
#define SLURM_JOB_DESC_DEFAULT_TIME_LIMIT	NO_VAL
#define SLURM_JOB_DESC_DEFAULT_NUM_PROCS	NO_VAL
#define SLURM_JOB_DESC_DEFAULT_NUM_NODES	NO_VAL
#define SLURM_JOB_DESC_DEFAULT_USER_ID		NO_VAL
#define SLURM_JOB_DESC_DEFAULT_WORKING_DIR	NULL
void slurm_init_job_desc_msg(job_desc_msg_t * job_desc_msg);
void slurm_init_part_desc_msg(update_part_msg_t * update_part_msg);

#endif
