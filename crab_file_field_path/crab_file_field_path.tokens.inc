<?php

/**
 * @file
 * Tokens for the File (Field) Paths module.
 */

use Drupal\Component\Utility\Unicode;
use Drupal\Core\Render\BubbleableMetadata;

/**
 * Implements hook_token_info().
 */
function crab_file_field_path_token_info() {
  $info['tokens']['file']['cffp-name-only'] = [
    'name'        => t("File name"),
    'description' => t("File name without extension."),
  ];

  $info['tokens']['file']['cffp-name-only-original'] = [
    'name'        => t("File name - original"),
    'description' => t("File name without extension - original."),
  ];

  $info['tokens']['file']['cffp-extension-original'] = [
    'name'        => t("File extension - original"),
    'description' => t("File extension - original."),
  ];

  return $info;
}

/**
 * Implements hook_tokens().
 */
function crab_file_field_path_tokens($type, $tokens, array $data, array $options, BubbleableMetadata $bubbleable_metadata) {
  $url_options = ['absolute' => TRUE];
  if (isset($language)) {
    $url_options['language'] = $language;
  }

  $replacements = [];

  if ($type == 'file' && !empty($data['file'])) {
    $file_system = \Drupal::service('file_system');

    /** @var \Drupal\file\Entity\File $file */
    $file = $data['file'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'cffp-name-only':
          $basename = $file_system->basename($file->filename->value);
          $extension = preg_match('/\.[^.]+$/', $basename, $matches) ? $matches[0] : NULL;
          $replacements[$original] = !is_null($extension) ? Unicode::substr($basename, 0, Unicode::strlen($basename) - Unicode::strlen($extension)) : $basename;
          break;

        case 'cffp-name-only-original':
          $basename = $file_system->basename($file->origname->value);
          $extension = preg_match('/\.[^.]+$/', $basename, $matches) ? $matches[0] : NULL;
          $replacements[$original] = !is_null($extension) ? Unicode::substr($basename, 0, Unicode::strlen($basename) - Unicode::strlen($extension)) : $basename;
          break;

        case 'cffp-extension-original':
          $replacements[$original] = preg_match('/[^.]+$/', $file_system->basename($file->origname->value), $matches) ? $matches[0] : NULL;
          break;
      }
    }
  }

  return $replacements;
}
