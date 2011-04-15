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
