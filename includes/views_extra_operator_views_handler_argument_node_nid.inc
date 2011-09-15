<?php
module_load_include('inc', 'views_extra_operator', 'includes/views_extra_operator_common');


/**
 * @file
 * This is the handler for node NID.
 */
class views_extra_operator_views_handler_argument_node_nid extends views_handler_argument_node_nid {

  function option_definition() {
    $options = parent::option_definition();

    $options['veo_operator']['veo_operator'] = array('default' => FALSE);
    $options['veo_functions']['veo_fct'] = array('default' => FALSE);
    $options['veo_functions']['veo_fct_parameters'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    views_extra_operator_options_form_string($this, $form, $form_state);

    unset($form['not']);

  }

  function query($group_by = FALSE) {
    views_extra_operator_query_string($this);
  }

  function admin_summary() {
    $fct = NULL;
    if ($this->options['veo_functions']['veo_fct']) {
      $fct = strtoupper($this->options['veo_functions']['veo_fct']) . '() ';
    }
    $operator = NULL;
    if ($this->options['veo_operator']['veo_operator']) {
      $operator = strtoupper($this->options['veo_operator']['veo_operator']) . ' ';
    }

    return '<span class="views-ascending"><span>' . $fct . '' . $operator . '</span></span>';
  }

}