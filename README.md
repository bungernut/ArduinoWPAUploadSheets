# ArduinoWPAUploadSheets

Upload data with an ESP8266 to Google Sheets using the g-script thing, and then pull it down in a jupyter-notebook  

### This works with a ESP8266 NodeMCU board right now...

## Setup G-Script
1. Make a new google sheet.
2. got to `Tools`>`Script Editor`
3. Paste in the following g-script (slightly modified from sources below):
```
function doGet(e) { 
  Logger.log( JSON.stringify(e) );  // view parameters
  var result = 'Ok'; // assume success
  if (e.parameter == 'undefined') {
    result = 'No Parameters';
  }
  else {
    var sheet_id = 'SHEET_ID'; // TODO put sheet_ID
    var sheet = SpreadsheetApp.openById(sheet_id).getSheetByName('SHEET_NAME');  // TODO put sheet name
    var newRow = sheet.getLastRow() + 1;						
    var rowData = [];
    rowData[0] = new Date();  // Timestamp in column A
    for (var param in e.parameter) {
      Logger.log('In for loop, param=' + param);
      var value = stripQuotes(e.parameter[param]);
      Logger.log(param + ':' + e.parameter[param]);
      switch (param) {
        case 'temperature': //Parameter
          rowData[1] = value; //Value in column B
          result = 'Written on column B';
          break;
        case 'humidity': //Parameter
          rowData[2] = value; //Value in column C
          result += ' ,Written on column C';
          break;  
        default:
          result = "unsupported parameter";
      }
    }
    Logger.log(JSON.stringify(rowData));
    // Write new row below
    var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
    newRange.setValues([rowData]);
  }
  // Return result of operation
  return ContentService.createTextOutput(result);
}
function stripQuotes( value ) {
  return value.replace(/^["']|['"]$/g, "");
}
```  
4. Replace the fields `SHEET_ID` and `SHEET_NAME` with the appropiate values.  
5. `Publish` to `web app`  
6. Test the script with the web_browser. So go to the current web app URL, and add `?temperature=21&humidity=21` to the end of the URL before pressing enter.
7. Save the script_ID for the arduino code.


## Setup the ESP8266  
1. Clone this repo to your computer.  
2. Make a new file in `LogDataSheets/` and fill in the fields. This file is included in .gitignore so your secrets should be safe if you try to commit.
```
#define SECRET_SSID ""
#define SECRET_PASS ""
#define SECRET_GSCRIPT_ID ""
```
3. Upload to ESP8266, and watch data hopefully be uploaded to your sheet.

## Download data from Google Sheet with Python  
1. Enable API for sheets, I made desktop-app and download the client configuration.json file. https://developers.google.com/sheets/api/quickstart/python  
2. Use `conda install -c conda-forge google-api-python-client` rather than use PIP to install it.
3. Copy the json file to this folder. It is also added to `.gitignore` to prevent you from uploading it.
4. Run jupyter notebook, and start executing cells. It should open another webbrowser asking if you want to give access to the sheet. If successful this should make a token.pickle to save the authorization whatevers. 
5. I needed to change the displayed format for the number in the sheet in the actual sheet, for example the dats needs to show all the data: YmdHMS. 
6. Run the jupyter-notebook.

**TODO:**
Nano 33 IoT uses WiFiNINA libs to connect to WiFi.  
Example `ConnectWithWPA` is a good start with this, said needed firmware update, but still managed to connect.   

Running the WiFiNINA Firmware Check results in. Again, lets not bother to do an update unless we can't make this work...
```
20:49:57.122 -> WiFiNINA firmware check.
20:49:57.122 -> 
20:49:57.875 -> Firmware version installed: 1.2.3
20:49:57.875 -> Latest firmware version available : 1.4.1
20:49:57.875 -> 
20:49:57.875 -> Check result: NOT PASSED
20:49:57.875 ->  - The firmware version on the module do not match the
20:49:57.875 ->    version required by the library, you may experience
20:49:57.875 ->    issues or failures.
```

***Giving credit where credit is due, the primary sources of this information is from these:***
https://create.arduino.cc/projecthub/24Ishan/log-temperature-data-to-google-sheets-0b189b  
https://lethanhtrieu.wordpress.com/2019/05/11/how-to-send-data-from-esp8266-to-google-drive/  
https://script.google.com/d/1zwxepnNveuW3bVCYewSnyGh0PAIBFxz-YQArSx-AQgxu38fhasaHRHRC/edit?usp=sharing
