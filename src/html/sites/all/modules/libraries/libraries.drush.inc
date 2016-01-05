<?php
/**
 * @file
 * Drush integration for Libraries API.
 */

/**
 * Implements hook_drush_command().
 */
function libraries_drush_command() {
  $items = array();

  $items['libraries-list'] = array(
    'description' => dt('Show a list of registered libraries.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('lls', 'lib-list'),
  );

  return $items;
}

/**
 * Implements hook_drush_cache_clear().
 *
 * @see drush_cache_clear_types()
 */
function libraries_drush_cache_clear(array &$types) {
  $types['libraries'] = 'libraries_drush_invalidate_cache';
}

/**
 * Clears the library cache.
 */
function libraries_drush_invalidate_cache() {
  // @see drupal_flush_all_caches()
  foreach (libraries_flush_caches() as $table) {
    cache_clear_all('*', $table, TRUE);
  }
}

/**
 * Command callback. Show a list of registered libraries.
 */
function drush_libraries_list() {
  $libraries = array();
  foreach (libraries_info() as $name => $info) {
    $libraries[$name] = libraries_detect($name);
  }
  ksort($libraries);

  if (empty($libraries)) {
    drush_print('There are no registered libraries.');
  }
  else {
    module_load_include('inc', 'libraries', 'libraries.admin');

    $rows = array();
    // drush_print_table() automatically treats the first row as the header, if
    // $header is TRUE.
    $rows[] = array(
      dt('Name'),
      dt('Status'),
      dt('Version'),
      dt('Variants'),
      dt('Dependencies'),
      dt('Provider'),
    );
    foreach ($libraries as $name => $library) {
      // Only list installed variants.
      $variants = array();
      foreach ($library['variants'] as $variant_name => $variant) {
        if ($variant['installed']) {
          $variants[] = $variant_name;
        }
      }

      $rows[] = array(
        $name,
        $library['installed'] ? dt('OK') : drupal_ucfirst($library['error']),
        ($library['installed'] && $library['version']) ? '-' : $library['version'],
        $variants ? implode(', ', $variants) : '-',
        $library['dependencies'] ? implode(', ', $library['dependencies']) : '-',
        libraries_admin_get_provider($library),
      );
    }

    // Make the possible values for the 'Status' column and the 'Version' header
    // wrap nicely.
    $widths = array(0, 12, 7, 0, 0, 0);
    drush_print_table($rows, TRUE, $widths);
  }
}
