<?php
// $Id$
require_once('views_extra_operator_common.inc');

/**
 * @file
 * This is the handler for string fields type.
 */
class views_extra_operator_views_handler_argument_string extends views_handler_argument_string {

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    views_extra_operator_options_form_string($this, $form, $form_state);
  }

  function query() {
    views_extra_operator_query_string($this);
  }

}

