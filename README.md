# HackOchi v1.0.0

Welcome, agent. HackOchi is a handheld device for hackers, network, and reconnaissance enthusiasts, or any hooman who loves cool cyberware.

HackOchi includes a vast array of features, including a special Pixel spirit pet called Foxie for non-persistent reconnaissance, a fast Fourier FFT waterfall like graph for Wi-Fi packets and BLE/BT Classic advertisements, and much more.

```see the demo and read till end... thwanks hooman!```  ![ed04e0f4908385ee](https://github.com/user-attachments/assets/f5e06e49-372e-46ec-aeb0-738799668609)

## Demo and Progress

video

## Features :
```(please see the demo video for full HackOchi Experience)```  


- **Custom Boot Sequence:** A unique HackOchi startup animation to kick things off right.

- **Pixel Spirit Pet (Foxie):** Your non-persistent reconnaissance companion, with flash alerts and customizable notifications to keep you in the loop.

- **Environmental Recon Dashboard:** Overview of surroundings, sensors, and ambient data at a glance.

- **Wi-Fi Reconnaissance:** Capture all the intel like SSIDs, BSSIDs, encryption, and more for thorough network mapping.

- **Current Network Dashboard:** Real time signal gauge, connection stats, and essential details (demo shows it in action).

- **Wi-Fi Packet Visualization:** Fast Fourier FFT waterfall style graph across all channels, user can switch channels on the fly for dynamic spectrum analysis.

- **BLE and BT Classic Scanning:** Clean table view with MAC addresses, signal strength, device names, and full packet details.

- **BLE/BT Advertisement Waterfall Graph:** Real time visual spectrum for sniffing and decoding adverts with FFT waterfall graph with full packet details on channel 37, 38, 39 and PPS.

- **Dedicated BLE and BT Recon Scans:** Separate modes pulling every scrap of data in the AIR, displayed in easy to digest formats.



## Detailed Menu : ![28592c2bf509c53f](https://github.com/user-attachments/assets/22b4d902-1c84-43f4-989e-983c66de3120)


**Central Dashboard:**

- Current heap memory usage for real-time system insights.
- Time, temperature, and humidity readings.
- Battery status via VDR.
- Current network status.
- HackOchi uptime for session duration.

**Bluetooth Menu:**

- Custom circular rotary dial HackOchi v1.0’s slickest interface, featuring orbit icons.
- BLE scan with detailed device discovery.
- BT Classic scan for legacy device recon.
- Advertisement waterfall graph for real-time BLE/BT adv packet visualization.

- Central Wi-Fi Dashboard:

    - Displays current network details (channel, SSID, MAC, RSSI, signal strength etc)

    - Signal lock percentage and dynamic strength gauge.

- Wi-Fi reconnaissance: Comprehensive data capture (RSSI, MAC, encryption, etc.).
- Waterfall graph visualizing PPS across all wifi channels.
- Channel switching via encoder for targeted monitoring.


**Pet Menu (Foxie):**

- Reconnaissance status: Current session, open/closed networks, total scans, and more.
- Active pet with dynamic emotions, status updates, and user interactions.
- Pet driven notifications with customizable alerts.
- Flash notifications: Foxie celebrates open networks with vibrant game style animations and jumps
- Eg: If ```Foxie``` finds a open network it shows a fancy game flash and jumps and cries 


**Graph Menu:**

- Military style UI with split screen temperature and humidity graphs for Environmental Mon.

**Temperature Menu:**

- Gradient gauge for current temperature reading.
- Status indicator with average minimum and maximum values.

**Humidity Menu:**

- Gradient gauge for current humidity reading.
- Status indicator with average minimum and maximum values.


## Hardware Progress
Started on a simple breadboard prototype, then moved to a perfboard soldered build for stability.

<p align="center">
  <img src="assets/combined_hardware_progress.png" style="border-radius:15px; margin: 8px;">
</p>  

### Devlogs (Journey of HackOchi v1.0)

### Log 16

https://github.com/user-attachments/assets/c69d4ed6-3c24-478a-9fa3-b2188ecd5793

### Log 15

https://github.com/user-attachments/assets/91beeee7-5dea-4a98-8684-19f50ef7b326

### Log 14

<img width="3440" height="1072" alt="hackochi_devlog14" src="https://github.com/user-attachments/assets/b23a18e8-af12-4143-a2c3-6b8e0bb3f8bb" />


---

## How setup and use HackOchi ?

### 0. Make your connections with HackOchi Pinouts

| Component          | ESP32 Connections                                                                 |
|--------------------|-----------------------------------------------------------------------------------|
| **TFT Touch Display (ILI9341)** | VCC → 5V <br> GND → GND <br> LED → 5V <br> CS → GPIO14<br>DC → GPIO27<br>RST → GPIO33<br>MOSI → GPIO23<br>MISO → GPIO19<br>CLK → GPIO18 <br> Leave touch pins (yet)|
| **VDR** | 10k ohm VDR → GPIO34                                                    |
| **DHT11 Sensor**   | VCC → 5V <br> GND → GND <br> Data → GPIO4                                                                     |
| **Rotary Encoder** | VCC → 5V/3V <br> GND → GND <br> CLK → GPIO32<br>DT → GPIO34<br>SW → GPIO21                                       |


### 1. Clone this repo
```bash
git clone https://github.com/gcloud0x1/hackochi 
cd hackochi/firmware
```


### 2. Create credentials.h
```bash
touch credentials.h
```

### 3. Add Wi-Fi credentials
```bash
#ifndef CREDENTIALS_H
#define CREDENTIALS_H

const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_CAT_NAME_INTERGALACTIC_SUPER_DUPER_ULTRA_MEGA_ASTEROID_DESTROYER";

#endif
```

### 4. Select your board
| Board   | Status       |
|---------|-------------|
| ESP32   | ✅ Supported |
| ESP8266 | ❌ Not Supported  |

### 5. Upload the code 
![b714809bfbdac8da](https://github.com/user-attachments/assets/e6fe0987-4cc4-4645-85a9-8d735c414025)
