/* Barrier Detection v1.0
   By: Sasha Dauz, Robert J. Guziec, Jacob JM Horstman
   Written: April 10, 2025
   
   Hardware:
   H-Bridge Connections
   Pin 1: Kill Switch
   Pin 2: D6 on Arduino
   Pin 3: Left Motor Reverse (Black Wire)
   Pin 6: Left Motor Forward (Red Wire)
   Pin 7: D5 on Arduino
   Pin 10: D11 on Arduino 
   Pin 11: Right Motor Forward (Red Wire)
   Pin 14: Right Motor Reverse (Black Wire)
   Pin 15: D3 on Arduino
   Shared VCC on Pin 1 and Pin 9
   
   I/O Pins:
  A0: LED Left Barrier LED [CORRECTED]
  A1: LED Right Barrier LED [CORRECTED] 
  A2:
  A3:
  A4: D/C Left Wheel Encoder
  A5: D/C Right Wheel Encoder
  D0:
  D1:
  D2:
  D3: Right Motor Forward; (OC2B) Connects to Pin 15 on the H-Bridge
  D4: Left Motor Control ->NOT BEING USED
  D5: [OCR0B] Left Motor Reverse Speed; Connects to H-Bridge Pin 7
  D6: [OCR0A] Left Motor Forward Speed; Connects to H-Bridge Pin 2
  D7:
  D8: D/C Direction Toggle
  D9:
  D10:
  D11: Right Motor Reverse Speed; Connects to Pin 10 on the H-Bridge; OC2A
  D12: Left Whisker [EDIT] Correction: Right Whisker
  D13: Right Whisker [EDIT] Correction: Left Whisker
  To-Do:
  - Fill in I/O pins in header
  - Edit header
  - Edit comments to be professional
*/
