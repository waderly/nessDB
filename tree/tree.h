/*
 * Copyright (c) 2012-2014 The nessDB Project Developers. All rights reserved.
 * Code is licensed with GPL. See COPYING.GPL file.
 *
 */

#ifndef nessDB_TREE_H_
#define nessDB_TREE_H_

#include "internal.h"
#include "block.h"
#include "posix.h"
#include "hdr.h"
#include "node.h"
#include "options.h"
#include "status.h"

/**
 *
 * @file tree.h
 * @brief a buffered-tree index data structure
 * http://cs.au.dk/~large/Paperpages/bufferalgo.htm
 *
 */

struct tree_callback {
	int (*fetch_node)(void *tree, NID nid, struct node **n);
	int (*flush_node)(void *tree, struct node *n);
	int (*fetch_hdr)(void *tree);
	int (*flush_hdr)(void *tree);
};

struct tree {
	int fd;
	struct hdr *hdr;
	struct block *block;
	struct options *opts;
	struct status *status;
	struct cache_file *cf;
};

struct cache;
struct tree *tree_open(const char *dbname,
                       struct options *opts,
                       struct status *status,
                       struct cache *cache,
                       struct tree_callback *tcb);

int tree_put(struct tree *t,
             struct msg *k,
             struct msg *v,
             msgtype_t type,
             TXN *txn);
void tree_free(struct tree *t);

NID hdr_next_nid(struct tree *t);

int root_put_cmd(struct tree *t, struct bt_cmd *cmd);
void leaf_put_cmd(struct node *node, struct bt_cmd *cmd);
void nonleaf_put_cmd(struct node *node, struct bt_cmd *cmd);
void node_put_cmd(struct tree *t, struct node *node, struct bt_cmd *cmd);
enum reactivity get_reactivity(struct tree *t, struct node *node);
void node_split_child(struct tree *t, struct node *parent, struct node *child);

#endif /* nessDB_TREE_H_ */
