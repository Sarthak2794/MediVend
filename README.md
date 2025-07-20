# MediVend – Automatic Medicine Vending Machine 💊🤖

MediVend is a smart medicine vending machine that allows users to select required medicines via a Wi-Fi-enabled web interface and get them dispensed automatically. Ideal for basic healthcare stations or remote kiosks.

---

## 🚀 Features
- Mobile-friendly web interface hosted on **ESP32**
- Symptom-based medicine recommendations
- Quantity input for up to **6 medicines**
- HTTP-based communication between frontend and microcontroller
- **QR code** payment trigger after order submission
- Real-time **stepper and servo motor** control for accurate dispensing

---

## 🛠️ Technologies Used
- **ESP32** – Web hosting, HTTP POST handling
- **HTML, CSS, JavaScript** – Frontend
- **Arduino UNO** – Hardware control
- **Stepper & Servo Motors** – Mechanical dispensing
- **ArduinoJson, ESPAsyncWebServer** libraries

---

## 🔁 Workflow

1. ESP32 creates a Wi-Fi hotspot and hosts the medicine selection page.
2. User connects to ESP32 via mobile and selects medicine quantities.
3. On clicking **Submit**, data is sent as a JSON array to ESP32 via **HTTP POST**.
4. ESP32 parses the data and sends it to Arduino over **Serial**.
5. Arduino:
   - Moves **stepper motor** to the appropriate medicine stack.
   - Uses **servo motor** to flick tablets based on selection.
6. A **QR code** is displayed for payment.

---

## 📁 File Structure

