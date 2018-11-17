/*
MIT License

Copyright (c) 2017 Marc Haisenko

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef MHGROVEBLE_H
#define MHGROVEBLE_H

#include <Arduino.h>
#include <SoftwareSerial.h>

/** Client/server implementation using Seeed Grove BLE.

 Communication is done via a Stream object which is supposed to be a HardwareSerial
 or SoftwareSerial object (or another compatible object).
 */
class MHGroveBLE {

public:
  /** The state the MHGroveBLE object is in. */
  enum class State {
    /** Initialization is still being executed. */
    initializing,
    /** An unrecoverable error occurred. The object is no longer able to function. */
    panicked,
    /** Waiting for a peer to connect. */
    waitingForConnection,
    /** A peer has connected. */
    connected,
  };

  /** Constructor.

   @param stream The stream to read from and write to. If possible, pass a
    HardwareSerial, otherwise a SoftwareSerial or other compatible object.
   @param name Name of the Bluetooth device. This is what a user sees when
    browsing Bluetooth devices.
   @param rxBufferSize Size of the receive buffer.
   */
  MHGroveBLE(
    Stream & device,
    const char * name,
    unsigned int rxBufferSize = 128
  );

  /** Do any work, if possible.

   Call this in your `loop()` function.
   */
  void runOnce();

  /** Query the current state.
   */
  State getState();

  /** Send data to the peer.

   @param data The data to send.
   @return Whether the data was sent.
   */
  bool send(const String & data);

  /** Set the Bluetooth PIN.

   This must be a string with six digits, from "000000" to "999999".
   If you want to set the PIN, call this function before calling `runOnce()`.
   */
  void setPIN(const char * pin);

  /** Handler: initialization has completed and the receiver is now waiting for
   connections.
   */
  void setOnReady(void (*) ());

  /** Handler: an unrecoverable error occurred. The receiver has shut down.
   */
  void setOnPanic(void (*) ());

  /** Handler: a connection to a peer has been established.
   */
  void setOnConnect(void (*) ());

  /** Handler: a connection to a peer has been closed.
   */
  void setOnDisconnect(void (*) ());

  /** Handler: data has been received from peer.
   */
  void setOnDataReceived(void (*) (const String & data));

  /** Optional debugging function or lambda.
   */
  void setDebug(void (*) (const char * text));

private:
  /** Internal state that should not be exposed. */
  enum class InternalState;
  /** The state of the `receiveResponse` method. */
  enum class ResponseState;

  /** Serial port object for communicating with Grove BLE. */
  Stream & device;
  /** Version of the Grove BLE firmware. */
  long firmwareVersion;
  /** Name of the Bluetooth device. */
  const char * name;
  /** Bluetooth pin as a string. */
  const char * pin;
  /** Receive buffer. */
  String rxBuffer;
  /** Size of the receive buffer. */
  unsigned int rxBufferSize;
  /** The current internal state. */
  InternalState internalState;
  /** For `handleGenericCommand`: the next state to transition to. */
  InternalState genericNextInternalState;
  /** Reference time for the soft timeout. */
  unsigned long softTimeoutReferenceTime;
  /** Duration for the soft timeout. */
  unsigned long softTimeoutDuration;
  /** Reference time for the hard timeout. */
  unsigned long timeoutReferenceTime;
  /** Duration for the hard timeout. */
  unsigned long timeoutDuration;
  /** Handler for completed initialization. */
  void (*onReady) ();
  /** Handler for panic shutdown. */
  void (*onPanic) ();
  /** Handler for established connection. */
  void (*onConnect) ();
  /** Handler for closed connection. */
  void (*onDisconnect) ();
  /** Handler for received data. */
  void (*onDataReceived) (const String & data);
  /** Optional debugging function or lambda. */
  void (*debug) (const char * text);

  /** Send a string to the device. */
  void sendCommand(const String & command);

  /** Receive a response from the device. */
  ResponseState receiveResponse();

  /** Go from one state to the next one and trigger the required action. */
  void transitionToState(InternalState nextState);

  /** Read data from the device into the receive buffer.

   @return Whether data was read.
   */
  bool readIntoBuffer();

  void handleWaitForDevice();
  void handleGenericCommand();
  void handleGetFirmwareVersion();
  void handleReset();
  void handleWaitForConnect();
  void handleConnected();
  void panic();
};

#endif
