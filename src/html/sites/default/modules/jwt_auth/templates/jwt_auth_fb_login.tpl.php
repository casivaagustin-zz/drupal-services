<script>
  window.fbAsyncInit = function() {
    FB.init({
      appId      : '<?php echo $fb_app_id ?>',
      xfbml      : true,
      version    : 'v2.5'
    });
  };

  (function(d, s, id){
    var js, fjs = d.getElementsByTagName(s)[0];
    if (d.getElementById(id)) {return;}
    js = d.createElement(s); js.id = id;
    js.src = '//connect.facebook.net/en_US/sdk.js';
    fjs.parentNode.insertBefore(js, fjs);
  }(document, 'script', 'facebook-jssdk'));


  function myFacebookStatus() {
    FB.getLoginStatus(function(response) {
      if (response.status === 'connected') {
        console.log(response);
        jQuery('.token').html(response.authResponse.accessToken);
        console.log('Logged in.');
      }
      else {
        FB.login();
      }
    });
  }

  function myFacebookLogin() {
    FB.login(function(){}, {scope: 'publish_actions'});
  }
</script>
<div>
  <button onclick='myFacebookLogin()'>Login with Facebook</button>
</div>
<div>
  <button onclick='myFacebookStatus()'>Status</button>
</div>
<pre class="token"></pre>