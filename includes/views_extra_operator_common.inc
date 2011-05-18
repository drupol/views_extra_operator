<?php 

function views_extra_operator_options_form_string(&$_this, &$form, &$form_state) {
  $form['operator'] = array(
    '#type' => 'select',
    '#title' => t('Argument operator'),
    '#description' => t('Choose how to handle the argument'),
    '#options' => array(
      'op_equal' => t('Is equal to'),
      'op_nequal' => t('Is not equal to'),
      'op_contains' => t('Contains'),
      'op_contains_any_word' => t('Contains any words'),
      'op_contains_all_words' => t('Contains all words'),
      'op_start_with' => t('Start with'),
      'op_nstart_with' => t('Does not start with'),
      'op_ends_with' => t('Ends with'),
      'op_nends_with' => t('Does not end with'),
      'op_ncontains' => t('Does not contain'),
      'op_length_greater' => t('Argument length greater'),
      'op_length_greater_equal' => t('Argument length greater or equal'),
      'op_length_lesser' => t('Argument length lesser'),
      'op_length_lesser_equal' => t('Argument length lesser or equal'),
      'op_length_different' => t('Argument length different'),
      'op_length_between' => t('Argument length is between'),
      'op_length_nbetween' => t('Argument length is not between'),
    ),
    '#default_value' => $_this->options['operator'],
  );
}

function views_extra_operator_query_string(&$_this) {
  $argument = $_this->argument;
  
  if (!empty($_this->options['transform_dash'])) {
    $argument = strtr($argument, '-', ' ');
  }
  
  $_this->ensure_my_table();
  if (empty($_this->options['glossary'])) {
    $field = "$_this->table_alias.$_this->real_field";
  }
  else {
    $field = $_this->get_formula();
  }
  
  switch ($_this->options['operator']) {
    case 'op_equal':
      $_this->query->add_where(0, "$field = '%s'", $argument);
    break;
    case 'op_nequal':
      $_this->query->add_where(0, "$field != '%s'", $argument);
    break;
    case 'op_contains':
      $_this->query->add_where(0, "$field LIKE '%%%s%%'", $argument);
    break;
    case 'op_contains_any_word':
      $where = array();
      preg_match_all('/ (-?)("[^"]+"|[^" ]+)/i', ' ' . $argument, $matches, PREG_SET_ORDER);
      foreach ($matches as $match) {
        $phrase = FALSE;
        // Strip off phrase quotes
        if ($match[2]{0} == '"') {
          $match[2] = substr($match[2], 1, -1);
          $phrase = TRUE;
        }
        $words = trim($match[2], ',?!();:-');
        $words = $phrase ? array($words) : preg_split('/ /', $words, -1, PREG_SPLIT_NO_EMPTY);
        foreach ($words as $word) {
          $where[] = "$upper($field) LIKE $upper('%%%s%%')";
          $values[] = trim($word, " ,!?");
        }
      }
      
      $where = '(' . implode(' OR ', $where) . ')';
      $_this->query->add_where(0, $where, $values);
    break;
    case 'op_contains_all_words':
      $where = array();
      preg_match_all('/ (-?)("[^"]+"|[^" ]+)/i', ' ' . $argument, $matches, PREG_SET_ORDER);
      foreach ($matches as $match) {
        $phrase = FALSE;
        // Strip off phrase quotes
        if ($match[2]{0} == '"') {
          $match[2] = substr($match[2], 1, -1);
          $phrase = TRUE;
        }
        $words = trim($match[2], ',?!();:-');
        $words = $phrase ? array($words) : preg_split('/ /', $words, -1, PREG_SPLIT_NO_EMPTY);
        foreach ($words as $word) {
          $where[] = "$upper($field) LIKE $upper('%%%s%%')";
          $values[] = trim($word, " ,!?");
        }
      }

      $where = implode(' AND ', $where);
      $_this->query->add_where(0, $where, $values);
    break;
    case 'op_start_with':
      $_this->query->add_where(0, "$field LIKE '%s%%'", $argument);
    break;
    case 'op_nstart_with':
      $_this->query->add_where(0, "$field NOT LIKE '%s%%'", $argument);
    break;
    case 'op_ends_with':
      $_this->query->add_where(0, "$field LIKE '%%%s'", $argument);
    break;
    case 'op_nends_with':
      $_this->query->add_where(0, "$field NOT LIKE '%%%s'", $argument);
    break;
    case 'op_ncontains':
      $_this->query->add_where(0, "$field NOT LIKE '%%%s%%'", $argument);
    break;
    case 'op_length_greater':
      $argument = (is_numeric($argument)) ? $argument : strlen($argument); 
      $_this->query->add_where(0, "LENGTH($field) > %s", $argument);
    break;
    case 'op_length_greater_equal':
      $argument = (is_numeric($argument)) ? $argument : strlen($argument); 
      $_this->query->add_where(0, "LENGTH($field) >= %s", $argument);
    break;
    case 'op_length_lesser':
      $argument = (is_numeric($argument)) ? $argument : strlen($argument); 
      $_this->query->add_where(0, "LENGTH($field) < %s", $argument);
    break;
    case 'op_length_lesser_equal':
      $argument = (is_numeric($argument)) ? $argument : strlen($argument); 
      $_this->query->add_where(0, "LENGTH($field) <= %s", $argument);
    break;
    case 'op_length_different':
      $argument = (is_numeric($argument)) ? $argument : strlen($argument); 
      $_this->query->add_where(0, "LENGTH($field) <> %s", $argument);
    break;
    case 'op_length_between':
      list($min, $max) = explode(",", $argument);
      $min = (is_numeric($min)) ? $min : strlen($min);
      $max = (is_numeric($max)) ? $max : strlen($max);
      $_this->query->add_where(0, "LENGTH($field) BETWEEN %d AND %d", $min, $max);
    break;
    case 'op_length_nbetween':
      list($min, $max) = explode(",", $argument);
      $min = (is_numeric($min)) ? $min : strlen($min);
      $max = (is_numeric($max)) ? $max : strlen($max);
      $_this->query->add_where(0, "LENGTH($field) NOT BETWEEN %d AND %d", $min, $max);
    break;
  }
}
  