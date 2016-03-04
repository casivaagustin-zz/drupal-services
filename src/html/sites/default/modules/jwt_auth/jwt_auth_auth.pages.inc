<?php

/**
 * Page Callback that will return a JWT if the credentials are valid.
 *
 * Url : /oauth2/auth
 * Method : POST
 * Content Type : Form Url Encoded
 * Args:
 *   username : Username or Email
 *   password : Password
 *   client : A valid client string (see oauth2 server)
 *
 */
function jwt_auth_auth_page() {
  $request = OAuth2\Request::createFromGlobals();

  $username = trim($request->request('username'));
  $password = trim($request->request('password'));
  $client_id = trim($request->request('client_id'));

  if (empty($username)) {
    return  drupal_json_output(array(
      'error' => "empty_username",
      'error_description' => t("User Name is empty")
    ));
  }

  if (empty($password) && empty($token)) {
    return drupal_json_output(array(
      'error' => "empty_challenge",
      'error_description' => t("Password and Token is empty")
    ));
  }

  if (empty($client_id)) {
    return drupal_json_output(array(
      'error' => "empty_client_id",
      'error_description' => t("Client ID is empty")
    ));
  }

  if (valid_email_address($username)) {
    $account = user_load_by_mail($username);
  }
  else {
    $account = user_load_by_name($username);
  }

  if (empty($account)) {
    return drupal_json_output(array(
      'error' => "invalid_user",
      'error_description' => t("Invalid User")
    ));
  }

  if (!empty($password)) {
    $auth = user_authenticate($account->name, $password);
    if (!is_numeric($auth)) {
      return drupal_json_output(array(
        'error' => "invalid_credentials",
        'error_description' => t("Invalid Credentials")
      ));
    }
  }
  else {
    return drupal_json_output(array(
      'error' => "invalid_case",
      'error_description' => t("No password")
    ));
  }

  try {
    $request->request['assertion'] = jwt_auth_gen_jws($account->uid, $client_id);
    $request->request['grant_type'] = 'urn:ietf:params:oauth:grant-type:jwt-bearer';

    $client_credentials = oauth2_server_get_client_credentials($request);
    $server = NULL;

    // Get the client and use it to load the server and initialize the server.
    if ($client_credentials) {
      $client = oauth2_server_client_load($client_credentials['client_id']);
      if ($client) {
        $server = oauth2_server_load($client->server);
      }
    }

    // Initialize the server.
    $response = new OAuth2\Response();
    $oauth2_server = oauth2_server_start($server);
    $oauth2_server->handleTokenRequest($request, $response);
    return oauth2_server_send_response($response);
  } catch(Exception $e) {
    return drupal_json_output(array(
      'error' => "exception",
      'error_description' => t($e->getMessage()),
    ));
  }

}

/**
 * Generate a valid JWT request packet to create a JWT Token.
 *
 * @param $uid : User ID
 * @param $client_id : Client (see oauth2 server).
 *
 * @return string
 *
 * @throws Exception
 */
function jwt_auth_gen_jws($uid, $client_id) {
  global $base_url;

  $private_key = variable_get('jwt_client_private_key', array());

  if (empty($private_key)) {
    watchdog('jwt_auth', 'Empty Private Key', WATCHDOG_ERROR);
    throw new Exception('Empty Private Key');
  }

  $jwt_util = new OAuth2\Encryption\Jwt();

  $token_url = $base_url . '/oauth2/token';

  $jwt_data = array(
    'iss' => $client_id,
    'exp' => time() + 1000,
    'iat' => time(),
    'sub' => $uid,
    'aud' => $token_url,
    'jti' => '123456',
    'server' => 'api',
  );

  return $jwt_util->encode($jwt_data, $private_key, 'RS256');
}


/**
 * Using a refresh token will generate a new token.
 */
function jwt_auth_refresh_page() {
  $request = OAuth2\Request::createFromGlobals();

  $client_id = trim($request->request('client_id'));

  if (empty($client_id)) {
    return drupal_json_output(array(
      'error' => "empty_client_key",
      'error_description' => t("Client ID is empty"),
    ));
  }

  module_load_include('inc', 'oauth2_server', 'oauth2_server.pages');
  return oauth2_server_token_page();
}