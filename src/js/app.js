Pebble.addEventListener('ready', function() {
  // PebbleKit JS is ready!
  console.log('PebbleKit requesting 2!');
  var method = 'GET';
  var url = 'http://192.168.1.102:3000/move';
  
  // Create the request
  var request = new XMLHttpRequest();
  
  // Specify the callback for when the request is completed
  request.onload = function() {
    // The request was successfully completed!
    console.log('Got response: ' + this.responseText);
  };
  
  // Send the request
  request.open(method, url);
  request.send();
  console.log('PebbleKit request sent!');
});