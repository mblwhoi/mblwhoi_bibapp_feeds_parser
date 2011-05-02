<?php

// Load the parser library.
require_once(drupal_get_path('module', 'mblwhoi_bibapp_feeds_parser') . '/mblwhoi_bibapp_parser.inc');

/**
 * Parses BibApp Works RDF.
 */
class BibAppWorksRdfParser extends FeedsParser {

  /**
   * Parses a raw string and populates FeedsImportBatch object from it.
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {

    // Get the file's content.
    $string = $batch->getRaw();

    // Parse it.
    $items = parse_bibapp_works_rdf($string);

    // Populate the FeedsImportBatch object with the parsed results.
    $batch->title = 'bibapp Feed';
    $batch->items = $items;
  }


  /**
   * Override of getSourceElement.
   * Our implementation downloads and parses author information if it has not already been parsed.
   * We do this here to avoid memory/time limits.  The batch process should help us avoid those problems.
   */
  public function getSourceElement(FeedsImportBatch $batch, $element_key) {
    if (($node = node_load($batch->feed_nid)) && $element_key == 'parent:uid') {
      return $node->uid;
    }

    $item = $batch->currentItem();

    // If we are requesting linked author information...
    if ($element_key == 'linked_authors_json'){

      // If we have not yet downloaded the author information...
      if (! isset($item['linked_authors_json'])){

        // Initialize a list of linked author info objects.
        $linked_author_infos = array();

        // For each linked author uri...
        foreach ($item['linked_author_uris'] as $linked_author_uri){

          // Try to download the author info.
          try {
            $linked_author_infos[] = get_author_info($linked_author_uri);
          }
          catch (Exception $e){
            // Do nothing.
          }
        }

        // Create a json string representing the linked author infos.
        $item['linked_authors_json'] = json_encode($linked_author_infos);

      }

      // Return the linked_authors_json.
      return $item['linked_authors_json'];

    }

    // Otherwise return the value from the item.
    return isset($item[$element_key]) ? $item[$element_key] : '';
  }



  public function getMappingSources() {

    return array(
                 'work_uri' => array(
                                  'name' => t('Work URI'),
                                  'description' => t('Work URI'),
                                  ),

                 'title' => array(
                                  'name' => t('Title'),
                                  'description' => t('Title'),
                                  ),

                 'rft.genre' => array(
                                  'name' => t('Genre'),
                                  'description' => t('Genre'),
                                  ),

                 'rft.jtitle' => array(
                                  'name' => t('Journal Title'),
                                  'description' => t('Journal Title'),
                                  ),

                 'rft.date' => array(
                                  'name' => t('Publication Date'),
                                  'description' => t('Publication Date'),
                                  ),

                 'rft.volume' => array(
                                  'name' => t('Volume'),
                                  'description' => t('Volume'),
                                  ),

                 'authors_list' => array(
                                  'name' => t('Authors List'),
                                  'description' => t('Authors List'),
                                  ),

                 'rft.issue' => array(
                                  'name' => t('Issue'),
                                  'description' => t('Issue'),
                                  ),

                 'rft.spage' => array(
                                  'name' => t('Start Page'),
                                  'description' => t('Start Page'),
                                  ),
                 'rft.epage' => array(
                                  'name' => t('End Page'),
                                  'description' => t('End Page'),
                                  ),

                 'rft.issn' => array(
                                  'name' => t('ISSN'),
                                  'description' => t('ISSN'),
                                  ),

                 'linked_authors_json' => array(
                                  'name' => t('Linked Authors JSON'),
                                  'description' => t('Linked Authors JSON'),
                                  ),
                 );

  }

  }
?>