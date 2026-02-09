
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoOTA.h>

class MyNetwork
{
public:
    MyNetwork()
        : server(80) {}

    void setup(const char *ssid, const char *password)
    {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        unsigned long startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 20000)
        {
            delay(500);
        }
        if (WiFi.status() != WL_CONNECTED)
        {
            // Could not connect, optionally handle fallback
        }

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

        server.begin();

        ArduinoOTA.setHostname("esp32-device");
        ArduinoOTA.begin();
    }

    void update()
    {
        server.handleClient();
        ArduinoOTA.handle();
    }

    String getLocalIP() const
    {
        return WiFi.localIP().toString();
    }

private:
    WebServer server;
};