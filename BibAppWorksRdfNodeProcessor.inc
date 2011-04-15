<?php
/**
 * @file
 * Class definition of BibAppWorksRdfNodeProcessor.
 * Subclass of FeedsNodeProcessor, just redefines FEEDS_NODE_BATCH_SIZE.
 */

// Create or delete FEEDS_NODE_BATCH_SIZE at a time.
define('FEEDS_NODE_BATCH_SIZE', 10);

class BibAppWorksRdfNodeProcessor extends FeedsNodeProcessor {
}

/**
 * Copy of node_delete() that circumvents node_access().
 *
 * Used for batch deletion.
 */
function _feeds_node_delete($nid) {

  $node = node_load($nid);

  db_query("DELETE FROM {node} WHERE nid = %d", $node->nid);
  db_query("DELETE FROM {node_revisions} WHERE nid = %d", $node->nid);

  // Call the node-specific callback (if any):
  node_invoke($node, 'delete');
  node_invoke_nodeapi($node, 'delete');

  // Clear the page and block caches.
  cache_clear_all();

  // Remove this node from the search index if needed.
  if (function_exists('search_wipe')) {
    search_wipe($node->nid, 'node');
  }
  watchdog('content', '@type: deleted %title.', array('@type' => $node->type, '%title' => $node->title));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => node_get_types('name', $node), '%title' => $node->title)));
}
