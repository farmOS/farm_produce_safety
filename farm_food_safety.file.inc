<?php

/**
 * @file
 * Farm food safety file management code.
 * This file encapsulates code that relates to food safety document management.
 */

/**
 * Define Food Safety document types.
 */
function farm_food_safety_document_types() {
  return array(
    'plan' => t('Food safety plan'),
    'sop' => t('Standard Operating Procedure (SOP)'),
    'training' => t('Training material'),
    'recall' => t('Recall plan'),
    'other' => t('Other'),
  );
}

/**
 * Document upload form.
 */
function farm_food_safety_document_upload_form($form, &$form_state) {
  $extensions = farm_fields_file_types();
  $form['file'] = array(
    '#type' => 'managed_file',
    '#title' => t('File'),
    '#description' => t('Select a file to upload. Allowed file extensions:') . ' ' . $extensions,
    '#required' => TRUE,
    '#progress_indicator' => 'bar',
    '#upload_validators' => array(
      'file_validate_extensions' => array($extensions),
    ),
    '#upload_location' => 'public://food_safety',
  );
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Document type'),
    '#description' => t('Specify what type of document this is.'),
    '#options' => farm_food_safety_document_types(),
    '#required' => TRUE,
  );
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#description' => t('Provide a brief one-line description of this file.'),
    '#required' => TRUE,
    '#maxlength' => 256,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Document upload form submit.
 */
function farm_food_safety_document_upload_form_submit($form, &$form_state) {

  // Load the file via file.fid.
  $file = file_load($form_state['values']['file']);

  // Make the storage of the file permanent.
  $file->status = FILE_STATUS_PERMANENT;

  // Save the file information.
  file_save($file);

  // Add a record to the {farm_food_safety_doc} table.
  $record = [
    'fid' => $file->fid,
    'type' => check_plain($form_state['values']['type']),
    'label' => check_plain($form_state['values']['label']),
  ];
  drupal_write_record('farm_food_safety_doc', $record);

  // Record that this module is using the file.
  file_usage_add($file, 'farm_food_safety', 'farm_food_safety_doc', 0);

  // Set a response to the user.
  drupal_set_message(t('The document has been saved: @filename.', ['@filename' => $file->filename]));
}