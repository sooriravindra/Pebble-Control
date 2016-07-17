Pebble.addEventListener('ready', function() {
  // PebbleKit JS is ready!
  Pebble.sendAppMessage({'JSReady': 1});
 
});

// Get AppMessage events
Pebble.addEventListener('appmessage', function(e) {
  // Get the dictionary from the message
  var dict = e.payload;
  makeRequest(dict.AppEvent,dict);
});

function makeRequest(event,data)
{
  var method = 'GET';
  var url = 'http://192.168.1.102:3000/';
  var path = '';
  var key = '';
  switch (data.KeyPress){
    case 0:
      key = "up";
      break;
    case 1:
      key = "sel"
      break;
    case 2:
      key = "down";
      break;
    default:
      key="";
  }
  switch(event)
    {
      case 10:
        path = "move_mouse?x="+data.AccelX+"&y="+data.AccelY+"&z="+data.AccelZ+"&key="+key;
        break;
      case 20:
        path = "move_keyboard?x="+data.AccelX+"&y="+data.AccelY+"&z="+data.AccelZ+"&key="+key;
        break;
    }
  // Create the request
  var request = new XMLHttpRequest();
  
  // Specify the callback for when the request is completed
  request.onload = function() {
    // The request was successfully completed!
  };
  
  // Send the request
  
  
  request.open(method, url+path);
  request.send();
}