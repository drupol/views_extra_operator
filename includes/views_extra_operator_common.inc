<?php

function views_extra_operator_options_form_string(&$_this, &$form, &$form_state) {

  $form['veo_functions'] = array(
      '#type' => 'fieldset',
      '#title' => 'Argument functions'
  );

  $form['veo_functions']['veo_fct'] = array(
      '#type' => 'select',
      '#title' => t('SQL Function'),
      '#description' => t('Choose which SQL function to apply'),
        '#options' => array(
          '' => t('<select>'),
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
      '#default_value' => isset($_this->options['veo_functions']['veo_fct'])  ? $_this->options['veo_functions']['veo_fct'] : NULL
  );

  $form['veo_functions']['veo_fct_parameters'] = array(
      '#type' => 'textfield',
      '#title' => t('Parameters'),
      '#description' => t('If the selected SQL function needs extra parameters, add them here, comma separated.'),
      '#default_value' => $_this->options['veo_functions']['veo_fct_parameters'],
  );

  $form['veo_operator'] = array(
      '#type' => 'fieldset',
      '#title' => 'Argument operator'
  );

  $form['veo_operator']['veo_operator'] = array(
      '#type' => 'select',
      '#title' => t('Operator'),
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
      '#default_value' => $_this->options['veo_operator']['veo_operator'],
  );
}

function views_extra_operator_rewrite_field($field, $fct, $fct_parameters) {
  switch ($fct) {
    case 'ascii':
      $field = "ASCII($field)";
      break;
    case 'format':
      $fct_parameters = (int) $fct_parameters;
      $field = "FORMAT($field, $fct_parameters)";
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
      $substr = $fct_parameters;
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
      $fct_parameters = (int) $fct_parameters;
      $field = "RIGHT($field, $fct_parameters)";
      break;
    case 'rpad':
      list($len, $padstr) = explode(',', $fct_parameters);
      $field = "RPAD($field, $len, $padstr)";
      break;
    case 'soundex':
      $field = "SOUNDEX($field)";
      break;
    case 'substring':
      list($pos, $len) = explode(',', $fct_parameters);
      $field = "SUBSTRING($field, $pos, $len)";
      break;
    case 'substring_index':
      list($delimt, $count) = explode(',', $fct_parameters);
      $field = "SUBSTRING_INDEX($field, $delim, $count)";
      break;
    case 'unhex':
      $field = "UNHEX($field)";
      break;
    case 'upper':
      $field = "UPPER($field)";
      break;
  }

   return $field;

  }
  
function views_extra_operator_query_string(&$_this) {
  $argument = $_this->argument;
  $multi = false;
  if (!empty($_this->options['break_phrase'])
        && ($_this->options['veo_operator']['veo_operator'] == 'op_equal' || $_this->options['veo_operator']['veo_operator'] == 'op_nequal')) {
    views_break_phrase($argument, &$this);
    if (count($_this->value) > 1) {
      $multi = true;
    }
  }
  else {
    $fct = $_this->options['veo_functions']['veo_fct'];
    $fct_parameters = $_this->options['veo_functions']['veo_fct_parameters'];
  
    if (!empty($_this->options['transform_dash'])) {
      $argument = strtr($argument, '-', ' ');
    }
  }
  
  $_this->ensure_my_table();
  if (empty($_this->options['glossary'])) {
    $field = "$_this->table_alias.$_this->real_field";
  } else {
    $field = $_this->get_formula();
  }

  if (!isset($_this->options['veo_operator']['veo_operator'])) {
    if ($_this->options['not'] == 0) {
      $_this->options['veo_operator']['veo_operator'] = 'op_equal';
    } else {
      $_this->options['veo_operator']['veo_operator'] = 'op_nequal';
    }
  }
 
  $field = views_extra_operator_rewrite_field($field, $fct, $fct_parameters);
  
  switch ($_this->options['veo_operator']['veo_operator']) {
    case 'op_equal':
      if ($multi) {
        $placeholders = implode(', ', array_fill(0, sizeof($_this->value), '%d'));
        $_this->query->add_where(0, "$field IN ($placeholders)", $_this->value);
      } else {
        $_this->query->add_where(0, "$field = '%s'", $argument);
      }
      break;
    case 'op_nequal':
      if ($multi) {
        $placeholders = implode(', ', array_fill(0, sizeof($_this->value), '%d'));
        $_this->query->add_where(0, "$field NOT IN ($placeholders)", $_this->value);
      } else {
        $_this->query->add_where(0, "$field != '%s'", $argument);
      }
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


function views_extra_operator_query_sort_string(&$_this) {
  
  $sort = $_this->argument;
  
  $fct = $_this->options['filter_fieldset']['filter_fct'];
  $fct_parameters = $_this->options['filter_fieldset']['filter_fct_parameters'];

  $_this->ensure_my_table();

  $field = $_this->real_field;
  
  $_this->sort_field = views_extra_operator_rewrite_field($field, $fct, $fct_parameters);
  
}

