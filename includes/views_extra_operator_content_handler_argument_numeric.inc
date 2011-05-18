<?php
module_load_include('inc', 'views_extra_operator', 'includes/views_extra_operator_common');

/**
 * @file
 * This is the handler for CCK type integer.
 */
class views_extra_operator_content_handler_argument_numeric extends views_handler_argument_numeric {

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    views_extra_operator_options_form_string($this, $form, $form_state);
  }

  function query() {
    views_extra_operator_query_string($this);
  }

}