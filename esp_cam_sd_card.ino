#include "FS.h"
#include "SD_MMC.h"   // Uses the onboard microSD on ESP32-CAM
static const char*    CSV_PATH  = "/log.csv";

void setup() {
  
Serial.begin(921600);

  // Mount SD in 1-bit mode (true). Folder mount point is "/sdcard".
  // Note: On many ESP32-CAM boards, 1-bit mode is more stable.
  if (!SD_MMC.begin("/sdcard", /*mode1bit=*/true, /*sdmmc_clk=*/false)) {
    // If mount fails, try default 4-bit as a fallback:
    if (!SD_MMC.begin()) {
      // If both fail, we can't proceed with logging.
      // Avoid blocking; just print an error if you have a USB-UART attached.
      // (If not, this does nothing.)
      Serial.println("SD_MMC mount failed!");
    }
  }

  // Create file with header if it doesn't exist
  if (!SD_MMC.exists(CSV_PATH)) {
    File f = SD_MMC.open(CSV_PATH, FILE_WRITE, true);
    if (f) {
      // Header is optional; keep generic since field count may vary.
      f.println("data"); 
      f.close();
    }
  }
}

void loop() {

   static bool inPacket = false;
  static String buf;           // Holds characters between s...n

  while (Serial.available() > 0) {
    char c = (char)Serial.read();

    if (!inPacket) {
      // Only start collecting after we see 's'
      if (c == 's') {
        inPacket = true;
        buf = "";              // reset buffer for a new packet
      }
      // else ignore any noise before 's'
    } else {
      // We are inside a packet (after seeing 's')
      if (c == 'n') {
        // End of packet -> write to SD if non-empty
        // (Do not include 's' or 'n' in the line)
        if (buf.length() > 0) {
          // Optional: sanitize CR/LF and spaces
          String cleaned = buf;
          cleaned.trim();           // remove stray whitespace
          cleaned.replace("\r",""); // strip CRs
          cleaned.replace("\n",""); // strip LFs
          // Ensure no trailing comma if sender accidentally added one
          if (cleaned.endsWith(",")) cleaned.remove(cleaned.length()-1);

          appendCSVLine(cleaned);
        }
        // Reset for next packet
        inPacket = false;
        buf = "";
      } else {
        // Accumulate payload bytes until 'n'
        buf += c;
      }
    }
  }

  // Yield a little; adjust if your input rate is very high
  delay(1);

}


void appendCSVLine(const String& payloadNoMarkers) {
  File f = SD_MMC.open(CSV_PATH, FILE_APPEND);
  if (!f) return;
  f.println(payloadNoMarkers);  // already CSV-formatted like "44,55,56.01,55"
  f.close();
}
