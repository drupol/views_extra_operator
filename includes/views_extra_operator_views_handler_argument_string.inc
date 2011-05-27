<?php
module_load_include('inc', 'views_extra_operator', 'includes/views_extra_operator_common');

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

  function admin_summary() {
    $fct = NULL;
    if ($this->options['veo_functions']['veo_fct']) {
      $fct = strtoupper($this->options['veo_functions']['veo_fct']).'() ';
    }
    $operator = NULL;
    if ($this->options['veo_operator']['veo_operator']) {
      $operator = strtoupper($this->options['veo_operator']['veo_operator']).' ';
    }

    return '<span class="views-ascending"><span>' . $fct . '' . $operator . '</span></span>';
  }


}

