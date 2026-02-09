
#include <WebServer.h>
#include <Update.h>
WebServer server(80);

class MyNetwork
{
public:
    void setup(const char *ssid, const char *password)
    {
        WiFi.disconnect(true);
        WiFi.mode(WIFI_AP);
        WiFi.setTxPower(WIFI_POWER_19_5dBm);
        WiFi.softAP(ssid, password, 6, false, 2);

        register_routes();

        server.begin();
        // OTA via HTTP only, no ArduinoOTA

        mySerial.write("WiFi AP started with SSID: " + String(ssid));
        mySerial.write("AP IP address: " + getLocalIP());
    }

    void update()
    {
        server.handleClient();
    }

    String getLocalIP() const
    {
        return WiFi.softAPIP().toString();
    }

private:
    void register_routes()
    {
        server.on("/get_status", [this]()
                  {
String json = "{";
json += "\"motor\":\"" + String(motor.getOn() ? "ON" : "OFF") + "\",";
json += "\"reading\":\"" + String(r700.get_read() ? "ON" : "OFF") + "\",";
json += "\"status\":\"" + String(controller.box_approved ? "APPROVED" : (controller.box_rejected ? "REJECTED" : "WAITING")) + "\",";
json += "\"retry\":" + String(controller.retry_count) + ",";
json += "\"speed\":" + String(motor.getSpeed()) + ",";
json += "\"emg\":\"" + String(sensors.check_emergency_stop() ? "ON" : "OFF") + "\",";
json += "\"sensor_in\":\"" + String(sensors.readIn() ? "ON" : "OFF") + "\",";
json += "\"sensor_out\":\"" + String(sensors.readOut() ? "ON" : "OFF") + "\"";
json += "}";
server.send(200, "application/json", json); });

        server.on("/", [this]()
                  {
String html =
"<html><head><title>Status</title></head><body>"\
"<pre id='status'>Loading...</pre>"\
"<script>"\
"function fetchStatus() {"\
"  fetch('/get_status').then(r => r.json()).then(j => {"\
"    document.getElementById('status').textContent = JSON.stringify(j, null, 2);"\
"  });"\
"}"\
"setInterval(fetchStatus, 1000);"\
"fetchStatus();"\
"</script>"\
"</body></html>";
server.send(200, "text/html", html); });

        server.on("/update_firmware", HTTP_POST, [this]()
                  {
server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
ESP.restart(); }, [this]()
                  {
HTTPUpload& upload = server.upload();
if (upload.status == UPLOAD_FILE_START) {
if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
Update.printError(Serial);
}
} else if (upload.status == UPLOAD_FILE_WRITE) {
if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
Update.printError(Serial);
}
} else if (upload.status == UPLOAD_FILE_END) {
if (Update.end(true)) {
// Success
} else {
Update.printError(Serial);
}
} });
    }
};