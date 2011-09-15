<?php
module_load_include('inc', 'views_extra_operator', 'includes/views_extra_operator_common');

class views_extra_operator_views_sort extends views_handler_sort {
  /**
   * Called to add the sort to a query.
   */
  function query() {
    $this->ensure_my_table();
    if (empty($this->options['veo_functions']['veo_fct'])) {
      $this->query->add_orderby($this->table_alias, $this->real_field, $this->options['order']);
    } else {
      $this->sort_field = views_extra_operator_rewrite_field($this->table_alias . '.' . $this->real_field, $this->options['veo_functions']['veo_fct'], $this->options['veo_functions']['veo_fct_parameters']);
      $this->query->add_orderby(NULL, $this->sort_field, $this->options['order'], $this->options['veo_functions']['veo_fct'] . '_' . $this->table_alias . '_' . $this->real_field);
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['order'] = array('default' => 'ASC');

    return $options;
  }

  function admin_summary() {
    switch ($this->options['order']) {
      case 'ASC':
      case 'asc':
      default:
        $type = t('asc');
        break;
      case 'DESC';
      case 'desc';
        $type = t('desc');
        break;
    }

    $fct = null;
    if ($this->options['veo_functions']['veo_fct']) {
      $fct = strtoupper($this->options['veo_functions']['veo_fct']) . '() ';
    }

    return '<span class="views-ascending"><span>' . $fct . '' . $type . '</span></span>';
  }

  /**
   * Basic options for all sort criteria
   */
  function options_form(&$form, &$form_state) {
    views_extra_operator_options_form_string($this, $form, $form_state);
    unset($form['veo_operator']);

    $form['order'] = array(
      '#type' => 'radios',
      '#title' => t('Sort order'),
      '#options' => array('ASC' => t('Ascending'), 'DESC' => t('Descending')),
      '#default_value' => $this->options['order'],
    );
  }
}
