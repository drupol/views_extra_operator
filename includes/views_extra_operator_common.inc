<?php

function views_extra_operator_options_form_string(&$_this, &$form, &$form_state) {

  $form['arg_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => 'Argument functions'
  );

  $form['arg_fieldset']['arg_fct'] = array(
      '#type' => 'select',
      '#title' => t('SQL Function'),
      '#optional' => TRUE,
      '#description' => t('Choose which SQL function to apply to the argument.'),
      '#options' => array(
          'ascii' => t('Ascii'),
          'format' => t('Format'),
          'hex' => t('Hex'),
          'length' => t('Length'),
          'locate' => t('Locate'),
          'lower' => t('Lower'),
          'oct' => t('Oct'),
          'ord' => t('Ord'),
          'reverse' => t('Reverse'),
          'right' => t('Right'),
          'rpad' => t('Rpad'),
          'soundex' => t('Soundex'),
          'substring' => t('Substring'),
          'substring_index' => t('Substring Index'),
          'unhex' => t('Unhex'),
          'upper' => t('Upper')
      ),
      '#default_value' => $_this->options['arg_fieldset']['arg_fct'],
  );

  $form['arg_fieldset']['arg_fct_parameters'] = array(
      '#type' => 'textfield',
      '#title' => t('Parameters'),
      '#description' => t('If the selected SQL function needs extra parameters, add them here, comma separated.'),
      '#default_value' => $_this->options['arg_fieldset']['arg_fct_parameters'],
  );

  $form['arg_operator'] = array(
      '#type' => 'fieldset',
      '#title' => 'Argument operator'
  );

  $form['arg_operator']['operator'] = array(
      '#type' => 'select',
      '#title' => t('Argument operator'),
      '#description' => t('Choose the operator'),
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
          'op_greater' => t('Is greater'),
          'op_greater_equal' => t('Is greater or equal'),
          'op_lesser' => t('Is lesser'),
          'op_lesser_equal' => t('Is lesser or equal'),
          'op_different' => t('Is different'),
          'op_between' => t('Is between'),
          'op_nbetween' => t('Is not between'),
      ),
      '#default_value' => $_this->options['arg_operator']['operator'],
  );
}

function views_extra_operator_options_filter_form_string(&$_this, &$form, &$form_state) {

  $form['filter_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => 'Filter functions'
  );

  $form['filter_fieldset']['filter_fct'] = array(
      '#type' => 'select',
      '#title' => t('SQL Function'),
      '#optional' => TRUE,
      '#description' => t('Choose which SQL function to apply to the filter.'),
      '#options' => array(
          'ascii' => t('Ascii'),
          'format' => t('Format'),
          'hex' => t('Hex'),
          'length' => t('Length'),
          'locate' => t('Locate'),
          'lower' => t('Lower'),
          'oct' => t('Oct'),
          'ord' => t('Ord'),
          'reverse' => t('Reverse'),
          'right' => t('Right'),
          'rpad' => t('Rpad'),
          'soundex' => t('Soundex'),
          'substring' => t('Substring'),
          'substring_index' => t('Substring Index'),
          'unhex' => t('Unhex'),
          'upper' => t('Upper')
      ),
      '#default_value' => $_this->options['filter_fieldset']['filter_fct'],
  );

  $form['filter_fieldset']['filter_fct_parameters'] = array(
      '#type' => 'textfield',
      '#title' => t('Parameters'),
      '#description' => t('If the selected SQL function needs extra parameters, add them here, comma separated.'),
      '#default_value' => $_this->options['filter_fieldset']['filter_fct_parameters'],
  );

}

