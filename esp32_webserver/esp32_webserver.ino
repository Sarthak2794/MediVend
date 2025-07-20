#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Wire.h>

const char* ssid = "ESP32_AP";
const char* password = "12345678";

AsyncWebServer server(80);

byte medicines[6] = {0}; // Stores received medicine quantities

// HTML Page (Served from ESP32)
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Automatic Medicine Vending Machine</title>
    <style>
        body {
            background-color: #000814;
            color: white;
            font-family: Arial, sans-serif;
            text-align: center;
        }
        .container {
            background-color: #003566;
            padding: 20px;
            border-radius: 10px;
            margin: 50px auto;
            width: 80%;
            max-width: 600px;
        }
        .card {
            background-color: #bfd7ea;
            color: #000814;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
            margin-top: 15px;
        }
        a {
            color: white;
        }
        #lastmsg {
            color: rgb(194, 145, 239);
        }
        .medicines {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 10px;
            margin-bottom: 20px;
        }
        .medicine {
            background-color: #ffb703;
            padding: 10px;
            border-radius: 5px;
        }
        input {
            width: 100%;
            padding: 5px;
            border: 1px solid #ccc;
            border-radius: 5px;
        }
        button {
            background-color: #007bff;
            color: white;
            border: none;
            padding: 10px 20px;
            border-radius: 5px;
            cursor: pointer;
        }
        button:hover {
            background-color: #0056b3;
        }
        #qrCode {
            margin-top: 20px;
            width: 150px;
            display: none;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Medicine Selection</h1>
        <hr>
        <h2>Please refer to the Recommendations Below</h2>
        <div class="card">
            <p><strong>Fever</strong> - Paracetamol</p>
            <p><strong>High Fever</strong> - Dolo</p>
            <p><strong>Cough</strong> - Coffcils</p>
            <p><strong>Headache</strong> - Crocine</p>
            <p><strong>Indigestion</strong> - Hajmola</p>
            <p><strong>Acidity</strong> - Gas O Fast</p>
        </div>
        <hr>
        <div class="medicines">
            <div class="medicine">
                <label for="med1">Paracetamol:</label>
                <input type="number" id="med1" placeholder="Quantity" min="0">
            </div>
            <div class="medicine">
                <label for="med2">Dolo:</label>
                <input type="number" id="med2" placeholder="Quantity" min="0">
            </div>
            <div class="medicine">
                <label for="med3">Coffcils:</label>
                <input type="number" id="med3" placeholder="Quantity" min="0">
            </div>
            <div class="medicine">
                <label for="med4">Crocine:</label>
                <input type="number" id="med4" placeholder="Quantity" min="0">
            </div>
            <div class="medicine">
                <label for="med5">Hajmola:</label>
                <input type="number" id="med5" placeholder="Quantity" min="0">
            </div>
            <div class="medicine">
                <label for="med6">Gas O Fast:</label>
                <input type="number" id="med6" placeholder="Quantity" min="0">
            </div>
        </div>
        <a href="https://meet.google.com/landing">Hyperlink to Google Meet</a>
        <br><br>
        <button id="submitBtn" onclick="sendData()">Submit</button>
        <h2 id="plaintext">Pay using the QR Code</h2>
        <hr>
        <h2 id="lastmsg">Thank you for using MEDIVEND</h2>
        <img id="qrCode" src="GooglePay_QR.png" alt="QR Code">
        <h3>Last Order: <span id="lastOrder">None</span></h3>
    </div>
    <script>
        function sendData() {
            let data = [
                document.getElementById("med1").value || 0,
                document.getElementById("med2").value || 0,
                document.getElementById("med3").value || 0,
                document.getElementById("med4").value || 0,
                document.getElementById("med5").value || 0,
                document.getElementById("med6").value || 0
            ].map(Number);
            
            fetch('/submit', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(data)
            })
            .then(response => response.text())
            .then(text => {
                alert(text);
                document.getElementById("qrCode").style.display = "block";
            })
            .catch(error => console.error('Error:', error));
        }

        function getLastOrder() {
            fetch('/status')
            .then(response => response.json())
            .then(data => {
                document.getElementById("lastOrder").textContent = data.length ? data.join(", ") : "None";
            })
            .catch(error => console.error('Error:', error));
        }

        window.onload = getLastOrder;
    </script>
</body>
</html>

)rawliteral";

void setup() {
  Serial.begin(115200);
  Wire.begin();
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");

  // Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlPage);
  });

  // Receive medicine quantities from the webpage
server.on("/submit", HTTP_POST, NULL, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    // Convert received data to a string
    String jsonBody = "";
    for (size_t i = 0; i < len; i++) {
        jsonBody += (char)data[i];
    }

    Serial.println("Received JSON: " + jsonBody);

    // Parse JSON
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, jsonBody);

    if (!error) {
        for (int i = 0; i < 6; i++) {
            medicines[i] = doc[i];
        }

        Serial.print("Stored Quantities: ");
        for (int i = 0; i < 6; i++) {
            Serial.print(medicines[i]);
            Serial.print(" ");
        }
        Serial.println();

        request->send(200, "text/plain", "Data received successfully!");
    } else {
        request->send(400, "text/plain", "Invalid JSON format");
    }
});

  // Send the last received medicine order
  server.on("/submit", HTTP_POST, [](AsyncWebServerRequest *request){
    // Set up an asynchronous request handler to read the request body
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Processing...");
    request->send(response);
}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    // Convert received data to a string
    String jsonBody = "";
    for (size_t i = 0; i < len; i++) {
        jsonBody += (char)data[i];
    }

    Serial.println("Received JSON: " + jsonBody);

    // Parse JSON
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, jsonBody);
    
    if (!error) {
        for (int i = 0; i < 6; i++) {
            medicines[i] = doc[i];
        }

        Serial.print("Stored Quantities: ");
        for (int i = 0; i < 6; i++) {
            Serial.print(medicines[i]);
            Serial.print(" ");
        }
        Serial.println();

        Wire.beginTransmission(8);
        Wire.write(medicines, 6);
        Wire.endTransmission();

        request->send(200, "text/plain", "Data received successfully!");
    } else {
        request->send(400, "text/plain", "Invalid JSON format");
    }
});


  server.begin();
}

void loop() {
  // Future logic for dispensing medicines can be added here
}
