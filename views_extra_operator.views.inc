<?php

/**
 * @file
 * This is the file who let views be aware of your custom stuff.
 */

/**
 *Let Views be aware of those handlers by defining them here.
 */
function views_extra_operator_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_extra_operator') .'/includes',
    ),
    'handlers' => array(
      'views_extra_operator_views_handler_argument_string' => array(
        'parent' => 'views_handler_argument_string'
      ),
      'views_extra_operator_content_handler_argument_string' => array(
        'parent' => 'content_handler_argument_string'
      ),
      'views_extra_operator_views_handler_argument_node_nid' => array(
        'parent' => 'views_handler_argument_node_nid'
      ),
      'views_extra_operator_content_handler_argument_numeric' => array(
        'parent' => 'views_handler_argument_numeric'
      ),
      'views_extra_operator_views_handler_argument_numeric' => array(
        'parent' => 'views_handler_argument_numeric'
      )
    )
  );
}