function views_extra_operator_query_string(&$_this) {
  $argument = $_this->argument;
  $arg_fct = $_this->options['arg_fieldset']['arg_fct'];
  $arg_fct_parameters = $_this->options['arg_fieldset']['arg_fct_parameters'];

  if (!empty($_this->options['transform_dash'])) {
    $argument = strtr($argument, '-', ' ');
  }

  $_this->ensure_my_table();
  if (empty($_this->options['glossary'])) {
    $field = "$_this->table_alias.$_this->real_field";
  } else {
    $field = $_this->get_formula();
  }

  switch ($arg_fct) {
    case 'ascii':
      $field = "ASCII($field)";
      break;
    case 'format':
      $arg_fct_parameters = (int) $arg_fct_parameters;
      $field = "FORMAT($field, $arg_fct_parameters)";
      break;
    case 'hex':
      $field = "HEX($field)";
      break;
    case 'length':
      $field = "LENGTH($field)";
      break;
    case 'length':
      $field = "LENGTH($field)";
      break;
    case 'locate':
      $substr = $arg_fct_parameters;
      $field = "LOCATE($substr, $field)";
      break;
    case 'lower':
      $field = "LOWER($field)";
      break;
    case 'oct':
      $field = "OCT($field)";
      break;
    case 'ord':
      $field = "ORD($field)";
      break;
    case 'reverse':
      $field = "REVERSE($field)";
      break;
    case 'right':
      $$arg_fct_parameters = (int) $arg_fct_parameters;
      $field = "RIGHT($field, $arg_fct_parameters)";
      break;
    case 'rpad':
      list($len, $padstr) = explode(',', $arg_fct_parameters);
      $field = "RPAD($field, $len, $padstr)";
      break;
    case 'soundex':
      $field = "SOUNDEX($field)";
      break;
    case 'substring':
      list($pos, $len) = explode(',', $arg_fct_parameters);
      $field = "SUBSTRING($field, $pos, $len)";
      break;
    case 'substring_index':
      list($delimt, $count) = explode(',', $arg_fct_parameters);
      $field = "SUBSTRING_INDEX($field, $delim, $count)";
      break;
    case 'unhex':
      $field = "UNHEX($field)";
      break;
    case 'upper':
      $field = "UPPER($field)";
      break;
  }

  switch ($_this->options['arg_operator']['operator']) {
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
    case 'op_greater':
      $argument = (is_numeric($argument)) ? $argument : strlen($argument);
      $_this->query->add_where(0, "$field > %s", $argument);
      break;
    case 'op_greater_equal':
      $argument = (is_numeric($argument)) ? $argument : strlen($argument);
      $_this->query->add_where(0, "$field >= %s", $argument);
      break;
    case 'op_lesser':
      $argument = (is_numeric($argument)) ? $argument : strlen($argument);
      $_this->query->add_where(0, "$field < %s", $argument);
      break;
    case 'op_lesser_equal':
      $argument = (is_numeric($argument)) ? $argument : strlen($argument);
      $_this->query->add_where(0, "$field <= %s", $argument);
      break;
    case 'op_different':
      $argument = (is_numeric($argument)) ? $argument : strlen($argument);
      $_this->query->add_where(0, "$field <> %s", $argument);
      break;
    case 'op_between':
      list($min, $max) = explode(",", $argument);
      $min = (is_numeric($min)) ? $min : strlen($min);
      $max = (is_numeric($max)) ? $max : strlen($max);
      $_this->query->add_where(0, "$field BETWEEN %d AND %d", $min, $max);
      break;
    case 'op_nbetween':
      list($min, $max) = explode(",", $argument);
      $min = (is_numeric($min)) ? $min : strlen($min);
      $max = (is_numeric($max)) ? $max : strlen($max);
      $_this->query->add_where(0, "$field NOT BETWEEN %d AND %d", $min, $max);
      break;
  }
}


function views_extra_operator_query_filter_string(&$_this) {
  
  $filter = $_this->argument;
  
  $filter_fct = $_this->options['filter_fieldset']['filter_fct'];
  $filter_fct_parameters = $_this->options['filter_fieldset']['filter_fct_parameters'];

  $_this->ensure_my_table();

  $field = $_this->real_field;

  switch ($filter_fct) {
    case 'ascii':
      $field = "ASCII($field)";
      break;
    case 'format':
      $arg_fct_parameters = (int) $filter_fct_parameters;
      $field = "FORMAT($field, $filter_fct_parameters)";
      break;
    case 'hex':
      $field = "HEX($field)";
      break;
    case 'length':
      $field = "LENGTH($field)";
      break;
    case 'length':
      $field = "LENGTH($field)";
      break;
    case 'locate':
      $substr = $filter_fct_parameters;
      $field = "LOCATE($substr, $field)";
      break;
    case 'lower':
      $field = "LOWER($field)";
      break;
    case 'oct':
      $field = "OCT($field)";
      break;
    case 'ord':
      $field = "ORD($field)";
      break;
    case 'reverse':
      $field = "REVERSE($field)";
      break;
    case 'right':
      $$arg_fct_parameters = (int) $filter_fct_parameters;
      $field = "RIGHT($field, $arg_fct_parameters)";
      break;
    case 'rpad':
      list($len, $padstr) = explode(',', $filter_fct_parameters);
      $field = "RPAD($field, $len, $padstr)";
      break;
    case 'soundex':
      $field = "SOUNDEX($field)";
      break;
    case 'substring':
      list($pos, $len) = explode(',', $filter_fct_parameters);
      $field = "SUBSTRING($field, $pos, $len)";
      break;
    case 'substring_index':
      list($delimt, $count) = explode(',', $filter_fct_parameters);
      $field = "SUBSTRING_INDEX($field, $delim, $count)";
      break;
    case 'unhex':
      $field = "UNHEX($field)";
      break;
    case 'upper':
      $field = "UPPER($field)";
      break;
  }
  
  $_this->sort_field = $field;
  
}

