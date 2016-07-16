Pebble.addEventListener('ready', function() {
  // PebbleKit JS is ready!
  console.log('PebbleKit requesting 2!');
  Pebble.sendAppMessage({'JSReady': 1});
 
});

// Get AppMessage events
Pebble.addEventListener('appmessage', function(e) {
  // Get the dictionary from the message
  var dict = e.payload;
  makeRequest(dict.AppEvent,dict);

  console.log('Got message: ' + JSON.stringify(dict));
});

function makeRequest(event,data)
{
  var method = 'GET';
  var url = 'http://192.168.1.102:3000/';
  var path = '';
  switch(event)
    {
      case 0:
        path = "button_down";
        break;
      case 1:
        path = "button_up";
        break;
      case 2:
        path = "button_sel";
        break;
      case 3:
        path = "move_mouse?x="+data.AccelX+"&y="+data.AccelY+"&z="+data.AccelZ;
        break;
      case 4:
        path = "move_keyboard?x="+data.AccelX+"&y="+data.AccelY+"&z="+data.AccelZ;
        break;
    }
  // Create the request
  var request = new XMLHttpRequest();
  
  // Specify the callback for when the request is completed
  request.onload = function() {
    // The request was successfully completed!
    console.log('Got response: ' + this.responseText);
  };
  
  // Send the request
  
  console.log('Requesting '+url+path);
  request.open(method, url+path);
  request.send();
  console.log('PebbleKit request sent!');
}